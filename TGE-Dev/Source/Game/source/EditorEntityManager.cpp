#include "EditorEntityManager.h"
#include "GameWorld.h"
#include "Camera.h"

EditorEntityManager::EditorEntityManager()
{
	myTree.SetName("Root");
	//myClickedEventKey = EditorEntity::OnClickEntity.Subscribe(this, &EditorEntityManager::onClick);
	//myRootNode = EditorEntityTreeNode::Create();
	//myRootNode->OnClick.Subscribe(this, &EditorEntityManager::onClick);
	//mySelectedNode = nullptr;
	//myCopiedObject = nullptr;
}

EditorEntityManager::~EditorEntityManager()
{
	//EditorEntity::OnClickEntity.Unsubscribe(myClickedEventKey);
}

std::shared_ptr<EditorTree::Node> EditorEntityManager::AddEditorEntity()
{
	//std::shared_ptr<EditorEntity> editor = EditorEntity::CreateEditorObject();
	//std::shared_ptr<ClickableObject> editorBounds = editor->GetEntity()->GetComponent<ClickableObject>();
	//editorBounds->GetOnClick().Subscribe(this, &EditorEntityManager::onClick);
	//myEditorEntities.push_back(editor);
	//return editor;

	std::shared_ptr<EditorTree::Node> node = std::make_shared<EditorTree::Node>(false, EntityTemplate());

	myTree.AddChild(node);
	//node->OnClick.Subscribe(this, &EditorEntityManager::onClick);
	//myTreeNodes.push_back(node);
	//std::shared_ptr<EditorEntityTreeNode> node = myRootNode->AddEmptyChild();
	//node->SetTemplate({});
	//node->OnClick.Subscribe(this, &EditorEntityManager::onClick);
	return node;
}

void EditorEntityManager::ClearEditorEntities()
{
	//myTreeNodes.clear();
	myTree.clear();
	//myRootNode = EditorEntityTreeNode::Create();
	//myRootNode->OnClick.Subscribe(this, &EditorEntityManager::onClick);
	mySelectedEntity.reset();
	myCopiedObject.reset();
	//mySelectedNode.reset();
	//myCopiedObject.reset();
	//for (std::shared_ptr<EditorEntity> ee : myEditorEntities)
	//{
	//	if (!ee->IsDestroyed())
	//	{
	//		ee->GetEntity()->Destroy();
	//	}
	//}
}

void EditorEntityManager::LoadBatch(const nlohmann::json& aBatch)
{
	for (auto& entity : aBatch)
	{
		//try
		//{
		EntityTemplate tEntity;
		tEntity.Load(entity);

		AddEditorEntity()->SetTemplate(tEntity);
		//}
		//catch (std::exception e)
		//{
		//	std::cout << "[" << __FILE__ << "][Line: " << __LINE__ << "]: " << "Failed to load EntityTemplate. -> " << e.what() << std::endl;
		//}

	}
}

