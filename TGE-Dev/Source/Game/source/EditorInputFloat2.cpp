#include "EditorInputFloat2.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorInputFloat2::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
	nlohmann::json current = GetValue(GetDefaultValue(), aOverride);
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

	if (ImGui::InputFloat2(GetKey().c_str(), values, GetData()["Format"].get<std::string>().c_str()))
	{
		aOverride = nlohmann::json(nlohmann::detail::value_t::array);
		aOverride += values[0];
		aOverride += values[1];
		return true;
	}
	else return false;
}
