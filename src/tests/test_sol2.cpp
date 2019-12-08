#include <gtest/gtest.h>
#include <sol/sol.hpp>

static const std::string SCRIPTS_TEST_DIR = "./scripts/test/";

TEST(Sol2, HelloWorld)
{
    struct vars {
        int boop = 0;
    };

    sol::state lua;
    lua.new_usertype<vars>("vars", "boop", &vars::boop);
    lua.script("beep = vars.new()\n"
               "beep.boop = 1");
    EXPECT_EQ(1, lua.get<vars>("beep").boop);
}

TEST(Sol2, Reading)
{
    const std::string luaFile = SCRIPTS_TEST_DIR + "/read_table.lua";

    sol::state lua;
    lua.script_file(luaFile);

    std::string Title = lua["player"]["Title"]; // can get nested variables
    EXPECT_EQ("Squire", Title);

    sol::table config = lua["player"];
    int Level = config["Level"];
    EXPECT_EQ(20, Level);

    auto name = config["InvalidName"];
    bool isValid = name.valid();
    EXPECT_FALSE(isValid);

    sol::optional<std::string> family = config["Family"];
    EXPECT_TRUE(family);
    EXPECT_TRUE(family.has_value());
    EXPECT_EQ("Wirral", family.value());
}

TEST(Sol2, Writing)
{
    sol::state lua;

    // open those basic lua libraries
    // again, for print() and other basic utilities
    lua.open_libraries(sol::lib::base);

    // value in the global table
    lua["bark"] = 50;

    // a table being created in the global table
    lua["some_table"] = lua.create_table_with(
        "key0", 24,
        "key1", 25,
        lua["bark"], "the key is 50 and this string is its value!");

    // Run a plain ol' string of lua code
    // Note you can interact with things set through sol in C++ with lua!
    // Using a "Raw String Literal" to have multi-line goodness:
    // http://en.cppreference.com/w/cpp/language/string_literal
    lua.script(R"(

        print(some_table[50])
        print(some_table["key0"])
        print(some_table["key1"])

        -- a lua comment: access a global in a lua script with the _G table
        print(_G["bark"])

        )");

    ASSERT_EQ(24, lua["some_table"]["key0"]);
    ASSERT_EQ(25, lua["some_table"]["key1"]);

    std::string bark = lua["some_table"][50];
    ASSERT_EQ("the key is 50 and this string is its value!", bark);
}

TEST(Sol2, Functions)
{
    bool success = false;
    auto my_function = [&success](int a, std::string b) -> std::string {
        success = true;
        return b + std::string('D', a);
    };

    sol::state lua;

    lua["my_func"] = my_function; // way 1
    lua.set("my_func", my_function); // way 2
    lua.set_function("my_func", my_function); // way 3

    // This function is now accessible as 'my_func' in
    // lua scripts / code run on this state:
    lua.script("some_str = my_func(1, 'Da')");

    // Read out the global variable we stored in 'some_str' in the
    // quick lua code we just executed
    std::string some_str = lua["some_str"];
    // some_str == "DaD"

    EXPECT_TRUE(success);
}

TEST(Sol2, MemberFunctions)
{
    struct Bar {
        int a = 0;

        Bar() = default;
        Bar(int x)
            : a(x)
        {
        }

        int foo()
        {
            ++a; // increment a by 1
            return a;
        }
    };

    sol::state lua;

    // Here, we are binding the member function and a class instance: it will call the function on
    // the given class instance
    lua.set_function("my_class_func", &Bar::foo, Bar());

    // We do not pass a class instance here:
    // the function will need you to pass an instance of "my_class" to it
    // in lua to work, as shown below
    lua.set_function("my_class_func_2", &Bar::foo);

    // With a pre-bound instance:
    lua.script(R"(
                    first_value = my_class_func()
                    second_value = my_class_func()
            )");
    // first_value == 1
    // second_value == 2

    // With no bound instance:
    Bar bar(24);
    lua.set("obj", bar);
    // Calls "func" on the class instance
    // referenced by "obj" in Lua
    lua.script(R"(
                    third_value = my_class_func_2(obj)
                    fourth_value = my_class_func_2(obj)
            )");
    // first_value == 25
    // second_value == 26
}

