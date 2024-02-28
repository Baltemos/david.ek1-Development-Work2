#pragma once
#include "ValueEditor.h"

class EditorOptional : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};