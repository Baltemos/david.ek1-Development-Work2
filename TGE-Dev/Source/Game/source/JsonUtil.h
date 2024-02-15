#pragma once
#include <nlohmann/json.hpp>

nlohmann::json JsonMerge(const nlohmann::json& aFirst, const nlohmann::json& aSecond, bool aIgnoreNull);
