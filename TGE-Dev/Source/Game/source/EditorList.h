#pragma once
#include "ValueEditor.h"

class EditorList : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};