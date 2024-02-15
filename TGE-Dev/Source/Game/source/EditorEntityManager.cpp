#include "EditorEntityManager.h"
#include "GameWorld.h"
#include "Camera.h"

EditorEntityManager::EditorEntityManager()
{
}

std::shared_ptr<EditorEntity> EditorEntityManager::AddEditorEntity()
{
	GameWorld* gameWorld = GameWorld::GetInstance();
	EntityTemplate2 tEditorObject;
	tEditorObject.AddComponent("Transform");
	tEditorObject.AddComponent("SpriteRenderer2D");
	tEditorObject.AddComponent("ClickableObject");
	tEditorObject.AddComponent("EditorEntity");
	std::shared_ptr<Entity> editorObject = gameWorld->GetEntityManager().Add(tEditorObject, gameWorld->GetComponentRegistry());
	std::shared_ptr<EditorEntity> editor = editorObject->GetComponent<EditorEntity>();
	std::shared_ptr<ClickableObject> editorBounds = editorObject->GetComponent<ClickableObject>();
	editorBounds->GetOnClick().Subscribe(this, &EditorEntityManager::onClick);
	myEditorEntities.push_back(editor);
	return editor;
}

void EditorEntityManager::ClearEditorEntities()
{
	for (std::shared_ptr<EditorEntity> ee : myEditorEntities)
	{
		if (!ee->IsDestroyed())
		{
			ee->GetEntity()->Destroy();
		}
	}
}

void EditorEntityManager::LoadBatch(const nlohmann::json& aBatch)
{
	for (auto& entity : aBatch)
	{
		EntityTemplate2 tEntity;
		tEntity.Load(entity);

		AddEditorEntity()->SetTemplate(tEntity);
	}
}

nlohmann::json EditorEntityManager::GetBatch() const
{
	nlohmann::json batch(nlohmann::detail::value_t::array);
	for (std::shared_ptr<EditorEntity> ee : myEditorEntities)
	{
		batch += ee->GetTemplate().Save();
	}
	return batch;
}

void EditorEntityManager::Read(const nlohmann::json&)
{
	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void EditorEntityManager::Update(float aTimeDelta)
{
	aTimeDelta;

	HandleClickedObjects();

	if (mySelectedObject)
	{
		if (mySelectedObject->IsDestroyed())
		{
			mySelectedObject = nullptr;
		}
		else
		{
			mySelectedObject->ImGui_ShowObjectEditor(nullptr);
		}
	}
	else
	{
		ImGui::Begin("Entity Editor");
		ImGui::Text("No Entity Selected.");
		ImGui::End();
	}

	auto it = myEditorEntities.begin();
	while (it < myEditorEntities.end())
	{
		if ((**it).IsDestroyed())
		{
			it = myEditorEntities.erase(it);
			continue;
		}
		++it;
	}

	if (mySelectedObject)
	{
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
		{
			myCopiedObject = mySelectedObject;
		}
	}
	if (myCopiedObject && myCopiedObject->IsDestroyed() == false)
	{
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_V))
		{
			std::shared_ptr<EditorEntity> entity = AddEditorEntity();
			entity->SetTemplate(myCopiedObject->GetTemplate());
			entity->TrySetPosition(Camera::GetActiveCamera()->GetEntity()->GetComponent<Transform>()->GetWorldPosition());
		}
	}

	ShowSidebar();
	ShowToolbox();
}

void EditorEntityManager::HandleClickedObjects()
{
	if (!myClickedObjects.empty())
	{
		auto it = myClickedObjects.begin();
		if (mySelectedObject)
		{
			ClickableObject* editorObject = mySelectedObject->GetEntity()->GetComponent<ClickableObject>().get();
			it = myClickedObjects.find(editorObject);
		}
		if (it != myClickedObjects.end())
		{
			it = std::next(it);
		}
		if (it == myClickedObjects.end())
		{
			it = myClickedObjects.begin();
		}
		Select((**it).GetEntity()->GetComponent<EditorEntity>());
		myClickedObjects.clear();
	}
}

