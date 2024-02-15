#pragma once
#include <nlohmann/json.hpp>

class ValueEditor
{
public:
	ValueEditor() = default;
	~ValueEditor() = default;

	virtual bool OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData);
};