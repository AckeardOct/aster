#include <gtest/gtest.h>
#include <rttr/registration>

static int t_doubler(int x)
{
    return x * 2;
};

struct t_Bar {
    int one = 1;
    float two = 2.f;
    bool three = true;
    std::string four = "fourString";
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::method("t_doubler", &t_doubler);

    registration::class_<t_Bar>("t_Bar")
        .property("one", &t_Bar::one)
        .property("two", &t_Bar::two)
        .property("three", &t_Bar::three)
        .property("four", &t_Bar::four);
}

TEST(RttR, Function)
{
    EXPECT_EQ(30, rttr::type::invoke("t_doubler", { 15 }).to_int());
}

TEST(RttR, SimpleStruct)
{
    t_Bar bar;
    std::string typeStr = rttr::instance(bar).get_type().get_name().to_string();
    EXPECT_STREQ("t_Bar", typeStr.c_str());

    EXPECT_TRUE(rttr::instance(bar).get_type().is_class());

    std::string memberTypeStr = rttr::instance(bar).get_type().get_property("three").get_type().get_name().to_string();
    EXPECT_STREQ("bool", memberTypeStr.c_str());
}
