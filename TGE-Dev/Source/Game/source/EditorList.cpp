#include "EditorList.h"
#include <imgui/imgui.h>

bool EditorList::OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	bool changed = false;

	if (ImGui::TreeNodeEx(GetKey().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		const nlohmann::json current = GetValue(GetDefaultValue(), aOverride);
		const nlohmann::json defaultItem = GetData()["DefaultItem"];

		// Array Edit
		for (int ind = 0; ind < current.size(); ind++)
		{
			ImGui::PushID(ind);

			nlohmann::json overrideOutput = {};

			nlohmann::json defaultValue = defaultItem;
			if (GetDefaultValue().size() > ind)
			{
				defaultValue = GetDefaultValue()[ind];
			}
			std::shared_ptr<ValueEditor> editor = GetEditor(ValueEditor::ArrayItemName, defaultValue, aEditorRegistry);

			if (aOverride.is_array() && aOverride.size() > ind)
			{
				overrideOutput = aOverride[ind];
			}

			if (editor->OnEditValue(overrideOutput, aEditorRegistry))
			{
				changed = true;
				if (aOverride.is_null())
				{
					aOverride = nlohmann::json::array();
					for (int i = 0; i < current.size(); i++)
					{
						aOverride += nlohmann::json(nlohmann::detail::value_t::null);
					}
				}
				aOverride[ind] = overrideOutput;
			}

			ImGui::PopID();
		}
		// ----------

		if (current.size() > 0)
		{
			if (ImGui::Button("Remove Item"))
			{
				changed = true;
				if (aOverride.is_null())
				{
					aOverride = current;
				}
				aOverride.erase(aOverride.size() - 1);
			}
			ImGui::SameLine();
		}
		if (ImGui::Button("Add Item"))
		{
			changed = true;
			while (aOverride.size() < current.size())
			{
				aOverride += nlohmann::json(nlohmann::detail::value_t::null);
			}
			aOverride += defaultItem;
		}

		ImGui::TreePop();
	}

	return changed;
}
