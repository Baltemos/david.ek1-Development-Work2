#include "EditorInputFloat3.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorInputFloat3::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
	nlohmann::json current = GetValue(GetDefaultValue(), aOverride);
	float values[3];
	for (size_t index = 0; index < 3; index++)
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

	if (ImGui::InputFloat3(GetKey().c_str(), values, GetData()["Format"].get<std::string>().c_str()))
	{
		aOverride = nlohmann::json(nlohmann::detail::value_t::array);
		aOverride += values[0];
		aOverride += values[1];
		aOverride += values[2];
		return true;
	}
	else return false;
}
