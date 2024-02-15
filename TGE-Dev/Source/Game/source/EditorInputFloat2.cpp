#include "EditorInputFloat2.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorInputFloat2::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData)
{
	nlohmann::json current = JsonMerge(aDefault, aOverride, true);
	float values[2];
	for (size_t index = 0; index < 2; index++)
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

	if (ImGui::InputFloat2(aKey.c_str(), values, aEditorData["Format"].get<std::string>().c_str()))
	{
		aOverride = nlohmann::json(nlohmann::detail::value_t::array);
		aOverride += values[0];
		aOverride += values[1];
		return true;
	}
	else return false;
}
