#include "ValueEditor.h"
#include <imgui/imgui.h>
#include "JsonUtil.h"

bool ValueEditor::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& /*aEditorData*/)
{
	if (aDefault.is_number_float())
	{
		float value = JsonMerge(aDefault, aOverride, true);

		if (ImGui::InputFloat(aKey.c_str(), &value, 1.f, 100.f))
		{
			aOverride = value;
			return true;
		}
	}
	else if (aDefault.is_number_integer())
	{
		int value = JsonMerge(aDefault, aOverride, true);
		if (ImGui::InputInt(aKey.c_str(), &value))
		{
			aOverride = value;
			return true;
		}
	}
	else if (aDefault.is_boolean())
	{
		bool value = JsonMerge(aDefault, aOverride, true);
		if (ImGui::Checkbox(aKey.c_str(), &value))
		{
			aOverride = value;
			return true;
		}
	}
	else if (aDefault.is_string())
	{
		char buffer[256];
		strncpy_s(buffer, JsonMerge(aDefault, aOverride, true).get<std::string>().c_str(), sizeof(buffer));
		if (ImGui::InputText(aKey.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
		{
			aOverride = std::string(buffer);
			return true;
		}
	}
	else if (aDefault.is_null())
	{
		ImGui::Text((aKey + " Value is Null, Cannot derive type.").c_str());
	}
	else
	{
		ImGui::Text((aKey + " Could not determine type of value.").c_str());
	}

	return false;
}
