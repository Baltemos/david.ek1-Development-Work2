#include "EditorEnum.h"
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <sstream>

bool EditorEnum::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
	nlohmann::json current = GetValue(GetDefaultValue(), aOverride);

	std::stringstream ss;
	ss << current;
	std::string selectedKey = "Unknown Enum: " + ss.str();

	auto& enums = GetData()["Enums"];

	std::map<std::string, nlohmann::json> myValueMap;
	std::map<nlohmann::json, std::string> myKeyMap;

	for (auto& entry : enums.items())
	{
		myValueMap.emplace(entry.key(), entry.value());
		myKeyMap.emplace(entry.value(), entry.key());

		if (current == entry.value())
		{
			selectedKey = entry.key();
		}
	}

	bool changed = false;
	if (ImGui::BeginCombo(GetData()["Name"].get<std::string>().c_str(), selectedKey.c_str()))
	{
		for (auto& entry : myKeyMap)
		{
			bool selected = selectedKey == entry.second;
			if (ImGui::Selectable(entry.second.c_str(), &selected))
			{
				changed = true;
				selectedKey = entry.second;
				aOverride = entry.first;
			}

		}

		ImGui::EndCombo();
	}

	return changed;
}
