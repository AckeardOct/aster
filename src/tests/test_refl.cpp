#include "refl/refl_json.h"
#include "refl/refl_lua.h"

#include <gtest/gtest.h>
#include <sol/sol.hpp>

struct t_Refl {
    int one = 0;
    float two = 0.f;
    bool three = false;
    std::string four;
};

struct t_ReflA {
    int two = 0;
};

struct t_ReflB {
    float one = 0.f;
    t_ReflA complex; // TODO: subtables is not working. see jsom example
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<t_Refl>("t_Refl")
        .property("one", &t_Refl::one)
        .property("two", &t_Refl::two)
        .property("three", &t_Refl::three)
        .property("four", &t_Refl::four);

    registration::class_<t_ReflA>("t_ReflA")
        .property("two", &t_ReflA::two);

    registration::class_<t_ReflB>("t_ReflB")
        .property("one", &t_ReflB::one)
        .property("complex", &t_ReflB::complex);
}

TEST(ReflLua, readLuaTable)
{
    t_Refl data;
    sol::state lua;
    lua.script(R"( data = {
                    one = 1349,
                    two = 14.88,
                    three = true,
                    four = "Hello World"
                   }
               )");
    readLuaTable(lua, "data", data);
    EXPECT_EQ(1349, data.one);
    EXPECT_FLOAT_EQ(14.88, data.two);
    EXPECT_EQ(true, data.three);
    EXPECT_EQ("Hello World", data.four);
}

TEST(ReflLua, readLuaTableRecur)
{
    t_ReflB data;
    sol::state lua;
    lua.script(R"( data = {
                    one = 13.49,
                    complex = {
                        two = 1488
                        }
                   }
               )");
    //readLuaTable(lua, "data", data);
    //EXPECT_FLOAT_EQ(13.49, data.one);
    //EXPECT_EQ(1488, data.complex.two);
}
