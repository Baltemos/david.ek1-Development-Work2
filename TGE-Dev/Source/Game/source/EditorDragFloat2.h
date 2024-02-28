#pragma once
#include "ValueEditor.h"

class EditorDragFloat2 : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};