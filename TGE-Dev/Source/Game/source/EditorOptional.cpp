#include "EditorOptional.h"
#include "EditorRegistry.h"

bool EditorOptional::OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	std::string fieldPath = GetData()["Field"];
	nlohmann::json_pointer<std::string> ptr(fieldPath);

	const nlohmann::json& field = GetRootValue()->at(ptr);

	std::shared_ptr<ValueEditor> editor = aEditorRegistry.GetDefault();
	nlohmann::json optionData = {};

	for (auto& entry : GetData()["Options"])
	{
		if (field == entry["Value"])
		{
			auto& editorType = entry["Editor"];
			optionData = editorType["Data"];

			editor = aEditorRegistry.GetDefault();

			editor = aEditorRegistry.Get(editorType["Type"]);
			if (editor == nullptr)
			{
				editor = aEditorRegistry.GetDefault();
			}
			break;
		}
	}
	editor->Init(GetContainerPath(), GetKey(), GetDefaultValue(), GetComponentEditorData(), optionData, GetRootValue(), GetEditorEntity());
	return editor->OnEditValue(aOverride, aEditorRegistry);
}
