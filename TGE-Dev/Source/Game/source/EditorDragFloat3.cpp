#include "EditorDragFloat3.h"
#include <imgui/imgui.h>
#include "JsonUtil.h"

bool EditorDragFloat3::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData)
{
	nlohmann::json result = JsonMerge(aDefault, aOverride, true);
	float values[3];
	values[0] = result[0];
	values[1] = result[1];
	values[2] = result[2];

	float minValue = 0;
	float maxValue = 0;

	auto minEntry = aEditorData.find("Min");
	if (minEntry != aEditorData.end())
	{
		minValue = minEntry.value();
	}
	auto maxEntry = aEditorData.find("Max");
	if (maxEntry != aEditorData.end())
	{
		maxValue = maxEntry.value();
	}

	if (ImGui::DragFloat3(aKey.c_str(), values, aEditorData["Speed"], minValue, maxValue, aEditorData["Format"].get<std::string>().c_str()))
	{
		aOverride[0] = values[0];
		aOverride[1] = values[1];
		aOverride[2] = values[2];
		return true;
	}
	else return false;
}