nlohmann::json EditorEntityManager::GetBatch() const
{
	nlohmann::json batch(nlohmann::detail::value_t::array);
	for (const std::shared_ptr<NodeCollection>& node : myTree)
	{
		batch += std::dynamic_pointer_cast<EditorTree::Node>(node)->GetTemplate().Save();
	}
	//for (const std::shared_ptr<EditorEntityTreeNode>& node : myRootNode->GetChildren())
	//{
	//	batch += node->GetTemplate().Save();
	//}
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

	if (mySelectedEntity.expired() == false)
	{
		mySelectedEntity.lock()->ImGui_ObjectEditor();
	}
	else
	{
		ImGui::Begin("Entity Editor");
		ImGui::Text("No Entity Selected.");
		ImGui::End();
	}



	//myRootNode->RemoveDestroyed();
	//if (mySelectedNode.expired() == false)
	//{
	//	mySelectedNode = nullptr;
	//}
	//if (myCopiedObject && myCopiedObject->IsDestroyed())
	//{
	//	myCopiedObject = nullptr;
	//}
	//auto it = myEditorEntities.begin();
	//while (it < myEditorEntities.end())
	//{
	//	if ((**it).IsDestroyed())
	//	{
	//		it = myEditorEntities.erase(it);
	//		continue;
	//	}
	//	++it;
	//}

	if (mySelectedEntity.expired() == false)
	{
		myEditorObjectDrawer.Draw(mySelectedEntity.lock()->GetEditorEntity());

		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::CONTROL)) && myInputHandler->IsKeyDown(static_cast<int>(cu::Keys::C)))
		{
			myCopiedObject = mySelectedEntity;
		}
	}
	if (myCopiedObject.expired() == false)
	{
		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::CONTROL)) && myInputHandler->IsKeyDown(static_cast<int>(cu::Keys::V)))
		{
			std::shared_ptr<EditorTree::Node> entity = AddEditorEntity();
			entity->SetTemplate(myCopiedObject.lock()->GetTemplate());
			entity->SetPosition(Camera::GetActiveCamera()->GetEntity()->GetComponent<Transform>()->GetWorldPosition());
		}
	}

	ShowSidebar();
	ShowToolbox();
}

void EditorEntityManager::HandleClickedObjects()
{
	auto rmIt = myClickedNodes.begin();
	while (rmIt != myClickedNodes.end())
	{
		if ((**rmIt).IsDestroyed())
		{
			rmIt = myClickedNodes.erase(rmIt);
		}
		else
		{
			++rmIt;
		}
	}

	std::sort(myClickedNodes.begin(), myClickedNodes.end(),
		[](const std::shared_ptr<EditorTree::Node>& aLhs, const std::shared_ptr<EditorTree::Node>& aRhs) {
			return aLhs.get() < aRhs.get();
		}
	);

	auto it = myClickedNodes.begin();
	while (it != myClickedNodes.end())
	{
		if (*it == mySelectedEntity.lock())
		{
			break;
		}
		++it;
	}

	if (!myClickedNodes.empty())
	{
		//auto it = myClickedNodes.begin();
		//if (mySelectedNode.expired() == false)
		//{
		//	std::shared_ptr<EditorEntityTreeNode> editorObject = mySelectedNode.lock();
		//	it = myClickedNodes.find(editorObject);
		//}
		if (it != myClickedNodes.end())
		{
			it = std::next(it);
		}
		if (it == myClickedNodes.end())
		{
			it = myClickedNodes.begin();
		}
		Select(*it);
		myClickedNodes.clear();
	}
}

void EditorEntityManager::Select(std::weak_ptr<EditorTree::Node> aNode)
{
	mySelectedEntity = aNode;
	//if (aNode != myRootNode)
	//{
	//	mySelectedNode = aNode;
	//	aNode->Reload();
	//}
}

