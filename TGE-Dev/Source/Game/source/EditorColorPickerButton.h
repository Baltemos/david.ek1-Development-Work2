#pragma once
#include "ValueEditor.h"

class EditorColorPickerButton : public ValueEditor
{
public:
	virtual bool OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData) override;
};