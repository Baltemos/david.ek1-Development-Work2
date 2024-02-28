#include "EditorEntityTreeNode.h"
#include "Entity.h"
#include <regex>
#include <io.h>
#include <ShlObj_core.h>
#include <tge/util/StringCast.h>
#include <fstream>
#include <imgui/misc/cpp/imgui_stdlib.h>

static int locInstanceCount = 0;
static int locCreated = 0;
static int locDestroyed = 0;
static int locChildrenRemoved = 0;

EditorEntityTreeNode::EditorEntityTreeNode()
{
	locInstanceCount++;
	locCreated++;
	std::cout << "[EditorEntityTreeNode]: Created Instance, Count: " << locInstanceCount << std::endl;
	std::cout << "[EditorEntityTreeNode]: Created Count: " << locCreated << std::endl;
	myEditorEntity = EditorEntity::CreateEditorObject();
	myEditorEntity->GetEntity()->GetComponent<ClickableSprite>()->GetOnClick().Subscribe(
		[=](ClickableSprite*) {
			OnClick(myInstance.lock());
		}
	);
	myIsPrefabChild = false;
	myIsEditable = true;
}

void EditorEntityTreeNode::PropegateShowing(bool aShowing)
{
	myEditorEntity->SetShowing(aShowing);
	for (auto& child : myChildren)
	{
		child->PropegateShowing(aShowing);
	}
}

std::shared_ptr<EditorEntityTreeNode> EditorEntityTreeNode::Create()
{
	std::shared_ptr<EditorEntityTreeNode> node = std::shared_ptr<EditorEntityTreeNode>(new EditorEntityTreeNode());
	node->myInstance = node;
	return node;
}

EditorEntityTreeNode::~EditorEntityTreeNode()
{
	locInstanceCount--;
	locDestroyed++;
	std::cout << "[EditorEntityTreeNode]: Instance Destroyed, Count: " << locInstanceCount << std::endl;
	std::cout << "[EditorEntityTreeNode]: Destroyed Count: " << locDestroyed << std::endl;


	//DestroyChildren();
	myChildren.clear();
	//if (myEditorEntity && myEditorEntity->IsDestroyed() == false)
	//{
	myEditorEntity->GetEntity()->Destroy();
	//}
}

void EditorEntityTreeNode::SetTemplate(const EntityTemplate& aTemplate)
{
	myEditorEntity->SetTemplate(aTemplate);
	GenerateChildren(aTemplate);
}

std::shared_ptr<EditorEntityTreeNode> EditorEntityTreeNode::AddChild(const EntityTemplate& aTemplate)
{
	std::shared_ptr<EditorEntityTreeNode> node = EditorEntityTreeNode::Create();
	node->SetTemplate(aTemplate);
	AddChild(node);
	return node;
}

std::shared_ptr<EditorEntityTreeNode> EditorEntityTreeNode::AddEmptyChild()
{
	std::shared_ptr<EditorEntityTreeNode> node = EditorEntityTreeNode::Create();
	AddChild(node);
	return node;
}

std::weak_ptr<EditorEntityTreeNode> EditorEntityTreeNode::GetParent() const
{
	return myParent;
}

bool EditorEntityTreeNode::IsChildOf(std::shared_ptr<EditorEntityTreeNode> aNode) const
{
	std::shared_ptr<EditorEntityTreeNode> node = myInstance.lock();
	while (node != nullptr)
	{
		node = node->myParent.lock();
		if (node == aNode)
		{
			return true;
		}
	}
	return false;
}

void EditorEntityTreeNode::Move(std::weak_ptr<EditorEntityTreeNode> aNode)
{
	if (aNode.expired() || aNode.lock()->IsChildOf(myInstance.lock()) == false)
	{
		if (myParent.expired() == false)
		{
			myParent.lock()->RemoveWithoutDelete(myInstance.lock());
		}

		if (aNode.expired() == false)
		{
			aNode.lock()->AddChild(myInstance.lock());
		}
	}
}

//void EditorEntityTreeNode::RemoveDestroyed()
//{
//	auto it = myChildren.begin();
//	while (it != myChildren.end())
//	{
//		if ((**it).IsDestroyed())
//		{
//			it = myChildren.erase(it);
//			continue;
//		}
//		else
//		{
//			(**it).RemoveDestroyed();
//		}
//		++it;
//	}
//}

