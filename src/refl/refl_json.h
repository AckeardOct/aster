#pragma once

#include <nlohmann/json.hpp>
#include <rttr/type>

using Json = nlohmann::json;

Json toJson(rttr::instance obj);
