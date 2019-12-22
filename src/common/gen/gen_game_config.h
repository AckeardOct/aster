#pragma once

#include "common/string.h"

namespace sol { class state; }
class WindowCfg;
class DbgGridCfg;

namespace gen {
    void fromLuaTable(sol::state& lua, StringRef tableName, WindowCfg& data);
    void fromLuaTable(sol::state& lua, StringRef tableName, DbgGridCfg& data);
}
