#pragma once
#include "ValueEditor.h"

class EditorCircleBounds : public ValueEditor
{
public:
	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry) override;
};