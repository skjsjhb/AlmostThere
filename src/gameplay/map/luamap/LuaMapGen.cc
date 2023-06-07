#include "../Controller.hh"

#include "lua/LuaSupport.hh"
#include "LuaMapGen.hh"
#include "support/Resource.hh"
#include "util/Util.hh"
#include "gameplay/objs/Note.hh"
#include "gameplay/objs/Slot.hh"
#include "gameplay/base/Game.hh"

#include <list>
#include <spdlog/spdlog.h>
#include <fstream>
#include <utility>
#include <any>

using namespace spdlog;

#define CHART_FILE "chart.lua"
#define META_SEC_DELM "-----\n"
#define OBJ_SEC_DELM "----\n"
#define OBJ_HEAD_BODY_DELM "---\n"

// The first number index of the slot
#define SLOT_TYPEID_BEGIN 8
#define TYPE_CAMERA (-1)
#define UNLOAD_TIME_THRESHOLD 1

enum class PositionMethod {
  // RUN: Right, Up, Normal
  ABSOLUTE,    // The value is set as-is, using the world-space XYZ.
  REPROJ,      // The value is set as-is, using RUN as XYZ.
  PERSPECTIVE, // The value is added to the bound object, using RUN as XYZ
  OFFSET,      // The value is added to the bound object, using the world-space XYZ.
};

/**
 * @brief The temporal struct to support Lua map creation.
 *
 * The script will call a native function to register objects. This struct stores the value it provides.
 */
struct LuaMapCreationParams {
  int type = -2, player = 0;
  double genTime = 0, hitTime = std::numeric_limits<double>::max(), length = 0;
  std::string id, bindObj;
  bool isFake;
};

/**
 * @brief An implementation of the controller using Lua map.
 *
 * This controller executes Lua script and retreive the state.
 */
class LuaMapObjectController : public Controller {
public:
  void tick(double absTime) override;
  const ControllerOutput &getOutput() override { return out; };
  const ControllerLifeTime &getLifeTime() override { return lifeTime; };

  static std::shared_ptr<LuaMapObjectController> create(const LuaMapCreationParams &lp,
                                                        const std::string &tickScript);

  // Set the dependency of this controller
  void setDep(std::weak_ptr<LuaMapObjectController> d) {
    if (d.expired()) {
      warn("Setting an expired controller dependency detected. This usually indicates map loader failure.\n");
    }
    dep = std::move(d);
  }

  // Get the dependency
  [[nodiscard]] const std::weak_ptr<LuaMapObjectController> &getDep() const {
    return dep;
  }

  // Intermediate method to get the text rel target (and transform it to pointer)
  [[nodiscard]] const std::string &getRelTargetLiteral() const {
    return tRelTarget;
  }

  // Get the id of the controller, the same as the object
  [[nodiscard]] std::string getID() const {
    return id;
  }

protected:
  std::string tRelTarget, id;
  std::weak_ptr<LuaMapObjectController> dep;
  ControllerLifeTime lifeTime;
  ControllerOutput out;
  int tickScriptId = LUA_NOREF;
  double lastTickTime = -1; // Indicator to prevent multiple tick
};

static std::list<LuaMapCreationParams> objCreationBuffer;

std::shared_ptr<LuaMapObjectController> LuaMapObjectController::create(const LuaMapCreationParams &lp,
                                                                       const std::string &tickScript) {
  auto pid = luaPrecompile(tickScript);
  if (pid == LUA_NOREF) {
    error("Failed to compile tick script. Skipped object creation.");
    return nullptr;
  }

  auto p = std::make_shared<LuaMapObjectController>();
  p->id = lp.id;
  p->tickScriptId = pid;
  p->tRelTarget = lp.bindObj;
  p->lifeTime.genTime = lp.genTime;
  p->lifeTime.hitTime = lp.hitTime;
  p->lifeTime.unloadTime = lp.hitTime + lp.length + UNLOAD_TIME_THRESHOLD;
  p->lifeTime.length = lp.length;
  return p;
}

