#include "refl_lua.h"
#include "common/logger.h"

#include <lua/lua.hpp>
#include <memory>
#include <sol/sol.hpp>

static bool __readSubTable(rttr::instance obj, rttr::property prop, sol::table subTable);

template <class LuaVal>
bool __readProperty(rttr::instance obj, rttr::property prop, LuaVal luaVal)
{
    // LuaVal is sol::optional type
    if (rttr::type::get<int>() == prop.get_type()) {
        int val = luaVal.value();
        prop.set_value(obj, val);
    } else if (rttr::type::get<float>() == prop.get_type()) {
        float val = luaVal.value();
        prop.set_value(obj, val);
    } else if (rttr::type::get<bool>() == prop.get_type()) {
        bool val = luaVal.value();
        prop.set_value(obj, val);
    } else if (rttr::type::get<std::string>() == prop.get_type()) {
        std::string val = luaVal.value();
        prop.set_value(obj, val);
    } else if (prop.get_type().is_class()) {
        sol::table subTable = luaVal.value();
        bool ret = __readSubTable(obj, prop, subTable);
        if (!ret) {
            ASSERT_FAIL("Can't read property %s in obj %s", prop.get_name().data(), obj.get_type().get_name().data());
            return false;
        }
    } else {
        ASSERT_FAIL("UnknownType");
        return false;
    }

    return true;
}

static bool __readSubTable(rttr::instance _obj, rttr::property _prop, sol::table _subTable)
{
    ASSERT_FAIL("SUB TABLES IS NOT WORKING");
    const auto propList = _prop.get_type().get_properties();
    for (const auto& prop : propList) {
        const char* propName = prop.get_name().data();

        sol::optional luaVal = _subTable[propName];
        ASSERT_MSG(luaVal, "Can't read field %s", propName);

        bool ret = __readProperty(_obj, prop, luaVal);
        if (!ret) {
            ASSERT_FAIL("Can't read property %s", propName);
            return false;
        }
    }
    return true;
}

bool readLuaTable(sol::state& lua, const char* table, rttr::instance obj)
{
    auto tbl = lua[table];
    ASSERT_MSG(tbl, "Can't find table %s", table);

    const auto propList = obj.get_type().get_properties();
    for (const auto& prop : propList) {
        const char* propName = prop.get_name().data();

        sol::optional luaVal = tbl[propName];
        ASSERT_MSG(luaVal, "Can't read field %s in table %s", propName, table);

        bool ret = __readProperty(obj, prop, luaVal);
        if (!ret) {
            ASSERT_FAIL("Can't read property %s in table %s", propName, table);
            return false;
        }
    }
    return true;
}