void EditorEntityManager::ShowSidebar()
{
	ImGui::Begin("Sidebar");

	//if (myEditorEntities.empty() == false)
	//{
	//	ShowSidebarItems(myEditorEntities, myEditorEntities.begin(), nullptr);
	//}
	//myRootNode->ImGui_Display();

	//ImGui::BeginChild("EditorEntityTreeRoot", { 0, 0 }, false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	std::vector<std::shared_ptr<NodeCollection>> clickedNodes;
	if (myTree.BeginDisplay())
	{
		myTree.DisplayChildren(clickedNodes, true);
		myTree.EndDisplay();
	}
	for (std::shared_ptr<NodeCollection> node : clickedNodes)
	{
		myClickedNodes.push_back(std::dynamic_pointer_cast<EditorTree::Node>(node));
	}

	//for (std::shared_ptr<NodeCollection>& node : myTree)
	//{
	//	std::dynamic_pointer_cast<EditorTree::Node>(node)->
	//	node->ImGui_Display();
	//}

	//ImGui::EndChild();

	ImGui::Separator();
	if (ImGui::Button("Create Empty"))
	{
		Select(AddEditorEntity());
	}
	ImGui::End();

	//ImGui::Begin("Sidebar");

	//for (size_t index = 0; index < myEditorEntities.size(); index++)
	//{
	//	ImGui::PushID(static_cast<int>(reinterpret_cast<intptr_t>(myEditorEntities[index].get())));

	//	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.0f, 0.5f });
	//	if (myEditorEntities[index]->IsPrefabInstance())
	//	{
	//		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(75, 75, 255, 255));
	//	}
	//	else
	//	{
	//		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
	//	}

	//	std::string displayName = myEditorEntities[index]->GetTemplate().GetName();
	//	if (myEditorEntities[index]->GetShowing() == false)
	//	{
	//		displayName = "[Hidden] - " + displayName;
	//	}

	//	if (ImGui::Button(displayName.c_str(), { 256, 24 }))
	//	{
	//		Select(myEditorEntities[index]);
	//	}
	//	ImGui::PopStyleColor();
	//	ImGui::PopStyleVar();
	//	if (ImGui::BeginDragDropSource())
	//	{
	//		ImGui::SetDragDropPayload("EditorManagerObjectPayload", &index, sizeof(size_t));
	//		ImGui::EndDragDropSource();
	//	}
	//	if (ImGui::BeginDragDropTarget())
	//	{
	//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorManagerObjectPayload"))
	//		{
	//			size_t droppedInd = *(const size_t*)payload->Data;
	//			std::shared_ptr<EditorEntity> obj = myEditorEntities[droppedInd];

	//			myEditorEntities.erase(myEditorEntities.begin() + droppedInd);
	//			myEditorEntities.insert(myEditorEntities.begin() + index, obj);
	//		}
	//		ImGui::EndDragDropTarget();

	//	}
	//	ImGui::PopID();
	//}

	//ImGui::Separator();
	//if (ImGui::Button("Create Empty"))
	//{
	//	Select(AddEditorEntity());
	//}
	//ImGui::End();
}

//void EditorEntityManager::DragDropSource(const payload_t& aPayload)
//{
//	// Begin Drag drop source
//	if (ImGui::BeginDragDropSource())
//	{
//		ImGui::SetDragDropPayload("EditorManagerObjectPayload", &aPayload, sizeof(aPayload));
//		ImGui::EndDragDropSource();
//	}
//}
//
//bool EditorEntityManager::DragDropTarget(payload_t* aOutPayload)
//{
//	if (ImGui::BeginDragDropTarget())
//	{
//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorManagerObjectPayload"))
//		{
//			*aOutPayload = *(payload_t*)payload->Data;
//
//			ImGui::EndDragDropTarget();
//			return true;
//		}
//	}
//	return false;
//}
//
//std::vector<std::shared_ptr<EditorEntity>>::const_iterator EditorEntityManager::MoveEntity(std::shared_ptr<EditorEntity> aFirstParent,
//	std::vector<std::shared_ptr<EditorEntity>>::const_iterator aFirstWhere,
//	std::shared_ptr<EditorEntity> aSecondParent,
//	std::vector<std::shared_ptr<EditorEntity>>::const_iterator aTarget)
//{
//	auto p = aSecondParent.get();
//	while (p != nullptr)
//	{
//		p = p->GetParent();
//		if (p == aFirstWhere->get())
//		{
//			return aTarget;
//		}
//	}
//
//	std::shared_ptr<EditorEntity> payloadEntity = *aFirstWhere;
//	if (aFirstParent == aSecondParent)
//	{
//		if (aSecondParent)
//		{
//			size_t ind = std::distance(aSecondParent->GetChildren().cbegin(), aTarget);
//			aFirstParent->EraseChild(aFirstWhere);
//			if (ind >= aFirstParent->GetChildren().size())
//			{
//				return aFirstParent->InsertChild(aFirstParent->GetChildren().cend(), payloadEntity);
//			}
//			else
//			{
//				return aFirstParent->InsertChild(aFirstParent->GetChildren().cbegin() + ind, payloadEntity);
//			}
//		}
//		else
//		{
//			size_t ind = std::distance(myEditorEntities.cbegin(), aTarget);
//			myEditorEntities.erase(aFirstWhere);
//			return myEditorEntities.insert(myEditorEntities.begin() + ind, payloadEntity);
//		}
//	}
//	else
//	{
//		if (aFirstParent)
//		{
//			aFirstParent->EraseChild(aFirstWhere);
//		}
//		else
//		{
//			myEditorEntities.erase(aFirstWhere);
//		}
//		if (aSecondParent)
//		{
//			return aSecondParent->InsertChild(aTarget, payloadEntity);
//		}
//		else
//		{
//			return myEditorEntities.insert(aTarget, payloadEntity);
//		}
//	}
//}

