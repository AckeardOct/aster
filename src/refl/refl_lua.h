#pragma once

#include <rttr/registration>
#include <sol/forward.hpp>

bool readLuaTable(sol::state& lua, const char* table, rttr::instance obj);
