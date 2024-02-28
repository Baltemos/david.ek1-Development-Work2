#pragma once
#include "ValueEditor.h"

class EditorColorPickerButton : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};