// __native_objreg(type, id, player, genTime, hitTime, bindObj, length, isFake)
static int objectRegistry(lua_State *l) {
  double length = lua_tonumber(l, -2);
  double hitTime = lua_tonumber(l, -4);
  double genTime = lua_tonumber(l, -5);
  if (genTime < 0) {
    warn("Invalid generate time detected. Generate time cannot be negative.");
    genTime = 0;
  }
  if (hitTime < 0 || hitTime <= genTime) {
    warn("Invalid end time detected. End time cannot be negative, nor smaller than generate time.");
    hitTime = std::numeric_limits<double>::max();
  }
  if (length < 0) {
    warn("Invalid length detected. Length cannot be negative value.");
    length = 0;
  }

  // Create new object
  objCreationBuffer.emplace_back(lua_tointeger(l, -8),
                                 lua_tointeger(l, -6),
                                 genTime,
                                 hitTime,
                                 length,
                                 lua_tostring(l, -7),
                                 lua_tostring(l, -3),
                                 lua_toboolean(l, -1));
  return 1;
}

static bool hasCircularRef(const std::weak_ptr<LuaMapObjectController> &ct) {
  std::set<std::weak_ptr<LuaMapObjectController>, std::owner_less<std::weak_ptr<LuaMapObjectController>>> paths;
  auto cref = ct;
  while (!cref.expired()) {
    paths.insert(cref);
    cref = cref.lock()->getDep();
    if (paths.contains(cref)) {
      return true; // Circular
    }
  }
  return false; // All checked
}

#define UNNAMED_OBJ_PREFIX "_UN_"

MapData generateLuaMap(const std::string &mapId, Game &g) {
  MapData m;
  std::ifstream chartFile(getMapResource(mapId, CHART_FILE));

  if (chartFile.fail()) {
    error("Cannot load map '" + mapId + "'. Either the chart file is missing, or it's not readable.");
    return m;
  }

  std::stringstream ss;
  ss << chartFile.rdbuf();
  std::string content = ss.str();

  auto metaBody = splitStr(content, META_SEC_DELM, 2);
  if (metaBody.size() != 2) {
    error("The structure of map '" + mapId + "' is corrupted. Either meta section or body is missing.");
    return m;
  }

  // Load meta section
  auto metaSec = metaBody[0];

  luaRun(metaSec); // Execute meta script

  m.meta.artist = luaGetStr("Artist");
  m.meta.songName = luaGetStr("SongName");
  m.meta.id = luaGetStr("ID");
  m.meta.mapper = luaGetStr("Mapper");
  m.meta.diffLevel = luaGetInt("DiffLevel");
  m.meta.audio = luaGetStr("Audio");
  m.meta.background = luaGetStr("Background");
  m.meta.duration = luaGetNumber("Duration");

  // Load objects
  auto objSrcs = splitStr(metaBody[1], OBJ_SEC_DELM);

  int unnamedCounter = 0;
  for (auto &os : objSrcs) {
    auto objSecs = splitStr(os, OBJ_HEAD_BODY_DELM, 2);

    if (objSecs.size() != 2) {
      // Invalid
      warn("Incomplete object detected. Skipped object creation.");
      continue;
    }

    // Each section can create multiple items, it's up to us to collect them

    objCreationBuffer.clear();
    luaRun(objSecs[0]); // Register section

    if (objCreationBuffer.empty()) {
      warn("No object registered. The corresponding tick script won't be executed.");
      continue;
    }

    for (auto &param : objCreationBuffer) {
      std::shared_ptr<TickObject> t;
      bool index = true; // Whether this is a named object

      // Automatic id generation for unnamed objects
      if (param.id.empty()) {
        index = false;
        param.id = UNNAMED_OBJ_PREFIX + std::to_string(unnamedCounter);
        unnamedCounter++;
      }

      // Controller generation
      auto ctl = LuaMapObjectController::create(param, objSecs[1]);

      // Game object creation
      if (param.type == TYPE_CAMERA) {
        // This is a camera
        t = Camera::create(g, ctl);
      } else if (param.type >= SLOT_TYPEID_BEGIN) {
        // This is a slot
        t = Slot::create(static_cast<SlotVariant>(param.type - SLOT_TYPEID_BEGIN), g, ctl);
      } else if (param.type >= 0) {
        // This is a note
        t = Note::create(static_cast<NoteType>(param.type), g, ctl, param.isFake);
      } else {
        warn("Unrecognized object type: " + std::to_string(param.type));
        continue; // Pointers will be deleted automatically
      }
      if (index) {
        m.content.nameMap[param.id] = t;
      }
      m.content.objects.push_back(t);
    }
  }

  for (auto &o : m.content.objects) {
    auto ct = std::dynamic_pointer_cast<LuaMapObjectController>(o->getController().lock());
    auto &lt = ct->getRelTargetLiteral();
    if (m.content.nameMap.contains(lt)) {
      auto wpt = m.content.nameMap.at(lt);
      auto spt = std::dynamic_pointer_cast<LuaMapObjectController>(wpt.lock()->getController().lock());
      ct->setDep(spt);
    }
  }

  // Check for circulars
  for (auto &o : m.content.objects) {
    if (hasCircularRef(std::dynamic_pointer_cast<LuaMapObjectController>(o->getController().lock()))) {
      warn("Circular reference detected during linking. Check your map.");
      std::weak_ptr<LuaMapObjectController> emptyPt;
      std::dynamic_pointer_cast<LuaMapObjectController>(o->getController().lock())->setDep(emptyPt);
    }
  }

  // Sort objects
  m.content.objects.sort([](const std::shared_ptr<TickObject> &a, const std::shared_ptr<TickObject> &b) -> bool {
    return a->getGenTime() < b->getGenTime();
  });
  return m;
}

