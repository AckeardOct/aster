#include "refl_json.h"

#include "common/logger.h"

static void to_json_recursively(const rttr::instance& obj2, Json& js);

static bool write_atomic_types_to_json(const rttr::type& t, const rttr::variant& var, Json& js)
{
    if (t.is_arithmetic()) {
        if (t == rttr::type::get<bool>())
            js = var.to_bool();
        else if (t == rttr::type::get<char>())
            js = var.to_string(); // BUG ?????
        else if (t == rttr::type::get<int8_t>())
            js = var.to_int8();
        else if (t == rttr::type::get<int16_t>())
            js = var.to_int16();
        else if (t == rttr::type::get<int32_t>())
            js = var.to_int32();
        else if (t == rttr::type::get<int64_t>())
            js = var.to_int64();
        else if (t == rttr::type::get<uint8_t>())
            js = var.to_uint8();
        else if (t == rttr::type::get<uint16_t>())
            js = var.to_uint16();
        else if (t == rttr::type::get<uint32_t>())
            js = var.to_uint32();
        else if (t == rttr::type::get<uint64_t>())
            js = var.to_uint64();
        else if (t == rttr::type::get<float>())
            js = var.to_float();
        else if (t == rttr::type::get<double>())
            js = var.to_double();

        return true;
    } else if (t.is_enumeration()) {
        bool ok = false;
        auto result = var.to_string(&ok);
        if (ok) {
            js = result;
        } else {
            ok = false;
            auto value = var.to_uint64(&ok);
            if (ok)
                js = value;
            else
                js = nullptr;
        }

        return true;
    } else if (t == rttr::type::get<std::string>()) {
        js = var.to_string();
        return true;
    }

    return false;
}

static bool write_variant(const rttr::variant& var, Json& js)
{
    auto value_type = var.get_type();
    auto wrapped_type = value_type.is_wrapper() ? value_type.get_wrapped_type() : value_type;
    bool is_wrapper = wrapped_type != value_type;

    if (write_atomic_types_to_json(is_wrapper ? wrapped_type : value_type,
            is_wrapper ? var.extract_wrapped_value() : var, js)) {
    } /* else if (var.is_sequential_container()) {
        write_array(var.create_sequential_view(), js);
    } else if (var.is_associative_container()) {
        write_associative_container(var.create_associative_view(), js);
    } */
    else {
        auto child_props = is_wrapper ? wrapped_type.get_properties() : value_type.get_properties();
        if (!child_props.empty()) {
            to_json_recursively(var, js);
        } else {
            bool ok = false;
            auto text = var.to_string(&ok);
            if (!ok) {
                js = text;
                return false;
            }

            js = text;
        }
    }

    return true;
}

static void to_json_recursively(const rttr::instance& obj2, Json& js)
{
    rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

    auto prop_list = obj.get_derived_type().get_properties();
    for (auto prop : prop_list) {
        rttr::variant prop_value = prop.get_value(obj);
        if (!prop_value)
            continue; // cannot serialize, because we cannot retrieve the value

        const char* name = prop.get_name().data();
        if (!write_variant(prop_value, js[name])) {
            ASSERT_FAIL("cannot serialize property: %s", name);
        }
    }
}

Json toJson(rttr::instance obj)
{
    if (!obj.is_valid())
        return nullptr;

    Json js;
    to_json_recursively(obj, js);
    return js;
}
