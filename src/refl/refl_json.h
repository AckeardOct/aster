#pragma once

#include <rttr/registration>
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

Json toJson(rttr::instance obj);
