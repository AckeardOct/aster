#include "refl_json.h"

#include "common/logger.h"

template <class T>
T __getPrimitive(Json& json, const rttr::variant& var)
{
    ASSERT_FAIL("UNKNOWN TYPE");
    return T();
}

template <>
int __getPrimitive<int>(Json& json, const rttr::variant& var)
{
    bool success = false;
    int ret = var.to_int(&success);
    ASSERT(success);
    json = ret;
    return ret;
}

Json toJson(rttr::instance obj)
{
    ASSERT(obj.is_valid());

    Json ret;
    const auto& props = obj.get_type().get_properties();
    for (const auto& prop : props) {
        const char* name = prop.get_name().data();
        if (prop.get_type().is_class()) {
            ret[name] = toJson(prop.get_value(obj));
        } else {
            const rttr::variant var = prop.get_value(obj);
            using T = decltype(prop.get_type().create());
            __getPrimitive<T>(ret[name], var);
        }
    }
    return ret;
}
