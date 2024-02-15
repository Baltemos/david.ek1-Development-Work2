#include "EditorInputFloat4.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorInputFloat4::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData)
{
	nlohmann::json current = JsonMerge(aDefault, aOverride, true);
	float values[4];
	for (size_t index = 0; index < 4; index++)
	{
		if (current[index].is_null())
		{
			values[index] = NAN;
		}
		else
		{
			values[index] = current[index];
		}
	}

	if (ImGui::InputFloat4(aKey.c_str(), values, aEditorData["Format"].get<std::string>().c_str()))
	{
		aOverride = nlohmann::json(nlohmann::detail::value_t::array);
		aOverride += values[0];
		aOverride += values[1];
		aOverride += values[2];
		aOverride += values[3];
		return true;
	}
	else return false;
}