// Interaction with Lua
enum InVarType {
  NUMBER,
  NIL,
  STRING,
  BOOL,
  TABLE // Out only
};

static std::unordered_map<std::string, std::pair<InVarType, std::any>> inValuePool, outValuePool;

static int incImpl(lua_State *l) {
  auto cid = std::string(lua_tostring(l, -1));
  if (inValuePool.contains(cid)) {
    auto v = inValuePool[cid];
    switch (v.first) {
    case NUMBER:lua_pushnumber(l, std::any_cast<double>(v.second));
      return 1;
    case STRING:lua_pushstring(l, std::any_cast<std::string>(v.second).c_str());
      return 1;
    case BOOL:lua_pushboolean(l, std::any_cast<bool>(v.second));
      return 1;
    case NIL:
    default:break;
    }
  }
  lua_pushnil(l);
  return 0;
}

static int outImpl(lua_State *l) {
  auto cid = std::string(lua_tostring(l, -2));
  switch (lua_type(l, -1)) {
  case LUA_TNUMBER:outValuePool[cid] = std::pair(NUMBER, lua_tonumber(l, -1));
    return 1;
  case LUA_TBOOLEAN:outValuePool[cid] = std::pair(BOOL, lua_toboolean(l, -1));
    return 1;
  case LUA_TSTRING:outValuePool[cid] = std::pair(STRING, std::string(lua_tostring(l, -1)));
    return 1;
  case LUA_TTABLE:outValuePool[cid] = std::pair(TABLE, luaBuildTable(-1));
    return 1;
  default:
    // No-op
    return 0;
  }
}

static void readVec3(const std::string &outName, glm::vec3 &d) {
  if (!outValuePool.contains(outName)) {
    d = {0, 0, 0};
    return;
  }
  auto t = outValuePool[outName];
  if (t.first == TABLE) {
    for (int i = 0; i < 3; i++) {
      d[i] = std::stof(std::any_cast<StringTable>(t.second)[std::to_string(i + 1)]);
    }
  }
}

static double readNumber(const std::string &outName, double dv) {
  if (!outValuePool.contains(outName)) {
    return dv;
  }
  double p = dv;
  auto iva = outValuePool[outName];
  if (iva.first == NUMBER) {
    p = std::any_cast<double>(iva.second);
  }
  return p;
}

// Reinterpret current vector using the base vector and position method indicator
static void relVec(glm::vec3 &org, glm::vec3 base, glm::vec3 er, glm::vec3 eu, glm::vec3 en, PositionMethod md) {
  glm::vec3 p;
  switch (md) {
  case PositionMethod::REPROJ:
    for (int i = 0; i < 3; i++) {
      p[i] = org[0] * er[i] + org[1] * eu[i] + org[2] * en[i];
    }
    break;
  case PositionMethod::PERSPECTIVE:
    for (int i = 0; i < 3; i++) {
      p[i] = base[i] + org[0] * er[i] + org[1] * eu[i] + org[2] * en[i];
    }
    break;
  case PositionMethod::OFFSET:
    for (int i = 0; i < 3; i++) {
      p[i] = base[i] + org[i];
    }
    break;
  case PositionMethod::ABSOLUTE:
  default:return;
  }
  org = p;
}

// Constant labels

