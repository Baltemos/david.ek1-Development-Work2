#pragma once
#include "ValueEditor.h"

class EditorInputFloat2 : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};