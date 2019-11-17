#include <gtest/gtest.h>

#include <lua/lua.hpp>

static const std::string SCRIPTS_TEST_DIR = "./scripts/test/";

TEST(Lua, ReadGlobals)
{
    lua_State* L = luaL_newstate();
    int r = luaL_dostring(L, "a = 7 + 3");
    EXPECT_EQ(LUA_OK, r);
    r = luaL_dostring(L, "b = 9 - 4");
    EXPECT_EQ(LUA_OK, r);

    lua_getglobal(L, "a");
    EXPECT_EQ(true, lua_isnumber(L, -1));
    int a = (int)lua_tonumber(L, -1);
    EXPECT_EQ(10, a);

    lua_getglobal(L, "b");
    EXPECT_EQ(true, lua_isnumber(L, -1));
    int b = (int)lua_tonumber(L, -1);
    EXPECT_EQ(5, b);

    lua_close(L);
}

TEST(Lua, ReadTable)
{
    const std::string scriptFile = SCRIPTS_TEST_DIR + "read_table.lua";
    lua_State* L = luaL_newstate();
    int r = luaL_dofile(L, scriptFile.c_str());
    EXPECT_EQ(LUA_OK, r);

    lua_getglobal(L, "table");
    EXPECT_EQ(true, lua_istable(L, -1));

    lua_close(L);
}
