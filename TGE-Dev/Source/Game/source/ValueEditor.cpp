#include "ValueEditor.h"
#include <imgui/imgui.h>
#include "JsonUtil.h"
#include "EditorRegistry.h"
#include <imgui/misc/cpp/imgui_stdlib.h>

void ValueEditor::Init(const std::string& aContainerPath, const std::string& aKey, const nlohmann::json& aDefault, const nlohmann::json& aComponentEditorData, const nlohmann::json& aEditorData, const nlohmann::json* aRootValue, std::shared_ptr<EditorEntity> aEditorEntity)
{
	myContainerPath = aContainerPath;
	myKey = aKey;
	myDefaultValue = aDefault;
	myComponentEditorData = aComponentEditorData;
	myData = aEditorData;
	myRootValue = aRootValue;
	myEditorEntity = aEditorEntity;
}

void ValueEditor::Init(const std::string& aContainerPath, const std::string& aKey, const nlohmann::json& aDefault, const nlohmann::json& aComponentEditorData, const nlohmann::json* aRootValue, std::shared_ptr<EditorEntity> aEditorEntity)
{
	nlohmann::json data;
	auto editorData = aComponentEditorData.find(aContainerPath + "/" + aKey);
	if (editorData != aComponentEditorData.end())
	{
		data = editorData.value()["Data"];
	}
	else
	{
		data = {};
	}
	Init(aContainerPath, aKey, aDefault, aComponentEditorData, data, aRootValue, aEditorEntity);
}

void ValueEditor::Init(const ValueEditor& aParent, const std::string& aKey, const nlohmann::json& aDefaultValue)
{
	Init(aParent.ValuePath(), aKey, aDefaultValue, aParent.GetComponentEditorData(), aParent.GetRootValue(), aParent.GetEditorEntity());
}

const std::string& ValueEditor::GetContainerPath() const
{
	return myContainerPath;
}

std::string ValueEditor::ValuePath() const
{
	return myContainerPath + "/" + myKey;
}

const std::string& ValueEditor::GetKey() const
{
	return myKey;
}

const nlohmann::json& ValueEditor::GetDefaultValue() const
{
	return myDefaultValue;
}

const nlohmann::json& ValueEditor::GetComponentEditorData() const
{
	return myComponentEditorData;
}

const nlohmann::json& ValueEditor::GetData() const
{
	return myData;
}

const nlohmann::json* ValueEditor::GetRootValue() const
{
	return myRootValue;
}

std::shared_ptr<EditorEntity> ValueEditor::GetEditorEntity() const
{
	return myEditorEntity.lock();
}

bool ValueEditor::OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	const nlohmann::json& defaultValue = GetDefaultValue();
	nlohmann::json value = GetValue(defaultValue, aOverride);

	bool changed = false;

	switch (defaultValue.type())
	{
	case nlohmann::detail::value_t::object:				changed = EditObject(value, aOverride, aEditorRegistry);	break;
	case nlohmann::detail::value_t::array:				changed = EditArray(value, aOverride, aEditorRegistry);		break;
	case nlohmann::detail::value_t::string:				changed = EditString(value, aOverride);						break;
	case nlohmann::detail::value_t::number_unsigned:
	case nlohmann::detail::value_t::number_integer:		changed = EditInteger(value, aOverride);					break;
	case nlohmann::detail::value_t::number_float:		changed = EditFloat(value, aOverride);						break;
	case nlohmann::detail::value_t::boolean:			changed = EditBool(value, aOverride);						break;
	default:											changed = HandleNull();										break;
	}

	return changed;
}

nlohmann::json ValueEditor::GetValue(const nlohmann::json& aDefault, nlohmann::json& aOverride) const
{
	if (aOverride.is_null())
	{
		return aDefault;
	}

	return JsonMerge(aDefault, aOverride);
}

std::shared_ptr<ValueEditor> ValueEditor::GetEditor(const std::string& aEditorKey, const nlohmann::json& aDefaultData, EditorRegistry& aEditorRegistry) const
{
	auto& editorData = GetComponentEditorData();
	auto editorType = editorData.find(ValuePath() + "/" + aEditorKey);

	std::shared_ptr<ValueEditor> editor = aEditorRegistry.GetDefault();

	if (editorType != editorData.end())
	{
		editor = aEditorRegistry.Get(editorType.value()["Type"]);
		if (editor == nullptr)
		{
			editor = aEditorRegistry.GetDefault();
		}
	}
	editor->Init(*this, aEditorKey, aDefaultData);
	return editor;
}

bool ValueEditor::EditObject(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	bool changed = false;

	if (ImGui::TreeNodeEx(GetKey().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& entry : aDefaultValue.items())
		{
			ImGui::PushID(entry.key().c_str());

			std::shared_ptr<ValueEditor> editor = GetEditor(entry.key(), GetDefaultValue()[entry.key()], aEditorRegistry);

			if (aOverride.contains(entry.key()))
			{
				if (ImGui::Button("*", { 14, 0 }))
				{
					aOverride.erase(entry.key());
					changed = true;
				}
			}
			else
			{
				ImGui::BeginDisabled();
				ImGui::Button("", { 14, 0 });
				ImGui::EndDisabled();
			}

			ImGui::SameLine();

			nlohmann::json overrideOutput = {};

			auto override = aOverride.find(entry.key());
			if (override != aOverride.end())
			{
				overrideOutput = override.value();
			}

			if (editor->OnEditValue(overrideOutput, aEditorRegistry))
			{
				aOverride[entry.key()] = overrideOutput;
				changed = true;
			}

			ImGui::PopID();
		}

		ImGui::TreePop();
	}


	return changed;
}

bool ValueEditor::EditArray(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	bool changed = false;

	if (ImGui::TreeNodeEx(GetKey().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int ind = 0; ind < aDefaultValue.size(); ind++)
		{
			ImGui::PushID(ind);

			nlohmann::json defaultVal = nlohmann::json(nlohmann::detail::value_t::null);
			if (GetDefaultValue().size() > ind)
			{
				defaultVal = GetDefaultValue()[ind];
			}
			std::shared_ptr<ValueEditor> editor = GetEditor(ArrayItemName, defaultVal, aEditorRegistry);

			nlohmann::json overrideOutput = {};

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
					for (int i = 0; i < aDefaultValue.size(); i++)
					{
						aOverride += nlohmann::json(nlohmann::detail::value_t::null);
					}
				}
				aOverride[ind] = overrideOutput;
			}

			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	return changed;
}

bool ValueEditor::EditString(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride)
{
	std::string value = aDefaultValue;
	if (ImGui::InputText(GetKey().c_str(), &value, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
	{
		aOverride = value;
		return true;
	}
	return false;
}

bool ValueEditor::EditInteger(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride)
{
	int value = aDefaultValue;
	if (ImGui::InputInt(GetKey().c_str(), &value))
	{
		aOverride = value;
		return true;
	}
	return false;
}

bool ValueEditor::EditFloat(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride)
{
	float value = aDefaultValue;
	if (ImGui::InputFloat(GetKey().c_str(), &value))
	{
		aOverride = value;
		return true;
	}
	return false;
}

bool ValueEditor::EditBool(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride)
{
	bool value = aDefaultValue;
	if (ImGui::Checkbox(GetKey().c_str(), &value))
	{
		aOverride = value;
		return true;
	}
	return false;
}

bool ValueEditor::HandleNull()
{
	ImGui::Text((GetKey() + ":").c_str());
	ImGui::SameLine();
	ImGui::Text("Default value is null, cannot read type.");
	return false;
}
