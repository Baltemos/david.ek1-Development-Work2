#include "EditorEntity.h"
#include "Entity.h"
#include <imgui/imgui.h>
#include <tge/settings/settings.h>
#include <fstream>
#include "GameWorld.h"
#include "JsonUtil.h"
#include <sstream>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "FileDialog.h"
#include <tge/util/StringCast.h>
#include <io.h>
#include <ShlObj_core.h>
#include <regex>

EditorEntity::EditorEntity()
{
}

void EditorEntity::SetTemplate(const EntityTemplate2& aTemplate)
{
	myTemplate = aTemplate;

	UpdateVisuals();
}

const EntityTemplate2& EditorEntity::GetTemplate() const
{
	return myTemplate;
}

bool EditorEntity::IsPrefabInstance() const
{
	return myTemplate.IsPrefabInstance();
}

void EditorEntity::MakePrefab()
{
	ImGui::OpenPopup("PrefabNameInputPopup");
}

bool EditorEntity::CheckSavePrefab(const std::string& aPath)
{
	TCHAR wpath[MAX_PATH + 1];
	lstrcpy(wpath, string_cast<std::wstring>(aPath).c_str());

	if (PathCleanupSpec(NULL, wpath) != NULL)
	{
		MessageBox(GetActiveWindow(), TEXT("Could not create prefab as the name has one or more illegal characters."), TEXT("Invalid Name"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}


	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath("Entities/") + aPath;

	if (_access_s((resolvedPath).c_str(), 0) == 0)
	{
		return MessageBox(GetActiveWindow(), TEXT("Do you want to write over it?"), TEXT("Prefab of that Name already exists"), MB_OKCANCEL | MB_ICONWARNING | MB_APPLMODAL) == IDOK;
	}
	else return true;
}

void EditorEntity::OverwritePrefab()
{
	myTemplate.OverwritePrefab();
}

void EditorEntity::MakeIndependent()
{
	myTemplate = myTemplate.GetIndependent();
}

void EditorEntity::Reload()
{
	myTemplate.Reload();
	UpdateVisuals();
}

void EditorEntity::AddComponent(const std::string& aType, const nlohmann::json& aOverrides)
{
	assert(myTemplate.IsPrefabInstance() == false && L"Cannot add Components to a Prefab Instance");

	ComponentTemplate tComp;
	tComp.Type = aType;
	tComp.Overrides = aOverrides;
	myTemplate.TryAddComponent(tComp);

	UpdateVisuals();
}

bool EditorEntity::TrySetPosition(const cu::Vector3<float>& aPosition)
{
	for (auto& component : myTemplate.GetRawComponents())
	{
		if (component.Type == "Transform")
		{
			component.Overrides["Translation"] = { aPosition.x, aPosition.y, aPosition.z };
			UpdateComponentVisuals(component, myComponentRegistry->GetEntry(component.Type));
			return true;
		}
	}

	return false;
}

void EditorEntity::ImGui_ShowObjectEditor(bool* aOpen)
{
	if (ImGui::Begin("Entity Editor", aOpen))
	{
		// Entity Header
		if (ImGui::Button("Delete") || ImGui::IsKeyDown(ImGuiKey_Delete))
		{
			GetEntity()->Destroy();
		}
		ImGui::SameLine();

		if (IsPrefabInstance())
		{
			if (ImGui::Button("Make Independent"))
			{
				MakeIndependent();
			}
			ImGui::SameLine();
			if (ImGui::Button("Update Prefab"))
			{
				OverwritePrefab();
			}
		}
		else
		{
			if (ImGui::Button("Make Prefab"))
			{
				MakePrefab();
			}
		}

		std::string name = myTemplate.GetName();
		if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
		{
			myTemplate.SetName(name);
		}
		ImGui::Separator();

		//Entity Components
		size_t ind = 0;
		std::vector<ComponentTemplate> components;
		while (ind < (components = myTemplate.GetComponents()).size())
		{
			ImGui::PushID(static_cast<int>(ind));
			ImGui::Separator();
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry(components[ind].Type);
			if (IsPrefabInstance() == false)
			{
				if (ImGui::Button("Remove"))
				{
					myTemplate.TryEraseComponent(ind);
					ImGui::PopID();
					continue;
				}
				ImGui::SameLine();
			}

			if (DisplayJson("Entity", components[ind].Type, JsonMerge(entry.DefaultData, components[ind].Defaults, true), components[ind].Overrides, entry.EditorInfo))
			{
				myTemplate.GetRawComponents()[ind].Overrides = components[ind].Overrides;
				UpdateComponentVisuals(components[ind], entry);
			}
			ImGui::PopID();
			++ind;
		}
		ImGui::Separator();

		if (IsPrefabInstance() == false && ImGui::Button("Add Component"))
		{
			POINT mousePos = myInputHandler->GetMouseScreenPosition();
			ImGui::SetNextWindowPos({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
			ImGui::OpenPopup("ComponentSelectionPopup");
		}
	}

	//Popups
	ImGui_ShowComponentPopup();
	ImGui_ShowPrefabNameInputPopup();

	ImGui::End();
}

void EditorEntity::Read(const nlohmann::json& /*someData*/)
{
	myEditorRegistry = &GameWorld::GetInstance()->GetEditorRegistry();
	myComponentRegistry = &GameWorld::GetInstance()->GetComponentRegistry();
	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();

	myTransform = GetEntity()->GetComponent<Transform>();
	myRenderer = GetEntity()->GetComponent<SpriteRenderer2D>();
}

void EditorEntity::Update(float)
{
	SetVisible(myTemplate.ContainsComponent("SpriteRenderer2D") && myTemplate.ContainsComponent("Transform"));
}

void EditorEntity::ImGui_ShowComponentPopup()
{
	if (ImGui::BeginPopup("ComponentSelectionPopup"))
	{
		for (const std::string& component : myComponentRegistry->GetRegistered())
		{
			if (ImGui::Button(component.c_str()))
			{
				AddComponent(component);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void EditorEntity::ImGui_ShowPrefabNameInputPopup()
{
	bool open = true;
	if (ImGui::BeginPopupModal("PrefabNameInputPopup", &open, ImGuiWindowFlags_NoResize))
	{
		static const std::regex fileRx("\\w+");
		std::string input;
		if (ImGui::InputText("Prefab Filename", &input, ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue,
			[](ImGuiInputTextCallbackData* data) {
				char chars[2];
				chars[1] = '\0';
				chars[0] = static_cast<char>(data->EventChar);
				return static_cast<int>(std::regex_match(chars, fileRx) == false);
			}))
		{
			std::string fileName = input + ".json";
			std::string path = "Entities/" + fileName;

			if (CheckSavePrefab(fileName))
			{
				std::string resolvedPath = Tga::Settings::ResolveGameAssetPath("") + path;

				{
					std::ofstream file(resolvedPath);
					file << myTemplate.Save();
				}

				myTemplate = EntityTemplate2(path);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void EditorEntity::SetVisible(bool aVisible)
{
	std::shared_ptr<SpriteRenderer2D> renderer = myRenderer.lock();
	Tga::Sprite2DInstanceData instance = renderer->GetInstance();
	instance.myIsHidden = !aVisible;
	renderer->SetInstance(instance);
}

bool EditorEntity::DisplayJson(const std::string& aValuePath, const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverrides, const nlohmann::json& aEditorInfo)
{
	std::string path = aValuePath + "/" + aKey;
	ImGui::PushID(path.c_str());
	bool changed = false;

	auto editorEntry = aEditorInfo.find(path);
	if (editorEntry != aEditorInfo.end())
	{
		std::string editorType = editorEntry.value()["Type"];
		ValueEditor* editor = myEditorRegistry->Get(editorType);
		if (editor == nullptr)
		{
			ImGui::Text((aKey + ": Error ValueEditor of Type '" + editorType + "' does not exist.").c_str());
		}
		else
		{
			if (editor->OnEditValue(aKey, aDefault, aOverrides, editorEntry.value()["Data"]))
			{
				changed = true;
			}
		}
	}
	else if (aDefault.is_object())
	{
		ImGui::Text(aKey.c_str());
		ImGui::Indent(20);
		for (auto& entry : aDefault.items())
		{
			ImGui::PushID((path + "/" + entry.key() + ":Head").c_str());

			if (aOverrides.contains(entry.key()))
			{
				if (ImGui::Button("*", { 14, 0 }))
				{
					aOverrides.erase(entry.key());
					changed = true;
				}
				else
				{
					ImGui::SameLine();

					if (DisplayJson(path, entry.key(), entry.value(), aOverrides[entry.key()], aEditorInfo))
					{
						changed = true;
					}
				}
			}
			else
			{
				ImGui::BeginDisabled(true);
				ImGui::Button("", { 14, 0 });
				ImGui::EndDisabled();
				ImGui::SameLine();
				nlohmann::json standIn(nlohmann::detail::value_t::null);
				if (DisplayJson(path, entry.key(), entry.value(), standIn, aEditorInfo))
				{
					aOverrides[entry.key()] = standIn;
					changed = true;
				}
			}
			ImGui::PopID();
		}
		ImGui::Indent(-20);
	}
	else if (aDefault.is_array())
	{
		ImGui::Text(aKey.c_str());
		ImGui::BeginGroup();

		for (size_t index = 0; index < aDefault.size(); index++)
		{
			if (aOverrides.is_null())
			{
				nlohmann::json standIn = aDefault;
				nlohmann::json prev = aOverrides;
				if (DisplayJson(path + "[" + std::to_string(index) + "]", aKey + "[" + std::to_string(index) + "]", aDefault[index], standIn[index], aEditorInfo))
				{
					changed = true;
					aOverrides = standIn;
				}
			}
			else
			{
				if (aOverrides.size() != aDefault.size()) throw std::exception("Array override did not match size of default array.");
				nlohmann::json prev = aOverrides;
				if (DisplayJson(path + "[" + std::to_string(index) + "]", aKey + "[" + std::to_string(index) + "]", aDefault[index], aOverrides[index], aEditorInfo))
				{
					changed = true;
				}
			}
		}
		ImGui::EndGroup();
	}
	else
	{
		if (myEditorRegistry->GetDefault()->OnEditValue(aKey, aDefault, aOverrides, {}))
		{
			changed = true;
		}
	}

	ImGui::PopID();
	return changed;
}

void EditorEntity::UpdateVisuals()
{
	for (ComponentTemplate& component : myTemplate.GetComponents())
	{
		if (component.Type == "Transform")
		{
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry("Transform");
			myTransform.lock()->Read(JsonMerge(entry.DefaultData, JsonMerge(component.Defaults, component.Overrides, true), true));
			break;
		}
	}
	for (ComponentTemplate& component : myTemplate.GetComponents())
	{
		if (component.Type == "SpriteRenderer2D")
		{
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry("SpriteRenderer2D");
			myRenderer.lock()->Read(JsonMerge(entry.DefaultData, JsonMerge(component.Defaults, component.Overrides, true), true));
			break;
		}
	}
}

void EditorEntity::UpdateComponentVisuals(const ComponentTemplate& aComponent, const ComponentRegistry::Entry& aEntry)
{
	if (aComponent.Type == "SpriteRenderer2D")
	{
		myRenderer.lock()->Read(JsonMerge(aEntry.DefaultData, JsonMerge(aComponent.Defaults, aComponent.Overrides, true), true));
	}
	else if (aComponent.Type == "Transform")
	{
		myTransform.lock()->Read(JsonMerge(aEntry.DefaultData, JsonMerge(aComponent.Defaults, aComponent.Overrides, true), true));
	}
}
