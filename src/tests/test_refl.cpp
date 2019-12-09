#include "refl/refl_json.h"
#include "refl/refl_lua.h"

#include <gtest/gtest.h>
#include <sol/sol.hpp>

struct t_Refl2 {
    int lol = 0;
};

struct t_Refl {
    int one = 0;
    float two = 0.f;
    bool three = false;
    std::string four;
    t_Refl2 complex; // TODO: not working.
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<t_Refl>("t_Refl")
        .property("one", &t_Refl::one)
        .property("two", &t_Refl::two)
        .property("three", &t_Refl::three)
        .property("four", &t_Refl::four);

    registration::class_<t_Refl2>("t_Refl2")
        .property("lol", &t_Refl2::lol);
}

TEST(ReflLua, readLuaTable)
{
    t_Refl data;
    sol::state lua;
    lua.script(R"( data = {
                    one = 1349,
                    two = 14.88,
                    three = true,
                    four = "Hello World",
                    complex = {
                        lol = 5;
                        }
                   }
               )");
    readLuaTable(lua, "data", data);
    EXPECT_EQ(1349, data.one);
    EXPECT_FLOAT_EQ(14.88, data.two);
    EXPECT_EQ(true, data.three);
    EXPECT_EQ("Hello World", data.four);
}