namespace LuaScriptIdentifiers {

const std::string RELMODE = "RELMODE";
const std::string POS = "POS";
const std::string UP = "UP";
const std::string NORMAL = "NORMAL";
const std::string PCT = "PCT";
const std::string TIME = "TIME";
const std::string ATIME = "ATIME";
const std::string OBJID = "OBJID";
const std::string ALPHA = "ALPHA";

const std::string NA_OBJREG = "__native_objreg";
const std::string NA_INC = "__native_inc";
const std::string NA_OUT = "__native_out";

}

// Controller impl

void LuaMapObjectController::tick(double absTime) { // NOLINT Recursion is necessary here
  if (lastTickTime == absTime) {
    // Up to date
    return;
  }

  if (!dep.expired()) {
    // Tick deps first
    dep.lock()->tick(absTime);
  }
  // Sync time flag
  lastTickTime = absTime;

  // Calculate the length value first
  // Note: length value is calculated by the system and cannot be modified by the script.
  // This is worked as intended.
  if (lifeTime.length != 0) {

    if (absTime <= lifeTime.hitTime) {
      // Not touched yet
      out.len = lifeTime.length;
    } else if (absTime < lifeTime.unloadTime && absTime > lifeTime.hitTime) {
      // Is 'consuming'
      out.len = lifeTime.length - (absTime - lifeTime.hitTime);
    } else {
      // Done
      out.len = 0;
    }
  } else {
    out.len = 0;
  }

  // Calculate related variable

  // Total time: from generation to hit
  double time = lifeTime.hitTime - lifeTime.genTime;
  double pct;

  if (time < 0) {
    time = 0;
  }

  if (time == 0) {
    pct = 1;
  } else {
    pct = (absTime - lifeTime.genTime) / time;
  }


  // Correction
  if (pct > 1) {
    pct = 1;
  }
  if (pct < 0) {
    pct = 0;
  }


  // Pass in variables
  inValuePool[LuaScriptIdentifiers::PCT] = std::pair(NUMBER, std::any(pct));
  inValuePool[LuaScriptIdentifiers::TIME] = std::pair(NUMBER, std::any(time));
  inValuePool[LuaScriptIdentifiers::ATIME] = std::pair(NUMBER, std::any(absTime));
  inValuePool[LuaScriptIdentifiers::OBJID] = std::pair(STRING, std::any(id));

  // Eval script
  luaRunCompiledCode(tickScriptId);

  // Clean input
  inValuePool.clear();

  if (outValuePool.empty()) {
    // No updates, skipped
    return;
  }

  // Extract values
  // This is not the final coord
  glm::vec3 iPos, iUp, iNorm, tMode;
  int mode[3];
  readVec3(LuaScriptIdentifiers::POS, iPos);
  readVec3(LuaScriptIdentifiers::UP, iUp);
  readVec3(LuaScriptIdentifiers::NORMAL, iNorm);
  readVec3(LuaScriptIdentifiers::RELMODE, tMode);

  for (int i = 0; i < 3; i++) {
    mode[i] = (int) tMode[i];
  }

  out.alpha = readNumber(LuaScriptIdentifiers::ALPHA, 1);

  // Normalize vector
  iUp = glm::normalize(iUp);
  iNorm = glm::normalize(iNorm);

  // Make rel
  if (!dep.expired()) {
    auto depx = dep.lock();
    auto dOutput = depx->getOutput();
    auto tUp = dOutput.up;
    auto tNorm = dOutput.norm;
    auto tPos = dOutput.pos;
    auto tRight = glm::cross(tUp, tNorm);
    tRight = glm::normalize(tRight);

    // Update relative position method
    relVec(iPos, tPos, tRight, tUp, tNorm, (PositionMethod) mode[0]);
    relVec(iUp, tPos, tRight, tUp, tNorm, (PositionMethod) mode[1]);
    relVec(iNorm, tPos, tRight, tUp, tNorm, (PositionMethod) mode[2]);
  }

  iUp = glm::normalize(iUp);
  iNorm = glm::normalize(iNorm);
  out.pos = iPos;
  out.up = iUp;
  out.norm = iNorm;

  // Cleanup output
  outValuePool.clear();
}
void initLuaMapLoader() {
  luaBind(LuaScriptIdentifiers::NA_OBJREG, objectRegistry);
  luaBind(LuaScriptIdentifiers::NA_INC, incImpl);
  luaBind(LuaScriptIdentifiers::NA_OUT, outImpl);
}
