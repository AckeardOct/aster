#include "refl/refl_json.h"

#include <gtest/gtest.h>
#include <rttr/registration>

struct t_Refl {
    int one = 0;
    float two = 0.f;
    bool three = false;
    std::string four;
};

struct t_ReflA {
    int two = 0;
    t_ReflA() = default;
    t_ReflA(int var)
        : two(var)
    {
    }
};

struct t_ReflB {
    float one = 0.f;
    t_ReflA complex; // TODO: subtables is not working. see jsom example
};

struct t_ReflC {
    float one = 0.f;
    std::vector<int> arr;
    std::vector<std::vector<int>> arr2;
    std::vector<t_ReflA> arr3;
};

struct t_ReflD {
    bool one = false;
    std::map<std::string, int> ass;
    std::map<std::string, std::vector<int>> ass2;
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
        .constructor<>()
        .constructor<int>()
        .property("two", &t_ReflA::two);

    registration::class_<t_ReflB>("t_ReflB")
        .property("one", &t_ReflB::one)
        .property("complex", &t_ReflB::complex);

    registration::class_<t_ReflC>("t_ReflC")
        .property("one", &t_ReflC::one)
        .property("arr", &t_ReflC::arr)
        .property("arr2", &t_ReflC::arr2)
        .property("arr3", &t_ReflC::arr3);

    registration::class_<t_ReflD>("t_ReflD")
        .property("one", &t_ReflD::one)
        .property("ass", &t_ReflD::ass)
        .property("ass2", &t_ReflD::ass2);
}

TEST(ReflJson, toJsonPrimitive)
{
    t_Refl data;
    data.one = 1488;
    data.two = 13.49f;
    data.three = true;
    data.four = "Hello World";

    Json js = toJson(data);
    EXPECT_EQ(1488, js["one"]);
    EXPECT_FLOAT_EQ(13.49f, js["two"]);
    EXPECT_EQ(true, js["three"]);
    EXPECT_EQ("Hello World", js["four"]);
}

TEST(ReflJson, fromJsonPrimitive)
{
    Json js = Json::parse(R"({
                            "two" : 1488
                          })");

    t_ReflA data;
    bool success = fromJson(js, data);
    EXPECT_TRUE(success);
    EXPECT_EQ(1488, data.two);
}

TEST(ReflJson, toJsonVector)
{
    t_ReflC data;
    data.one = 13.49f;
    data.arr = { 1, 2 };
    data.arr2 = { { 1, 2 }, { 3, 4 } };
    data.arr3 = { t_ReflA(1), t_ReflA(2) };

    Json js = toJson(data);

    EXPECT_FLOAT_EQ(13.49f, js["one"]);
    EXPECT_EQ(1, js["arr"][0]);
    EXPECT_EQ(2, js["arr"][1]);

    EXPECT_EQ(3, data.arr2[1][0]);

    EXPECT_EQ(2, js["arr3"][1]["two"]);
}

TEST(ReflJson, toJsonMap)
{
    t_ReflD data;
    data.one = true;
    data.ass = { { "1488", 1488 }, { "plague", 1349 } };
    data.ass2 = { { "first", { 1, 2, 3 } }, { "second", { 4, 5, 6 } } };

    Json js = toJson(data);

    EXPECT_EQ(true, js["one"]);
    EXPECT_EQ(1488, js["ass"]["1488"]);
    EXPECT_EQ(1349, js["ass"]["plague"]);

    EXPECT_EQ(2, js["ass2"]["first"][1]);
    std::vector<int> vec = { 4, 5, 6 };
    EXPECT_EQ(vec, js["ass2"]["second"]);
}
