#include "gen_game_config.h"

#include "../game_config.h"
#include "common/logger.h"

#include <nlohmann/json.hpp>
#include <sol/sol.hpp>

void gen::fromLuaTable(sol::state& lua, StringRef tableName, WindowCfg& data)
{
    sol::table table = lua[tableName];
    ASSERT_MSG(table, "Can't find table %s", tableName.data());

    {
        StringRef member = "width";
        auto& dataVal = data.width;
        sol::optional luaVal = table[member];
        ASSERT_MSG(luaVal, "Can't read field %s in table %s", member.data(), tableName.data());
        ASSERT_MSG(sol::type::number == luaVal->get_type(), "wrong type %d", luaVal->get_type());
        int val = luaVal->get<int>();
        dataVal = val;
    }

    {
        StringRef member = "height";
        auto& dataVal = data.height;
        sol::optional luaVal = table[member];
        ASSERT_MSG(luaVal, "Can't read field %s in table %s", member.data(), tableName.data());
        ASSERT_MSG(sol::type::number == luaVal->get_type(), "wrong type %d", luaVal->get_type());
        int val = luaVal->get<int>();
        dataVal = val;
    }

    {
        StringRef member = "fps";
        auto& dataVal = data.fps;
        sol::optional luaVal = table[member];
        ASSERT_MSG(luaVal, "Can't read field %s in table %s", member.data(), tableName.data());
        ASSERT_MSG(sol::type::number == luaVal->get_type(), "wrong type %d", luaVal->get_type());
        int val = luaVal->get<int>();
        dataVal = val;
    }

    {
        StringRef member = "title";
        auto& dataVal = data.title;
        sol::optional luaVal = table[member];
        ASSERT_MSG(luaVal, "Can't read field %s in table %s", member.data(), tableName.data());
        ASSERT_MSG(sol::type::string == luaVal->get_type(), "wrong type %d", luaVal->get_type());
        String val = luaVal->get<String>();
        dataVal = val;
    }
}

Json gen::toJson(WindowCfg& data)
{
    Json ret;

    ret["width"] = data.width;
    ret["height"] = data.height;
    ret["fps"] = data.fps;
    ret["title"] = data.title;

    return ret;
}
