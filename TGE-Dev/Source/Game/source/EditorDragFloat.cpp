#include "EditorDragFloat.h"
#include <imgui/imgui.h>
#include "JsonUtil.h"

bool EditorDragFloat::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
	nlohmann::json result = GetValue(GetDefaultValue(), aOverride);
	float value = result;

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

	if (ImGui::DragFloat(GetKey().c_str(), &value, GetData()["Speed"], minValue, maxValue, GetData()["Format"].get<std::string>().c_str()))
	{
		aOverride = value;
		return true;
	}
	else return false;
}
