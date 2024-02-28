#pragma once
#include "ValueEditor.h"

class EditorDragFloat3 : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};