void EditorEntityManager::Select(std::shared_ptr<EditorEntity> aEditorEntity)
{
	mySelectedObject = aEditorEntity;
	aEditorEntity->Reload();
}

void EditorEntityManager::ShowSidebar()
{
	ImGui::Begin("Sidebar");

	for (size_t index = 0; index < myEditorEntities.size(); index++)
	{
		ImGui::PushID(static_cast<int>(reinterpret_cast<intptr_t>(myEditorEntities[index].get())));

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.0f, 0.5f });
		if (myEditorEntities[index]->IsPrefabInstance())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(75, 75, 255, 255));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
		}

		if (ImGui::Button(myEditorEntities[index]->GetTemplate().GetName().c_str(), { 256, 24 }))
		{
			Select(myEditorEntities[index]);
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("EditorManagerObjectPayload", &index, sizeof(size_t));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorManagerObjectPayload"))
			{
				size_t droppedInd = *(const size_t*)payload->Data;
				std::shared_ptr<EditorEntity> obj = myEditorEntities[droppedInd];

				myEditorEntities.erase(myEditorEntities.begin() + droppedInd);
				myEditorEntities.insert(myEditorEntities.begin() + index, obj);
			}
			ImGui::EndDragDropTarget();

		}
		ImGui::PopID();
	}

	ImGui::Separator();
	if (ImGui::Button("Create Empty"))
	{
		Select(AddEditorEntity());
	}
	ImGui::End();
}

void EditorEntityManager::ShowToolbox()
{
	if (myInputHandler->IsKeyDown(static_cast<int>(cu::Keys::MOUSERBUTTON)))
	{
		POINT mousePos = myInputHandler->GetMouseScreenPosition();
		ImGui::SetNextWindowPos({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) });
		ImGui::OpenPopup("EditorWorldPopup");
	}

	if (ImGui::BeginPopup("EditorWorldPopup"))
	{
		POINT mousePosition = myInputHandler->GetMouseClientPosition();
		cu::Vector2<float> worldPos = Camera::GetActiveCamera()->ScreenToWorld({ static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0 }).ConvertToVector2XY();

		if (ImGui::Button("Create Object"))
		{
			std::shared_ptr<EditorEntity> editor = AddEditorEntity();

			editor->AddComponent("Transform", { {"Translation", { worldPos.x, worldPos.y, 0.f } } });
			editor->AddComponent("SpriteRenderer2D");

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Create Prefab Instance"))
		{
			ImGui::OpenPopup("CreatePrefabInstancePopup");
		}

		if (ImGui::BeginPopup("CreatePrefabInstancePopup"))
		{
			std::filesystem::directory_iterator directory(Tga::Settings::ResolveGameAssetPath("Entities"));

			for (const auto& fileEntry : directory)
			{
				std::filesystem::path path = fileEntry.path();
				std::string unresolvedPath = "Entities/" + path.filename().string();
				if (ImGui::Button(path.filename().string().c_str()))
				{
					std::ifstream file(path.c_str());
					EntityTemplate2 prefabInstance(unresolvedPath);
					std::shared_ptr<EditorEntity> editor = AddEditorEntity();

					for (ComponentTemplate& comp : prefabInstance.GetRawComponents())
					{
						if (comp.Type == "Transform")
						{
							if (comp.Overrides.is_null())
							{
								comp.Overrides = nlohmann::json(nlohmann::detail::value_t::object);
							}
							comp.Overrides["Translation"] = { worldPos.x, worldPos.y, 0.f };
						}
					}

					editor->SetTemplate(prefabInstance);
				}
			}
			ImGui::EndPopup();
		}

		ImGui::EndPopup();

	}
}

void EditorEntityManager::onClick(ClickableObject* aObject)
{
	myClickedObjects.emplace(aObject);
}
