#include "game_config.h"
#include "common/logger.h"
#include "refl/refl_json.h"
#include "refl/refl_lua.h"

#include <rttr/registration>
#include <sol/sol.hpp>
#include <sstream>

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<WindowCfg>("WindowCfg")
        .property("width", &WindowCfg::width)
        .property("height", &WindowCfg::height)
        .property("fps", &WindowCfg::fps)
        .property("title", &WindowCfg::title);

    registration::class_<DbgGridCfg>("DbgGridCfg")
        .property("enable", &DbgGridCfg::enable)
        .property("resolutionX", &DbgGridCfg::resolutionX)
        .property("resolutionY", &DbgGridCfg::resolutionY)
        .property("offsetX", &DbgGridCfg::offsetX)
        .property("offsetY", &DbgGridCfg::offsetY);
}

GameConfig::GameConfig(int argc, char** argv)
{
    initByLua();
    LogMsg("LuaInited by file: %s", luaConfigFile);
    LogMsg("cfg window = %s", toJson(window).dump(4).c_str());
    LogMsg("cfg debugGrid = %s", toJson(dbgDrawGrid).dump(4).c_str());
}

bool GameConfig::initByLua()
{
    sol::state lua;
    lua.script_file(luaConfigFile);

    readLuaTable(lua, "window", window);
    readLuaTable(lua, "dbgGrid", dbgDrawGrid);

    return true;
}
