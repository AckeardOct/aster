#include "game_config.h"
#include "common/logger.h"

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

GameConfig::GameConfig(int argc, char** argv)
{
    initByLua();
    LogMsg("LuaInited by file: %s", luaConfigFile.data());
    LogMsg("cfg window = %s", gen::toJson(window).dump(4).c_str());
}

void GameConfig::initByLua()
{
    sol::state lua;
    lua.script_file(luaConfigFile.data());

    gen::fromLuaTable(lua, "window", window);
}
