#include "refl_lua.h"
#include "common/logger.h"

#include <lua/lua.hpp>
#include <memory>

int readLuaInt(lua_State& L, const char* name)
{
    lua_pushstring(&L, name);
    lua_gettable(&L, -2);

    ASSERT(1 == lua_isinteger(&L, -1));
    int ret = lua_tonumber(&L, -1);
    lua_pop(&L, -2);
    return ret;
}

float readLuaFloat(lua_State& L, const char* name)
{
    lua_pushstring(&L, name);
    lua_gettable(&L, -2);

    ASSERT(1 == lua_isnumber(&L, -1));
    float ret = lua_tonumber(&L, -1);
    lua_pop(&L, -2);
    return ret;
}

bool readLuaBool(lua_State& L, const char* name)
{
    lua_pushstring(&L, name);
    lua_gettable(&L, -2);

    ASSERT(1 == lua_isboolean(&L, -1));
    bool ret = lua_toboolean(&L, -1);
    lua_pop(&L, -2);
    return ret;
}

std::string readLuaString(lua_State& L, const char* name)
{
    lua_pushstring(&L, name);
    lua_gettable(&L, -2);

    ASSERT(1 == lua_isstring(&L, -1));
    const char* ret = lua_tostring(&L, -1);
    lua_pop(&L, -2);
    return ret;
}

void readLuaTable(const char* luaFile, const char* luaTable, rttr::instance obj)
{
    std::unique_ptr<lua_State, void (*)(lua_State*)> L(luaL_newstate(), lua_close);
    int r = luaL_dofile(L.get(), luaFile);
    ASSERT_MSG(LUA_OK == r, "lua can't process file: %s", luaFile);

    lua_getglobal(L.get(), luaTable);
    ASSERT_MSG(lua_istable(L.get(), -1), "%s isn't table in file: %s", luaTable, luaFile);

    const auto propList = obj.get_type().get_properties();
    for (const auto& prop : propList) {
        const char* propName = prop.get_name().data();

        if (rttr::type::get<int>() == prop.get_type()) {
            int val = readLuaInt(*L.get(), propName);
            prop.set_value(obj, val);
        } else if (rttr::type::get<float>() == prop.get_type()) {
            float val = readLuaFloat(*L.get(), propName);
            prop.set_value(obj, val);
        } else if (rttr::type::get<bool>() == prop.get_type()) {
            bool val = readLuaBool(*L.get(), propName);
            prop.set_value(obj, val);
        } else if (rttr::type::get<std::string>() == prop.get_type()) {
            std::string val = readLuaString(*L.get(), propName);
            prop.set_value(obj, val);
        } else {
            ASSERT_FAIL("UNKNOWN TYPE");
        }
    }
}
