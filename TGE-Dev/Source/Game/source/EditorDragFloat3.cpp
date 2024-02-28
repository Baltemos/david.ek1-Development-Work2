#include "EditorDragFloat3.h"
#include <imgui/imgui.h>
#include "JsonUtil.h"

bool EditorDragFloat3::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
	nlohmann::json result = GetValue(GetDefaultValue(), aOverride);
	float values[3];
	values[0] = result[0];
	values[1] = result[1];
	values[2] = result[2];

	float minValue = 0;
	float maxValue = 0;

	auto minEntry = GetData().find("Min");
	if (minEntry != GetData().end())
	{
		minValue = minEntry.value();
	}
	auto maxEntry = GetData().find("Max");
	if (maxEntry != GetData().end())
	{
		maxValue = maxEntry.value();
	}

	if (ImGui::DragFloat3(GetKey().c_str(), values, GetData()["Speed"], minValue, maxValue, GetData()["Format"].get<std::string>().c_str()))
	{
		aOverride[0] = values[0];
		aOverride[1] = values[1];
		aOverride[2] = values[2];
		return true;
	}
	else return false;
}
