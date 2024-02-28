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

//Event<std::shared_ptr<EditorEntity>> EditorEntity::OnClickEntity;

std::shared_ptr<EditorEntity> EditorEntity::CreateEditorObject()
{
	GameWorld* gameWorld = GameWorld::GetInstance();
	EntityTemplate tEditorObject;
	tEditorObject.AddComponent("Transform");
	tEditorObject.AddComponent("SpriteRenderer2D");
	tEditorObject.AddComponent("ClickableSprite");
	tEditorObject.AddComponent("EditorEntity");
	std::shared_ptr<Entity> editorObject = gameWorld->GetEntityManager().Add(tEditorObject, gameWorld->GetComponentRegistry());
	//editorObject->GetComponent<ClickableObject>()->GetOnClick().Subscribe([](ClickableObject* ptr) { EditorEntity::OnClickEntity(ptr->GetEntity()->GetComponent<EditorEntity>()); });
	std::shared_ptr<EditorEntity> editor = editorObject->GetComponent<EditorEntity>();
	return editor;
}

EditorEntity::EditorEntity()
{
	myIsShowing = true;
}

EditorEntity::~EditorEntity()
{
}

void EditorEntity::SetTemplate(const EntityTemplate& aTemplate)
{
	myEditorEntity = aTemplate;

	//Event<ClickableObject*>& myOnClickEvent = GetEntity()->GetComponent<ClickableObject>()->GetOnClick();

	//for (auto& child : myTemplate.GetRawChildren())
	//{
	//	std::shared_ptr<EditorEntity> entity = CreateEditorObject();
	//	entity->GetEntity()->GetComponent<Transform>()->SetParent(myTransform.lock().get(), false);
	//	entity->GetEntity()->GetComponent<ClickableObject>()->GetOnClick().Subscribe([=](ClickableObject* aClickable) { myOnClickEvent(aClickable); });
	//	entity->SetTemplate(child);
	//}

	UpdateVisuals();
}

const EntityTemplate& EditorEntity::GetTemplate() const
{
	return myEditorEntity;
}

bool EditorEntity::IsPrefabInstance() const
{
	return myEditorEntity.IsPrefabInstance();
}

//bool EditorEntity::CheckSavePrefab(const std::string& aPath)
//{
//	TCHAR wpath[MAX_PATH + 1];
//	lstrcpy(wpath, string_cast<std::wstring>(aPath).c_str());
//
//	if (PathCleanupSpec(NULL, wpath) != NULL)
//	{
//		MessageBox(GetActiveWindow(), TEXT("Could not create prefab as the name has one or more illegal characters."), TEXT("Invalid Name"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
//		return false;
//	}
//
//
//	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath("Entities/") + aPath;
//
//	if (_access_s((resolvedPath).c_str(), 0) == 0)
//	{
//		return MessageBox(GetActiveWindow(), TEXT("Do you want to write over it?"), TEXT("Prefab of that Name already exists"), MB_OKCANCEL | MB_ICONWARNING | MB_APPLMODAL) == IDOK;
//	}
//	else return true;
//}

//void EditorEntity::OverwritePrefab()
//{
//	myTemplate.OverwritePrefab();
//}
//
//void EditorEntity::MakeIndependent()
//{
//	myTemplate = myTemplate.GetIndependent();
//}

void EditorEntity::Reload()
{
	myEditorEntity.Reload();
	SetTemplate(myEditorEntity);
	UpdateVisuals();
}

void EditorEntity::AddComponent(const std::string& aType, const nlohmann::json& aOverrides)
{
	assert(myEditorEntity.IsPrefabInstance() == false && L"Cannot add Components to a Prefab Instance");

	ComponentTemplate tComp;
	tComp.Type = aType;
	tComp.Overrides = aOverrides;
	myEditorEntity.TryAddComponent(tComp);

	UpdateVisuals();
}

