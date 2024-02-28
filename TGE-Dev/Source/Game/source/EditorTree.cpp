#include "EditorTree.h"
#include <fstream>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Entity.h"
#include <ShlObj_core.h>
#include <io.h>
#include <tge/util/StringCast.h>
#include <regex>

#pragma region NodeCollection

NodeCollection::NodeCollection()
{
	myIsDisabled = false;
	myIsClicked = false;
	myParent = nullptr;
}

void NodeCollection::SetName(const std::string& aName)
{
	myName = aName;
}

const std::string& NodeCollection::GetName() const
{
	return myName;
}

bool NodeCollection::BeginDisplay()
{
	FilterChildren();

	ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (myChildren.empty())
	{
		treeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	bool nodeOpen = ImGui::TreeNodeEx((GetName() + "##Node").c_str(), treeFlags);
	return nodeOpen;
}

void NodeCollection::DisplayChildren(std::vector<std::shared_ptr<NodeCollection>>& aOutClicked, bool aAllowDrop, bool* aOutDirty)
{
	if (size() != 0)
	{
		bool allowingDragDrop = aAllowDrop && IsDisabled() == false;

		static constexpr char* nodePayloadType = "NodeCollectionMovePayload";
		ImGui::BeginChild(ImGui::GetID(this), { -1, myContentsSizeY }, false);

		ImGui::BeginGroup();
#pragma region Node_Contents

		iterator it = myChildren.begin();
		bool isDirty = false;
		while (isDirty == false && it < myChildren.end())
		{
			std::shared_ptr<NodeCollection> node = *it;

			ImGui::PushID(node.get());

			bool open = node->BeginDisplay();

			if (ImGui::IsItemClicked() || node->EatClicked())
			{
				aOutClicked.push_back(node);
			}

			if (open)
			{

				if (allowingDragDrop && ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(nodePayloadType))
					{
						DragDropPayload* data = reinterpret_cast<DragDropPayload*>(payload->Data);
						if (data->CurrentParent)
						{
							//std::cout << "[EditorTree]: Dropped : '" << (*data->Iterator)->GetName() << "', on '" << (*it)->GetName() << "'." << std::endl;
							Move(data->CurrentParent, data->Iterator, (*it).get(), (*it)->begin());
							data->CurrentParent = nullptr;
							isDirty = true;
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (allowingDragDrop && ImGui::BeginDragDropSource())
				{
					DragDropPayload data;
					data.CurrentParent = this;
					data.Iterator = it;
					ImGui::SetDragDropPayload(nodePayloadType, &data, sizeof(data));

					ImGui::EndDragDropSource();
				}

				if (isDirty == false)
				{
					node->DisplayChildren(aOutClicked, allowingDragDrop, &isDirty);
				}
				node->EndDisplay();
			}

			ImGui::PopID();

			if (isDirty == false && it < myChildren.end())
			{
				++it;
			}
		}

#pragma endregion
		ImGui::EndGroup();
		myContentsSizeY = ImGui::GetItemRectSize().y;
		myContentsSizeY += ImGui::GetStyle().ItemSpacing.y;

		ImGui::EndChild();

		if (allowingDragDrop && ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(nodePayloadType))
			{
				DragDropPayload* data = reinterpret_cast<DragDropPayload*>(payload->Data);
				if (data->CurrentParent)
				{
					Move(data->CurrentParent, data->Iterator, this, myChildren.begin());
					data->CurrentParent = nullptr;
					isDirty = true;
					//std::cout << "[EditorTree]: Dropped Payload on child window." << std::endl;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (aOutDirty)
		{
			*aOutDirty = isDirty;
		}
	}
}

void NodeCollection::EndDisplay()
{
	//ImGui::Unindent();
	ImGui::TreePop();
}

void NodeCollection::AddChild(std::shared_ptr<NodeCollection> aChild)
{
	myChildren.push_back(aChild);
	aChild->SetParent(this);
	OnChildAdded(aChild);
}

void NodeCollection::SetDisabled(bool aDisabled)
{
	myIsDisabled = aDisabled;
}

bool NodeCollection::IsDisabled() const
{
	return myIsDisabled;
}

void NodeCollection::SetClicked()
{
	myIsClicked = true;
}

bool NodeCollection::EatClicked()
{
	if (myIsClicked)
	{
		myIsClicked = false;
		return true;
	}
	else return false;
}

#pragma region iterators

NodeCollection::iterator NodeCollection::begin()
{
	return myChildren.begin();
}

NodeCollection::const_iterator NodeCollection::cbegin() const
{
	return myChildren.cbegin();
}

NodeCollection::iterator NodeCollection::end()
{
	return myChildren.end();
}

NodeCollection::const_iterator NodeCollection::begin() const
{
	return myChildren.begin();
}

NodeCollection::const_iterator NodeCollection::end() const
{
	return myChildren.end();
}

NodeCollection::const_iterator NodeCollection::cend() const
{
	return myChildren.cend();
}

void NodeCollection::clear()
{
	myChildren.clear();
}

size_t NodeCollection::size() const
{
	return myChildren.size();
}

#pragma endregion

void NodeCollection::SetParent(NodeCollection* aParent)
{
	myParent = aParent;
}

void NodeCollection::Move(NodeCollection* aFromParent, const_iterator aFrom, NodeCollection* aToParent, const_iterator aTo)
{
	NodeCollection* p = aToParent;
	std::shared_ptr<NodeCollection> node = *aFrom;
	while (p != nullptr)
	{
		if (p == node.get())
		{
			return;
		}
		p = p->myParent;
	}

	size_t ind = std::distance(aToParent->myChildren.cbegin(), aTo);

	aFromParent->myChildren.erase(aFrom);
	aToParent->myChildren.insert(aToParent->myChildren.cbegin() + ind, node);
	node->SetParent(aToParent);
	aToParent->OnChildAdded(node);
}

void NodeCollection::FilterChildren()
{
	myChildren.erase(
		std::remove_if(
			myChildren.begin(),
			myChildren.end(),
			[](std::shared_ptr<NodeCollection>& child) {
				return child->ShouldBeRemoved();
			}
		),
		myChildren.end()
	);
}

#pragma endregion

#pragma region Node

EditorTree::Node::Node(bool aDisabled, EntityTemplate aTemplate)
{
	myEditorEntity = EditorEntity::CreateEditorObject();
	myEditorEntity->GetEntity()->GetComponent<ClickableSprite>()->GetOnClick().Subscribe([=](auto) { SetClicked(); });
	SetDisabled(aDisabled);
	SetTemplate(aTemplate);
}

EditorTree::Node::~Node()
{
	Destroy();
}

void EditorTree::Node::SetTemplate(const EntityTemplate& aTemplate)
{
	EntityTemplate tEntity = aTemplate;
	SetName(tEntity.GetName());
	NodeCollection::clear();
	tEntity.Reload();

	myEditorEntity->SetTemplate(tEntity);

	for (const EntityTemplate& tChild : tEntity.GetRawChildren())
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(false || IsDisabled(), tChild);
		//node->SetTemplate(tChild);
		NodeCollection::AddChild(node);
	}
	for (const EntityTemplate& tChild : tEntity.GetPrefabChildren())
	{
		std::shared_ptr<Node> node = std::make_shared<Node>(true, tChild);
		//node->SetTemplate(tChild);
		NodeCollection::AddChild(node);
	}
}

EntityTemplate EditorTree::Node::GetTemplate()
{
	EntityTemplate tEntity = myEditorEntity->GetTemplate();

	tEntity.GetRawChildren().clear();
	tEntity.Reload();

	for (std::shared_ptr<NodeCollection>& child : *this)
	{
		std::shared_ptr<Node> node = std::dynamic_pointer_cast<Node>(child);
		if (node->IsDisabled() == false)
		{
			tEntity.TryAddChild(node->GetTemplate());
		}
	}

	return tEntity;
}

void EditorTree::Node::MakeIndependent()
{
	SetTemplate(GetTemplate().GetIndependent());
}

void EditorTree::Node::MakePrefab(const std::string& aPath)
{
	nlohmann::json data = GetTemplate().Save();
	std::ofstream file(aPath);
	file << data;
	EntityTemplate tEntity;
	tEntity.Load(data);
	SetTemplate(tEntity);
}

void EditorTree::Node::ImGui_ObjectEditor()
{
	if (ImGui::Begin("Entity Editor"))
	{
		if (myEditorEntity)
		{
			bool disabled = IsDisabled();
			if (disabled)
			{
				ImGui::BeginDisabled();
			}

			// Entity Header
			if (ImGui::Button("Delete") || ImGui::IsKeyDown(ImGuiKey_Delete))
			{
				Destroy();
				//GetEditorEntity()->GetEntity()->Destroy();
			}
			ImGui::SameLine();

			////DEBUG
			//if (ImGui::Button("Regen Children"))
			//{
			//    GenerateChildren(GetTemplate());
			//}
			//ImGui::SameLine();

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

			if (ImGui::Button("Make Prefab"))
			{
				MakeIntoPrefab();
			}


			if (disabled)
			{
				ImGui::EndDisabled();
			}

			ImGui::SameLine();
			bool showing = myEditorEntity->GetShowing();
			if (ImGui::Checkbox("Show", &showing))
			{
				PropegateShowing(showing);
			}

			if (disabled)
			{
				ImGui::BeginDisabled();
			}

			myEditorEntity->ImGui_ObjectEditor();

			SetName(myEditorEntity->GetTemplate().GetName());


			if (disabled)
			{
				ImGui::EndDisabled();
			}
		}
		else
		{
			ImGui::Text((std::string("[") + __FILE__ + "]: Did not have an assigned template.").c_str());
		}
	}
	ImGui_ShowPrefabNameInputPopup();
	ImGui::End();
}

std::shared_ptr<EditorEntity> EditorTree::Node::GetEditorEntity()
{
	return myEditorEntity;
}

void EditorTree::Node::Destroy()
{
	if (myEditorEntity->IsDestroyed() == false)
	{
		myEditorEntity->GetEntity()->Destroy();
	}
}

bool EditorTree::Node::IsDestroyed() const
{
	return myEditorEntity->IsDestroyed();
}

void EditorTree::Node::SetPosition(const cu::Vector3<float>& aPosition)
{
	myEditorEntity->GetEntity()->GetComponent<Transform>()->SetLocalPosition(aPosition);
}

void EditorTree::Node::PropegateShowing(bool aShowing)
{
	GetEditorEntity()->SetShowing(aShowing);
	for (std::shared_ptr<NodeCollection>& child : *this)
	{
		std::shared_ptr<Node> node = std::dynamic_pointer_cast<Node>(child);
		if (node)
		{
			node->PropegateShowing(aShowing);
		}
	}
}

bool EditorTree::Node::BeginDisplay()
{
	bool isPrefab = myEditorEntity->IsPrefabInstance();
	float rFactor = 1.f;
	float gFactor = 1.f;
	float bFactor = 1.f;
	if (isPrefab)
	{
		rFactor *= 1.0f;
		gFactor *= 2.0f;
		bFactor *= 1.0f;
	}
	if (GetEditorEntity()->GetShowing() == false)
	{
		rFactor *= 0.25f;
		gFactor *= 0.25f;
		bFactor *= 0.25f;
	}
	if (IsDisabled())
	{
		rFactor *= 0.5f;
		gFactor *= 0.5f;
		bFactor *= 0.5f;
	}

	ImVec4 headerColor = ImGui::GetStyleColorVec4(ImGuiCol_Header);
	headerColor.x *= rFactor;
	headerColor.y *= gFactor;
	headerColor.z *= bFactor;
	ImGui::PushStyleColor(ImGuiCol_Header, headerColor);

	ImVec4 headerActiveColor = ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive);
	headerActiveColor.x *= rFactor;
	headerActiveColor.y *= gFactor;
	headerActiveColor.z *= bFactor;
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerActiveColor);

	ImVec4 headerHoveredColor = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
	headerHoveredColor.x *= rFactor;
	headerHoveredColor.y *= gFactor;
	headerHoveredColor.z *= bFactor;
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerHoveredColor);

	bool result = NodeCollection::BeginDisplay();

	ImGui::PopStyleColor(3);
	return result;
}

void EditorTree::Node::OnChildAdded(std::shared_ptr<NodeCollection> aChild)
{
	std::shared_ptr<Transform> transform = myEditorEntity->GetEntity()->GetComponent<Transform>();
	std::shared_ptr<Transform> childTransform = std::dynamic_pointer_cast<Node>(aChild)->myEditorEntity->GetEntity()->GetComponent<Transform>();

	childTransform->SetParent(transform, false);
}

void EditorTree::Node::OverwritePrefab()
{
	EntityTemplate tEntity = GetTemplate();
	tEntity.OverwritePrefab();
	SetTemplate(tEntity);
}

void EditorTree::Node::MakeIntoPrefab()
{
	ImGui::OpenPopup("PrefabNameInputPopup");
}

bool EditorTree::Node::CheckSavePrefab(const std::string& aPath) const
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

void EditorTree::Node::ImGui_ShowPrefabNameInputPopup()
{
	bool open = true;
	if (ImGui::BeginPopupModal("PrefabNameInputPopup", &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		static const std::regex fileRx("\\w+");
		std::string input;
		bool submitted = (ImGui::InputText("Prefab Filename", &input, ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue,
			[](ImGuiInputTextCallbackData* data) {
				char chars[2];
				chars[1] = '\0';
				chars[0] = static_cast<char>(data->EventChar);
				return static_cast<int>(std::regex_match(chars, fileRx) == false);
			}
		));
		if (submitted)
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

#pragma endregion
#pragma region EditorTree

void EditorTree::OnChildAdded(std::shared_ptr<NodeCollection> aChild)
{
	std::shared_ptr<Transform> childTransform = std::dynamic_pointer_cast<EditorTree::Node>(aChild)->GetEditorEntity()->GetEntity()->GetComponent<Transform>();

	childTransform->SetParent(nullptr, false);
}

#pragma endregion

