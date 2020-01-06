#pragma once

#include "common/string.h"

#include <nlohmann/json_fwd.hpp>
using Json = nlohmann::json;

namespace sol {
class state;
}

class WindowCfg;
namespace gen {
void fromLuaTable(sol::state& lua, StringRef tableName, WindowCfg& data);
Json toJson(WindowCfg& data);
}
