#pragma once

#include <gtest/gtest.h>
#include <rttr/registration>

static int doubler(int x) {
    return x * 2;
};

struct Bar {
    int one = 1;
    float two = 2.f;
    bool three = true;
    std::string four = "fourString";
};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::method("doubler", &doubler);

    registration::class_<Bar>("Bar")
            .property("one",   &Bar::one)
            .property("two",   &Bar::two)
            .property("three", &Bar::three)
            .property("four",  &Bar::four);
}

TEST(RttR, Function)
{
    EXPECT_EQ(30, rttr::type::invoke("doubler", {15}).to_int());
}

TEST(RttR, SimpleStruct)
{
    Bar bar;
    std::string typeStr = rttr::instance(bar).get_type().get_name().to_string();
    EXPECT_STREQ("Bar", typeStr.c_str());

    EXPECT_TRUE(rttr::instance(bar).get_type().is_class());

    std::string memberTypeStr = rttr::instance(bar).get_type().get_property("three").get_type().get_name().to_string();
    EXPECT_STREQ("bool", memberTypeStr.c_str());
}