template <typename A, typename B>
A my_add(A a, B b)
{
    return a + b;
}

TEST(Sol2, TemplateFunctions)
{
    sol::state lua;

    // adds 2 integers
    lua["my_int_add"] = my_add<int, int>;

    // concatenates 2 strings
    lua["my_string_combine"] = my_add<std::string, std::string>;

    lua.script("my_num = my_int_add(1, 2)");
    int my_num = lua["my_num"];
    // my_num == 3
    EXPECT_EQ(3, my_num);

    lua.script("my_str = my_string_combine('bark bark', ' woof woof')");
    std::string my_str = lua["my_str"];
    // my_str == "bark bark woof woof"
    EXPECT_EQ("bark bark woof woof", my_str);

    // OVERLOAD
    // adds 2 integers
    lua["my_combine"] = sol::overload(my_add<int, int>, my_add<std::string, std::string>);

    lua.script("my_num = my_combine(1, 2)");
    lua.script("my_str = my_combine('bark bark', ' woof woof')");
    int my_num2 = lua["my_num"];
    // my_num == 3
    EXPECT_EQ(3, my_num2);
    std::string my_str2 = lua["my_str"];
    // my_str == "bark bark woof woof"
    EXPECT_EQ("bark bark woof woof", my_str2);
}

TEST(Sol2, FunctionsFromLua)
{
    sol::state lua;

    lua.script(R"(
                    function f (a)
                            return a + 5
                    end
            )");

    // Get and immediately call
    int x = lua["f"](30);
    // x == 35
    EXPECT_EQ(35, x);

    // Store it into a variable first, then call
    sol::function f = lua["f"];
    int y = f(20);
    // y == 25
    EXPECT_EQ(25, y);
}

TEST(Sol2, FunctionProtectedd)
{
    sol::state lua;

    lua.script(R"(
                    function handler (message)
                            return "Handled this message: " .. message
                    end

                    function f (a)
                            if a < 0 then
                                    error("negative number detected")
                            end
                            return a + 5
                    end
            )");

    sol::protected_function f = lua["f"];
    f.error_handler = lua["handler"];

    sol::protected_function_result result = f(-500);
    EXPECT_FALSE(result.valid());
    if (result.valid()) {
        // Call succeeded
        int x = result;
    } else {
        // Call failed
        sol::error err = result;
        std::string what = err.what();
        // 'what' Should read
        // "Handled this message: negative number detected"
    }
}

TEST(Sol2, FunctionMultipleReturn)
{
    sol::state lua;

    lua.script("function f (a, b, c) return a, b, c end");

    std::tuple<int, int, int> result;
    result = lua["f"](1, 2, 3);
    // result == { 1, 2, 3 }
    int a, b;
    std::string c;
    sol::tie(a, b, c) = lua["f"](1, 2, "bark");
    // a == 1
    // b == 2
    // c == "bark"
    EXPECT_EQ(1, a);
    EXPECT_EQ(2, b);
    EXPECT_EQ("bark", c);
}

TEST(Sol2, FunctionMultipleReturnCpp)
{
    sol::state lua;

    lua["f"] = [](int a, int b, sol::object c) {
        // sol::object can be anything here: just pass it through
        return std::make_tuple(a, b, c);
    };

    std::tuple<int, int, int> result = lua["f"](1, 2, 3);
    // result == { 1, 2, 3 }

    std::tuple<int, int, std::string> result2;
    result2 = lua["f"](1, 2, "Arf?");
    // result2 == { 1, 2, "Arf?" }

    int a, b;
    std::string c;
    sol::tie(a, b, c) = lua["f"](1, 2, "meow");
    // a == 1
    // b == 2
    // c == "meow"
    EXPECT_EQ(1, a);
    EXPECT_EQ(2, b);
    EXPECT_EQ("meow", c);
}

