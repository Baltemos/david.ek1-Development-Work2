#pragma once
#include "ValueEditor.h"

class EditorColorPicker : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};