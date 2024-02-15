#pragma once

struct ComponentTemplate
{
public:
	std::string Type;
	nlohmann::json Defaults;
	nlohmann::json Overrides;
};