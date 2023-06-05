#include "LuaExt.hh"

#define _USE_MATH_DEFINES // NOLINT

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "LuaSupport.hh"

static double sinValue[361], cosValue[361];

static int stdDeg(double rad) {
  int refDeg = static_cast<int>(rad / (2 * M_PI) * 360) % 360;
  if (refDeg < 0) {
    refDeg = 360 + refDeg;
  }
  return refDeg;
}

static int quickSin(lua_State *l) {
  double rad = lua_tonumber(l, -1);
  auto d = stdDeg(rad);
  lua_pushnumber(l, 0.5 * (sinValue[d] + sinValue[d + 1]));
  return 1;
}

static int quickCos(lua_State *l) {
  double rad = lua_tonumber(l, -1);
  auto d = stdDeg(rad);
  lua_pushnumber(l, 0.5 * (cosValue[d] + cosValue[d + 1]));
  return 1;
}

static int quickTan(lua_State *l) {
  double rad = lua_tonumber(l, -1);
  auto d = stdDeg(rad);
  lua_pushnumber(l, 0.5 * (sinValue[d] / cosValue[d] + sinValue[d + 1] / cosValue[d + 1]));
  return 1;
}

static int rawRotate(lua_State *l) {
  // Sig: __native_rotate(vec3 target, vec3 axis, double radius)

  if (lua_istable(l, -2) && lua_istable(l, -3) & lua_isnumber(l, -1)) {
    glm::vec3 cpTarget, cpAxis;
    float rad;

    for (int i = 0; i < 3; i++) {
      lua_pushinteger(l, i + 1);
      lua_gettable(l, -4);
      cpTarget[i] = lua_tonumber(l, -1);
      lua_remove(l, -1);
    }

    for (int i = 0; i < 3; i++) {
      lua_pushinteger(l, i + 1);
      lua_gettable(l, -3);
      cpAxis[i] = lua_tonumber(l, -1);
      lua_remove(l, -1);
    }
    rad = lua_tonumber(l, -1);
    cpTarget = glm::rotate(cpTarget, rad, cpAxis);
    for (int i = 0; i < 3; i++) {
      lua_pushnumber(l, i + 1);
      lua_pushnumber(l, cpTarget[i]);
      lua_settable(l, -5);
    }
  }

  return 1;
}

void luaSetupExt() {
  for (int i = 0; i < 360; i++) {
    sinValue[i] = std::sin(i / 180.0 * M_PI);
    cosValue[i] = std::cos(i / 180.0 * M_PI);
  }
  sinValue[360] = sinValue[0];
  cosValue[360] = cosValue[0];
  luaBind("__native_qsin", quickSin);
  luaBind("__native_qcos", quickCos);
  luaBind("__native_qtan", quickTan);
  luaBind("__native_raw_rotate", rawRotate);
}
