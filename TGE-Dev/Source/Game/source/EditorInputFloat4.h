#pragma once
#include "ValueEditor.h"

class EditorInputFloat4 : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};