#pragma once

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
    EXPECT_TRUE(lua_isnumber(L, -1));
    int a = (int)lua_tonumber(L, -1);
    EXPECT_EQ(10, a);

    lua_getglobal(L, "b");
    EXPECT_TRUE(lua_isnumber(L, -1));
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

    { // table PLAYER
        lua_getglobal(L, "player");
        EXPECT_TRUE(lua_istable(L, -1));

        { // Name
            lua_pushstring(L, "Name");
            lua_gettable(L, -2);
            std::string name = lua_tostring(L, -1);
            EXPECT_STREQ("Ciaran", name.c_str());
            lua_pop(L, 1);
        }

        { // Title
            lua_pushstring(L, "Title");
            lua_gettable(L, -2);
            std::string title = lua_tostring(L, -1);
            EXPECT_STREQ("Squire", title.c_str());
            lua_pop(L, 1);
        }

        { // Level
            lua_pushstring(L, "Level");
            lua_gettable(L, -2);
            int level = lua_tonumber(L, -1);
            EXPECT_EQ(20, level);
            lua_pop(L, 1);
        }
    }

    { // table info
        lua_getglobal(L, "info");
        EXPECT_TRUE(lua_istable(L, -1));

        { // lol
            lua_pushstring(L, "lol");
            lua_gettable(L, -2);
            std::string name = lua_tostring(L, -1);
            EXPECT_STREQ("LOL", name.c_str());
            lua_pop(L, 1);
        }
    }

    lua_close(L);
}

struct Player {
    std::string Title;
    std::string Name;
    std::string Family;
    int Level;
};

TEST(Lua, ReadStruct)
{
    const std::string scriptFile = SCRIPTS_TEST_DIR + "read_table.lua";
    lua_State* L = luaL_newstate();
    int r = luaL_dofile(L, scriptFile.c_str());
    EXPECT_EQ(LUA_OK, r);

    Player player;

    { // table PLAYER
        lua_getglobal(L, "player");
        EXPECT_TRUE(lua_istable(L, -1));

        { // Name
            lua_pushstring(L, "Name");
            lua_gettable(L, -2);
            player.Name = lua_tostring(L, -1);
            EXPECT_STREQ("Ciaran", player.Name.c_str());
            lua_pop(L, 1);
        }

        { // Title
            lua_pushstring(L, "Title");
            lua_gettable(L, -2);
            player.Title = lua_tostring(L, -1);
            EXPECT_STREQ("Squire", player.Title.c_str());
            lua_pop(L, 1);
        }

        { // Level
            lua_pushstring(L, "Level");
            lua_gettable(L, -2);
            player.Level = lua_tonumber(L, -1);
            EXPECT_EQ(20, player.Level);
            lua_pop(L, 1);
        }
    }
    lua_close(L);
}

int lua_HostFunction(lua_State* L)
{
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);

    std::cout << "[C++] lua_HostFunction(" << a << ", " << b << ") called" << std::endl;
    float c = a * b;
    lua_pushnumber(L, c);

    return 1;
}

TEST(Lua, CallFunction)
{
    const std::string scriptFile = SCRIPTS_TEST_DIR + "call_function.lua";
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int err = luaL_dofile(L, scriptFile.c_str());
    EXPECT_EQ(LUA_OK, err);

    { // AddStuff
        lua_getglobal(L, "AddStuff");
        EXPECT_TRUE(lua_isfunction(L, -1));

        lua_pushnumber(L, 3.5);
        lua_pushnumber(L, 4.5);

        err = lua_pcall(L, 2, 1, 0);
        EXPECT_EQ(LUA_OK, err);
        float res = (float)lua_tonumber(L, -1);
        EXPECT_FLOAT_EQ(8.f, res);
    }

    lua_register(L, "HostFunction", lua_HostFunction);
    lua_register(L, "HostLambda", [](lua_State* L) -> int {
        float a = (float)lua_tonumber(L, 1);
        std::cout << "[C++] lua_HostLambda(" << a << ") called" << std::endl;
        float c = a * 2;
        lua_pushnumber(L, c);
        return 1;
    });

    { // DoAThing
        lua_getglobal(L, "DoAThing");
        EXPECT_TRUE(lua_isfunction(L, -1));

        lua_pushnumber(L, 5.0);
        lua_pushnumber(L, 6.0);

        err = lua_pcall(L, 2, 1, 0);
        EXPECT_EQ(LUA_OK, err);
        float res = (float)lua_tonumber(L, -1);
        EXPECT_FLOAT_EQ(540.f, res);
    }

    lua_close(L);
}
