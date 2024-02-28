#pragma once
#include "ValueEditor.h"

class EditorRectBounds : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};