#include "game_config.h"
#include "common/logger.h"
#include "refl/refl_lua.h"

#include <SDL2/SDL.h>
#include <lua/lua.hpp>
#include <rttr/registration>
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
    LogMsg("Config: %s", toString().c_str());
}

std::string GameConfig::toString() const
{
    std::stringstream ret;
    ret << "luaConfigFile: '" << luaConfigFile << "'";
    ret << ", width: " << window.width;
    ret << ", height: " << window.height;
    ret << ", fps: " << window.fps;
    ret << ", titls: '" << window.title << "'";
    return ret.str();
}

bool GameConfig::initByLua()
{
    readLuaTable(luaConfigFile, "window", window);
    readLuaTable(luaConfigFile, "dbgGrid", dbgDrawGrid);
    return true;
}
