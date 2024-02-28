#pragma once
#include "ValueEditor.h"

class EditorInputFilePath : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};