bool EditorEntity::TrySetPosition(const cu::Vector3<float>& aPosition)
{
	for (auto& component : myEditorEntity.GetRawComponents())
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

void EditorEntity::ImGui_ObjectEditor()
{
	//if (ImGui::Begin("Entity Editor", aOpen))
	//{
	//	// Entity Header
	//	if (ImGui::Button("Delete") || ImGui::IsKeyDown(ImGuiKey_Delete))
	//	{
	//		GetEntity()->Destroy();
	//	}
	//	ImGui::SameLine();

	//	if (IsPrefabInstance())
	//	{
	//		if (ImGui::Button("Make Independent"))
	//		{
	//			MakeIndependent();
	//		}
	//		ImGui::SameLine();
	//		if (ImGui::Button("Update Prefab"))
	//		{
	//			OverwritePrefab();
	//		}
	//	}
	//	else
	//	{
	//		if (ImGui::Button("Make Prefab"))
	//		{
	//			MakePrefab();
	//		}
	//	}

	//	ImGui::SameLine();
	//	ImGui::Checkbox("Show", &myIsShowing);

		std::string name = myEditorEntity.GetName();
		if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
		{
			myEditorEntity.SetName(name);
		}

		ImGui::Separator();

		//Entity Components
		size_t ind = 0;
		std::vector<ComponentTemplate> components;
		while (ind < (components = myEditorEntity.GetComponents()).size())
		{
			ImGui::PushID(static_cast<int>(ind));
			ImGui::Separator();
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry(components[ind].Type);
			if (IsPrefabInstance() == false)
			{
				if (ImGui::Button("Remove"))
				{
					myEditorEntity.TryEraseComponent(ind);
					ImGui::PopID();
					continue;
				}
				ImGui::SameLine();
			}

			if (DisplayJson("Entity", components[ind].Type, JsonMerge(entry.DefaultData, components[ind].Defaults), components[ind].Overrides, entry.EditorInfo))
			{
				myEditorEntity.GetRawComponents()[ind].Overrides = components[ind].Overrides;
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
	//}

	//Popups
	ImGui_ShowComponentPopup();
	//ImGui_ShowPrefabNameInputPopup();

	//ImGui::End();
}

void EditorEntity::SetShowing(bool aShowing)
{
	myIsShowing = aShowing;
}

bool EditorEntity::GetShowing() const
{
	return myIsShowing;
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
	SetVisible(myIsShowing && myEditorEntity.ContainsComponent("SpriteRenderer2D") && myEditorEntity.ContainsComponent("Transform"));
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
	std::shared_ptr<ValueEditor> editor;

	auto editorEntry = aEditorInfo.find(path);
	if (editorEntry != aEditorInfo.end())
	{
		editor = myEditorRegistry->Get(editorEntry.value()["Type"]);
	}
	else
	{
		editor = myEditorRegistry->GetDefault();
	}

	nlohmann::json rootValue = JsonMerge(aDefault, aOverrides);

	editor->Init(aValuePath, aKey, aDefault, aEditorInfo, &rootValue, std::dynamic_pointer_cast<EditorEntity>(GetSharedPtr()));

	return editor->OnEditValue(aOverrides, *myEditorRegistry);
}

void EditorEntity::UpdateVisuals()
{
	for (ComponentTemplate& component : myEditorEntity.GetComponents())
	{
		if (component.Type == "Transform")
		{
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry("Transform");
			myTransform.lock()->Read(JsonMerge(entry.DefaultData, JsonMerge(component.Defaults, component.Overrides, true)));
			break;
		}
	}
	for (ComponentTemplate& component : myEditorEntity.GetComponents())
	{
		if (component.Type == "SpriteRenderer2D")
		{
			const ComponentRegistry::Entry& entry = myComponentRegistry->GetEntry("SpriteRenderer2D");
			myRenderer.lock()->Read(JsonMerge(entry.DefaultData, JsonMerge(component.Defaults, component.Overrides, true)));
			break;
		}
	}
}

void EditorEntity::UpdateComponentVisuals(const ComponentTemplate& aComponent, const ComponentRegistry::Entry& aEntry)
{
	if (aComponent.Type == "SpriteRenderer2D")
	{
		myRenderer.lock()->Read(JsonMerge(aEntry.DefaultData, JsonMerge(aComponent.Defaults, aComponent.Overrides, true)));
	}
	else if (aComponent.Type == "Transform")
	{
		myTransform.lock()->Read(JsonMerge(aEntry.DefaultData, JsonMerge(aComponent.Defaults, aComponent.Overrides, true)));
	}
}
