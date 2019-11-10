#include "game_config.h"

#include "logger.h"
#include <lua/lua.hpp>
#include <sstream>

GameConfig::GameConfig(int argc, char** argv)
{
    initByLua();
    LogMsg("Config: %s", toString().c_str());
}

std::string GameConfig::toString() const
{
    std::stringstream ret;
    ret << "luaConfigFile: '" << luaConfigFile << "'";
    ret << ", width: " << width;
    ret << ", height: " << height;
    ret << ", fps: " << fps;
    ret << ", titls: '" << title << "'";
    return ret.str();
}

bool GameConfig::initByLua()
{
    lua_State* L = luaL_newstate();
    luaL_dofile(L, luaConfigFile);
    {
        int stackIdx = 1;
        lua_getglobal(L, "width");
        width = lua_tonumber(L, stackIdx++);
        lua_getglobal(L, "height");
        height = lua_tonumber(L, stackIdx++);
        lua_getglobal(L, "fps");
        fps = lua_tonumber(L, stackIdx++);
        lua_getglobal(L, "title");
        title = lua_tostring(L, stackIdx++);
    }
    lua_close(L);
    return true;
}