//void EditorEntityManager::ShowSidebarItems(const std::vector<std::shared_ptr<EditorEntity>>& aCollection, std::vector<std::shared_ptr<EditorEntity>>::const_iterator aWhere, std::shared_ptr<EditorEntity> aParent)
//{
//	if (aWhere >= aCollection.end())
//	{
//		return;
//	}
//
//	bool isDirty = false;
//
//	std::shared_ptr<EditorEntity> entity = *aWhere;
//
//	ImGui::PushID(static_cast<int>(reinterpret_cast<intptr_t>(entity.get())));
//
//	std::string displayName = entity->GetTemplate().GetName();
//	if (entity->GetShowing() == false)
//	{
//		displayName = "[Hidden] - " + displayName;
//	}
//
//	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
//	if (mySelectedObject.get() == entity.get())
//	{
//		treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
//	}
//	if (entity->GetChildren().empty())
//	{
//		treeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
//	}
//
//	bool nodeOpen = ImGui::TreeNodeEx(displayName.c_str(), treeNodeFlags);
//
//	DragDropSource({ aParent, aWhere });
//
//	//Begin Drag drop target
//	{
//		payload_t payload;
//		if (DragDropTarget(&payload))
//		{
//			const std::vector<std::shared_ptr<EditorEntity>>& children = (**aWhere).GetChildren();
//			MoveEntity(payload.first, payload.second, *aWhere, children.end());
//			isDirty = true;
//		}
//	}
//
//	//Display Node
//	if (nodeOpen)
//	{
//		if (isDirty == false)
//		{
//			ImGui::BeginChildFrame(static_cast<ImGuiID>(reinterpret_cast<intptr_t>((*aWhere).get())), { -1, 256 }, ImGuiWindowFlags_AlwaysAutoResize);
//			const std::vector<std::shared_ptr<EditorEntity>>& children = (**aWhere).GetChildren();
//			ShowSidebarItems(children, children.begin(), entity);
//
//			ImGui::EndChildFrame();
//			payload_t payload;
//			if (DragDropTarget(&payload))
//			{
//				aWhere = MoveEntity(payload.first, payload.second, aParent, aWhere);
//				isDirty = true;
//			}
//		}
//		ImGui::TreePop();
//	}
//	if (isDirty == false && ImGui::IsItemClicked())
//	{
//		Select(entity);
//	}
//
//	ImGui::PopID();
//
//	if (isDirty == false)
//	{
//		ShowSidebarItems(aCollection, std::next(aWhere), aParent);
//	}
//}

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
			EntityTemplate tObject;
			tObject.AddComponent("Transform", { {"Translation", {worldPos.x, worldPos.y, 0.f}} });
			tObject.AddComponent("SpriteRenderer2D");

			std::shared_ptr<EditorTree::Node> editor = AddEditorEntity();
			editor->SetTemplate(tObject);

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
					EntityTemplate prefabInstance(unresolvedPath);
					std::shared_ptr<EditorTree::Node> editor = AddEditorEntity();

					for (ComponentTemplate& comp : prefabInstance.GetRawComponents())
					{
						if (comp.Type == "Transform")
						{
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

//void EditorEntityManager::onClick(std::shared_ptr<EditorTree::Node> aNode)
//{
//	myClickedNodes.push_back(aNode);
//}
