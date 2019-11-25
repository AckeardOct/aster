#include "refl/refl_json.h"

#include <gtest/gtest.h>

struct t_Refl {
    int one = 1488;
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<t_Refl>("t_Refl")
        .property("one", &t_Refl::one);
}

TEST(ReflJson, Simple)
{
    t_Refl foo;
    //Json j = toJson(foo);
    //int one = j["one"];
    //ASSERT_EQ(1488, one);
}