bool EditorEntityTreeNode::ImGui_Display()
{
	bool moved = false;

	if (myInstance.use_count() != 1)
	{
		std::cout << "Boy what the hell." << std::endl;
	}

	bool treeOpen = false;
	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
	if (myChildren.empty())
	{
		treeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	ImGui::PushID(myInstance.lock().get());

	if (myEditorEntity && myEditorEntity->IsPrefabInstance() || IsPrefabChild())
	{
		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Header);
		color.x *= 1.5;
		color.y *= 1.5;
		color.z *= 1.5;
		ImGui::PushStyleColor(ImGuiCol_Header, color);
	}
	treeOpen = ImGui::TreeNodeEx((GetName() + "##Node").c_str(), treeFlags);
	if (myEditorEntity && myEditorEntity->IsPrefabInstance() || IsPrefabChild())
	{
		ImGui::PopStyleColor();
	}
	ImGui::PopID();

	if (ImGui::IsItemClicked())
	{
		OnClick(myInstance.lock());
	}

	if (IsEditable() && ImGui::BeginDragDropSource())
	{
		std::weak_ptr<EditorEntityTreeNode> node = myInstance;
		ImGui::SetDragDropPayload("EditorEntityTreeNodeMovePayload", &node, sizeof(node));
		ImGui::EndDragDropSource();
	}

	if (IsEditable() && ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorEntityTreeNodeMovePayload"))
		{
			std::weak_ptr<EditorEntityTreeNode> node = *reinterpret_cast<std::weak_ptr<EditorEntityTreeNode>*>(payload->Data);
			if (node.expired() == false)
			{
				node.lock()->Move(myInstance.lock());
				moved = true;
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (treeOpen && (treeFlags & ImGuiTreeNodeFlags_Leaf) == 0)
	{
		ImGui::PushID(myInstance.lock().get());

		ImGuiStyle& style = ImGui::GetStyle();

		cu::Vector2<float> itemSpacing;
		itemSpacing.x = style.ItemSpacing.x;
		itemSpacing.y = style.ItemSpacing.y;
		cu::Vector2<float> framePadding;
		framePadding.x = style.FramePadding.x;
		framePadding.y = style.FramePadding.y;


		ImGui::BeginChild(ImGui::GetID(this), { -1, myChildrenContentSize.y }, false, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::BeginGroup();
		ImGui::Indent(itemSpacing.x);

		for (std::shared_ptr<EditorEntityTreeNode> child : myChildren)
		{
			moved = child->ImGui_Display();
			if (moved)
			{
				break;
			}
		}

		ImGui::Indent(-itemSpacing.x);
		ImGui::EndGroup();
		myChildrenContentSize = ImGui::GetItemRectSize();
		ImGui::EndChild();

		if (IsEditable() && ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EditorEntityTreeNodeMovePayload"))
			{
				std::shared_ptr<EditorEntityTreeNode> node = *reinterpret_cast<std::shared_ptr<EditorEntityTreeNode>*>(payload->Data);
				node->Move(myInstance);
				moved = true;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}
	if (treeOpen)
	{
		ImGui::TreePop();
	}
	return moved;
}

//bool EditorEntityTreeNode::IsDestroyed() const
//{
//	return myEditorEntity && myEditorEntity->IsDestroyed();
//}

bool EditorEntityTreeNode::IsPrefabChild() const
{
	return myIsPrefabChild;
}

void EditorEntityTreeNode::SetPrefabChild(bool aValue)
{
	myIsPrefabChild = aValue;
	for (std::shared_ptr<EditorEntityTreeNode> child : myChildren)
	{
		child->SetPrefabChild(aValue);
	}
}

bool EditorEntityTreeNode::IsEditable() const
{
	return myIsEditable && myIsPrefabChild == false;
}

void EditorEntityTreeNode::SetEditable(bool aValue)
{
	myIsEditable = aValue;
}

std::string EditorEntityTreeNode::GetName()
{
	if (myEditorEntity)
	{
		return myEditorEntity->GetTemplate().GetName();
	}
	else return "Root";
}

//void EditorEntityTreeNode::DestroyChildren()
//{
//	for (std::shared_ptr<EditorEntityTreeNode> child : myChildren)
//	{
//		child->Destroy();
//	}
//	myChildren.clear();
//}

EntityTemplate EditorEntityTreeNode::GetTemplate() const
{
	if (myEditorEntity)
	{
		EntityTemplate tEntity = myEditorEntity->GetTemplate();
		std::vector<EntityTemplate>& children = tEntity.GetRawChildren();
		children.clear();
		for (std::shared_ptr<EditorEntityTreeNode> node : myChildren)
		{
			if (node->IsPrefabChild())
			{
			}
			else
			{
				children.push_back(node->GetTemplate());
			}
		}
		return tEntity;
	}
	else throw std::exception("Tried to get template from node witout set template.");
}

const std::multiset<std::shared_ptr<EditorEntityTreeNode>>& EditorEntityTreeNode::GetChildren() const
{
	return myChildren;
}

void EditorEntityTreeNode::SetPosition(const cu::Vector3<float>& aPosition)
{
	if (myEditorEntity)
	{
		myEditorEntity->TrySetPosition(aPosition);
	}
}

void EditorEntityTreeNode::Reload()
{
	if (myEditorEntity)
	{
		SetTemplate(GetTemplate());
		myEditorEntity->Reload();
	}
}

void EditorEntityTreeNode::MakeIndependent()
{
	if (myEditorEntity)
	{
		//Reload();
		SetTemplate(GetTemplate().GetIndependent());
		//Reload();
	}
}

void EditorEntityTreeNode::OverwritePrefab()
{
	if (myEditorEntity)
	{
		GetTemplate().OverwritePrefab();
		Reload();
	}
}

void EditorEntityTreeNode::MakeIntoPrefab()
{
	if (myEditorEntity)
	{
		//Reload();
		ImGui::OpenPopup("PrefabNameInputPopup");
		//Reload();
	}
}

std::shared_ptr<EditorEntity> EditorEntityTreeNode::GetEditorEntity()
{
	return myEditorEntity;
}

void EditorEntityTreeNode::ImGui_ShowObjectEditor()
{
	if (ImGui::Begin("Entity Editor"))
	{
		if (myEditorEntity)
		{
			if (IsEditable() == false)
			{
				ImGui::BeginDisabled();
			}

			// Entity Header
			if (ImGui::Button("Delete") || ImGui::IsKeyDown(ImGuiKey_Delete))
			{
				GetEditorEntity()->GetEntity()->Destroy();
			}
			ImGui::SameLine();

			//DEBUG
			if (ImGui::Button("Regen Children"))
			{
				GenerateChildren(GetTemplate());
			}
			ImGui::SameLine();

			if (GetEditorEntity()->IsPrefabInstance())
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
					MakeIntoPrefab();
				}
			}

			ImGui::SameLine();
			bool showing = myEditorEntity->GetShowing();
			ImGui::Checkbox("Show", &showing);
			myEditorEntity->SetShowing(showing);

			myEditorEntity->ImGui_ObjectEditor();
			PropegateShowing(myEditorEntity->GetShowing());


			if (IsEditable() == false)
			{
				ImGui::EndDisabled();
			}
		}
		else
		{
			ImGui::Text((std::string("[") + __FILE__ + "]: Did not have an assigned template.").c_str());
		}
	}
	ImGui::End();

	ImGui_ShowPrefabNameInputPopup();
}

bool EditorEntityTreeNode::CheckSavePrefab(const std::string& aPath) const
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

void EditorEntityTreeNode::ImGui_ShowPrefabNameInputPopup()
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
					file << GetTemplate().Save();
				}

				SetTemplate(EntityTemplate(path));
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
}

void EditorEntityTreeNode::GenerateChildren(const EntityTemplate& aTemplate)
{
	locChildrenRemoved += static_cast<int>(myChildren.size());
	myChildren.clear();
	if (myEditorEntity)
	{
		for (const EntityTemplate& child : aTemplate.GetRawChildren())
		{
			std::shared_ptr<EditorEntityTreeNode> entity = AddChild(child);
			entity->SetEditable(IsEditable());
		}

		for (const EntityTemplate& child : aTemplate.GetPrefabChildren())
		{
			std::shared_ptr<EditorEntityTreeNode> entity = AddChild(child);
			entity->SetPrefabChild(true);
			entity->SetEditable(false);
		}
	}
}

void EditorEntityTreeNode::RemoveWithoutDelete(std::shared_ptr<EditorEntityTreeNode> aNode)
{
	myChildren.erase(aNode);
	aNode->myParent.reset();
}

void EditorEntityTreeNode::AddChild(std::shared_ptr<EditorEntityTreeNode> aNode)
{
	myChildren.emplace(aNode);
	aNode->myParent = myInstance;
	aNode->OnClick.UnsubscribeAll();
	aNode->OnClick.Subscribe([=](std::shared_ptr<EditorEntityTreeNode> aNode) { OnClick(aNode); });

	std::shared_ptr<EditorEntity> childEntity = aNode->GetEditorEntity();

	if (myEditorEntity && childEntity)
	{
		childEntity->GetEntity()->GetComponent<Transform>()->SetParent(myEditorEntity->GetEntity()->GetComponent<Transform>(), false);
	}
	else if (childEntity)
	{
		childEntity->GetEntity()->GetComponent<Transform>()->SetParent(nullptr, false);
	}
}
