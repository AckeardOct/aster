#include "refl_json.h"

#include "common/logger.h"

namespace toJsonImpl {

static void toJsonRecursively(const rttr::instance& obj2, Json& js);
static bool writeVariant(const rttr::variant& var, Json& js);

static bool writeAtomicTypesToJson(const rttr::type& t, const rttr::variant& var, Json& js)
{
    if (t.is_arithmetic()) {
        if (t == rttr::type::get<bool>()) {
            js = var.to_bool();
        } else if (t == rttr::type::get<char>()) {
            js = var.to_uint8();
        } else if (t == rttr::type::get<int8_t>()) {
            js = var.to_int8();
        } else if (t == rttr::type::get<int16_t>()) {
            js = var.to_int16();
        } else if (t == rttr::type::get<int32_t>()) {
            js = var.to_int32();
        } else if (t == rttr::type::get<int64_t>()) {
            js = var.to_int64();
        } else if (t == rttr::type::get<uint8_t>()) {
            js = var.to_uint8();
        } else if (t == rttr::type::get<uint16_t>()) {
            js = var.to_uint16();
        } else if (t == rttr::type::get<uint32_t>()) {
            js = var.to_uint32();
        } else if (t == rttr::type::get<uint64_t>()) {
            js = var.to_uint64();
        } else if (t == rttr::type::get<float>()) {
            js = var.to_float();
        } else if (t == rttr::type::get<double>()) {
            js = var.to_double();
        }
        return true;
    } else if (t.is_enumeration()) {
        ASSERT_FAIL("NEED CHECK");
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

static void writeArray(const rttr::variant_sequential_view& view, Json& js)
{
    for (const auto& item : view) {
        if (item.is_sequential_container()) {
            Json jsVar;
            writeArray(item.create_sequential_view(), jsVar);
            js += jsVar;
        } else {
            rttr::variant wrapped_var = item.extract_wrapped_value();
            rttr::type value_type = wrapped_var.get_type();
            if (value_type.is_arithmetic() || value_type == rttr::type::get<std::string>() || value_type.is_enumeration()) {
                Json jsVar;
                writeAtomicTypesToJson(value_type, wrapped_var, jsVar);
                js += jsVar;
            } else // object
            {
                Json jsVar;
                toJsonRecursively(wrapped_var, jsVar);
                js += jsVar;
            }
        }
    }
}

static void writeAssociativeContainer(const rttr::variant_associative_view& view, Json& js)
{
    if (view.is_key_only_type()) {
        for (auto& item : view) {
            Json jsVar; // TODO: unused ????
            writeVariant(item.first, jsVar);
            js += jsVar;
        }
    } else {
        for (auto& item : view) {
            Json jsKey;
            writeVariant(item.first, jsKey);

            Json jsVar;
            writeVariant(item.second, jsVar);

            std::string key;
            jsKey.get_to(key);
            js[key] = jsVar;
        }
    }
}

static bool writeVariant(const rttr::variant& var, Json& js)
{
    auto value_type = var.get_type();
    auto wrapped_type = value_type.is_wrapper() ? value_type.get_wrapped_type() : value_type;
    bool is_wrapper = (wrapped_type != value_type);

    if (writeAtomicTypesToJson(is_wrapper ? wrapped_type : value_type,
            is_wrapper ? var.extract_wrapped_value() : var, js)) {
    } else if (var.is_sequential_container()) {
        writeArray(var.create_sequential_view(), js);
    } else if (var.is_associative_container()) {
        writeAssociativeContainer(var.create_associative_view(), js);
    } else {
        auto child_props = is_wrapper ? wrapped_type.get_properties() : value_type.get_properties();
        if (!child_props.empty()) {
            toJsonRecursively(var, js);
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

static void toJsonRecursively(const rttr::instance& obj2, Json& js)
{
    rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;

    auto prop_list = obj.get_derived_type().get_properties();
    for (auto prop : prop_list) {
        rttr::variant prop_value = prop.get_value(obj);
        if (!prop_value)
            continue; // cannot serialize, because we cannot retrieve the value

        const char* name = prop.get_name().data();
        if (!writeVariant(prop_value, js[name])) {
            ASSERT_FAIL("cannot serialize property: %s", name);
        }
    }
}

} // namespace toJsonImpl

Json toJson(rttr::instance obj)
{
    ASSERT_MSG(obj.is_valid(), "Obj must be registered in RttR system");

    Json js;
    toJsonImpl::toJsonRecursively(obj, js);
    return js;
}

namespace fromJsonImpl {

static bool readPrimitive(const Json& jsVal, rttr::instance& obj, rttr::property& prop)
{
    rttr::type propType = prop.get_type();
    if (propType.is_arithmetic()) {
        if (propType == rttr::type::get<bool>()) {
            bool val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<char>()) {
            char val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<int8_t>()) {
            int8_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<int16_t>()) {
            int16_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<int32_t>()) {
            int32_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<int64_t>()) {
            int64_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<uint8_t>()) {
            uint8_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<uint16_t>()) {
            uint16_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<uint32_t>()) {
            uint32_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<uint64_t>()) {
            uint64_t val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<float>()) {
            float val = jsVal.get_to(val);
            prop.set_value(obj, val);
        } else if (propType == rttr::type::get<double>()) {
            double val = jsVal.get_to(val);
            prop.set_value(obj, val);
        }
        return true;
    } else if (propType.is_enumeration()) {
        ASSERT_FAIL("NEED CHECK");
    } else if (propType == rttr::type::get<std::string>()) {
        std::string val = jsVal.get_to(val);
        prop.set_value(obj, val);
        return true;
    }
    return false;
}

static bool readArray(const Json& js, rttr::instance& obj, rttr::property& prop)
{
    const auto propList = prop.get_type().get_properties();

    return true;
}

static bool fromJsonRecur(const Json& js, rttr::instance obj2)
{
    rttr::instance obj = obj2.get_type().get_raw_type().is_wrapper() ? obj2.get_wrapped_instance() : obj2;
    const auto propList = obj.get_derived_type().get_properties();

    for (auto prop : propList) {
        const char* propName = prop.get_name().data();
        const rttr::type propType = prop.get_type();

        Json jsVal = js[propName];
        if (propType.is_array()) {
            ASSERT(jsVal.is_array());
            readArray(jsVal, obj, prop);
        } else if (propType.is_class()) {
            ASSERT(jsVal.is_object());
            // object
        } else {
            ASSERT(jsVal.is_primitive());
            readPrimitive(jsVal, obj, prop);
        }
    }
    return true;
}

} // namespace fromJsonImpl

bool fromJson(const Json& js, rttr::instance obj)
{
    ASSERT_MSG(!js.is_null(), "Json is null");
    fromJsonImpl::fromJsonRecur(js, obj);
    return true;
}
