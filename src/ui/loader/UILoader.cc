#include "UILoader.hh"

#include "cJSON.h"
#include "support/Resource.hh"

#include <fstream>
#include <memory>
#include <sstream>
#include <spdlog/spdlog.h>

#include "../comp/impl/Box.hh"

static std::shared_ptr<Component> createTypedComponent(const std::string &type, const ComponentProps &props) {
  if (type == "box" || type == "root") {
    return std::make_shared<Box>(props);
  }
  spdlog::warn("Unknown UI component type '" + type + "'");
  return nullptr;
}

static std::shared_ptr<Component> constructComponent(cJSON *st) { // NOLINT
  ComponentProps props;

  // Get ID
  auto id = st->string;
  std::string pid = "root";
  if (id != nullptr) {
    pid = id;
  }

  // Set type
  auto stType = cJSON_GetObjectItem(st, "type");
  if (!cJSON_IsString(stType)) {
    spdlog::warn("Missing type during ui constructing. Skipped component.");
    return nullptr;
  }
  auto tp = std::string(cJSON_GetStringValue(stType));

  // Put props
  auto stProps = cJSON_GetObjectItem(st, "props");

  if (cJSON_IsObject(stProps)) {
    cJSON *p;
    cJSON_ArrayForEach(p, stProps) {
      auto k = p->string;
      auto v = cJSON_GetStringValue(p);
      if (k && v) {
        props[k] = v;
      }
    }
  }

  // Process children
  std::list<std::shared_ptr<Component>> chd;
  auto cd = cJSON_GetObjectItem(st, "children");
  if (cJSON_IsObject(cd)) {
    cJSON *c;
    cJSON_ArrayForEach(c, cd) {
      auto cp = constructComponent(c);
      if (cp) {
        chd.push_back(cp);
      }
    }
  }

  auto comp = createTypedComponent(tp, props);
  Component::patchComponent(comp, pid, std::move(chd));
  return comp;
}

std::shared_ptr<Component> loadUITree(const std::string &uiName) {
  auto ft = getAppResource("ui/" + uiName + ".json");

  std::ifstream f(ft);
  if (f.fail()) {
    spdlog::error("Could not load ui for '" + uiName + "'");
    return nullptr;
  }

  std::stringstream ss;
  ss << f.rdbuf();
  auto content = ss.str();
  auto tree = cJSON_Parse(content.c_str());
  if (!tree) {
    spdlog::error("Corrupted ui content in '" + uiName + "'");
    return nullptr;
  }
  auto c = constructComponent(tree);
  cJSON_Delete(tree);
  return c;
}