TEST(Sol2, FunctionReturnToLua)
{
    sol::state lua;

    auto fancy_func = [](sol::object a, sol::object b, sol::this_state s) -> sol::object {
        sol::state_view lua(s);
        if (a.is<int>() && b.is<int>()) {
            return sol::make_object(lua, a.as<int>() + b.as<int>());
        } else if (a.is<bool>()) {
            bool do_triple = a.as<bool>();
            return sol::make_object(lua, b.as<double>() * (do_triple ? 3 : 1));
        }
        return sol::make_object(lua, sol::lua_nil);
    };

    lua["f"] = fancy_func;

    int result = lua["f"](1, 2);
    // result == 3
    EXPECT_EQ(3, result);
    double result2 = lua["f"](false, 2.5);
    // result2 == 2.5
    EXPECT_EQ(2.5, result2);

    // call in Lua, get result
    lua.script("result3 = f(true, 5.5)");
    double result3 = lua["result3"];
    // result3 == 16.5
    EXPECT_EQ(16.5, result3);
}

TEST(Sol2, CppLua)
{
    const std::string preludeScript = SCRIPTS_TEST_DIR + "/prelude_script.lua";
    const std::string playerScript = SCRIPTS_TEST_DIR + "/player_script.lua";

    struct player {
    public:
        int bullets;
        int speed;

        player()
            : player(3, 100)
        {
        }

        player(int ammo)
            : player(ammo, 100)
        {
        }

        player(int ammo, int hitpoints)
            : bullets(ammo)
            , hp(hitpoints)
        {
        }

        void boost()
        {
            speed += 10;
        }

        bool shoot()
        {
            if (bullets < 1)
                return false;
            --bullets;
            return true;
        }

        void set_hp(int value)
        {
            hp = value;
        }

        int get_hp() const
        {
            return hp;
        }

    private:
        int hp;
    };

    sol::state lua;

    lua.open_libraries(sol::lib::base);

    // note that you can set a
    // userdata before you register a usertype,
    // and it will still carry
    // the right metatable if you register it later

    // set a variable "p2" of type "player" with 0 ammo
    lua["p2"] = player(0);

    // make usertype metatable
    sol::usertype<player> player_type = lua.new_usertype<player>("player",
        // 3 constructors
        sol::constructors<player(), player(int), player(int, int)>());

    // typical member function that returns a variable
    player_type["shoot"] = &player::shoot;
    // typical member function
    player_type["boost"] = &player::boost;

    // gets or set the value using member variable syntax
    player_type["hp"] = sol::property(&player::get_hp, &player::set_hp);

    // read and write variable
    player_type["speed"] = &player::speed;
    // can only read from, not write to
    // .set(foo, bar) is the same as [foo] = bar;
    player_type.set("bullets", sol::readonly(&player::bullets));

    lua.script_file(preludeScript);
    lua.script_file(playerScript);
}

TEST(Sol2, ReadConfig)
{
    const std::string usertypeScript = SCRIPTS_TEST_DIR + "/usertype.lua";

    struct SomeData {
        int one = 0;
        float two = 0;
        bool three = false;
        std::string str;

        SomeData() {}
        SomeData(int one, float two, bool three, std::string str)
            : one(one)
            , two(two)
            , three(three)
            , str(str)
        {
        }
    };

    sol::state lua;
    { // bind type
        sol::usertype<SomeData> type = lua.new_usertype<SomeData>("natSomeData",
            sol::constructors<
                SomeData(),
                SomeData(int, float, bool, std::string)>());
        type["one"] = &SomeData::one;
        type["two"] = &SomeData::two;
        type["three"] = &SomeData::three;
        type["str"] = &SomeData::str;
    }

    lua.script_file(usertypeScript);
    SomeData data = lua.get<SomeData>("someData");
    EXPECT_EQ(1488, data.one);
    EXPECT_EQ("Hello World", data.str);

    SomeData data2 = lua.get<SomeData>("someData2");
    EXPECT_EQ(1, data2.one);
    EXPECT_EQ("LOL", data2.str);
}
