#pragma once
#include "EditorEntity.h"

class EditorEntityTreeNode
{
public:
	static std::shared_ptr<EditorEntityTreeNode> Create();

	~EditorEntityTreeNode();

	void SetTemplate(const EntityTemplate& aTemplate);
	std::shared_ptr<EditorEntityTreeNode> AddChild(const EntityTemplate& aTemplate);
	std::shared_ptr<EditorEntityTreeNode> AddEmptyChild();

	std::weak_ptr<EditorEntityTreeNode> GetParent() const;

	bool IsChildOf(std::shared_ptr<EditorEntityTreeNode> aNode) const;
	void Move(std::weak_ptr<EditorEntityTreeNode> aNode);

	bool ImGui_Display();
	bool IsPrefabChild() const;
	void SetPrefabChild(bool aValue);
	bool IsEditable() const;
	void SetEditable(bool aValue);

	std::string GetName();

	Event<std::shared_ptr<EditorEntityTreeNode>> OnClick;

	EntityTemplate GetTemplate() const;

	const std::multiset<std::shared_ptr<EditorEntityTreeNode>>& GetChildren() const;

	void SetPosition(const cu::Vector3<float>& aPosition);
	void Reload();

	std::shared_ptr<EditorEntity> GetEditorEntity();

	void ImGui_ShowObjectEditor();

	bool CheckSavePrefab(const std::string& aPath) const;

	void ImGui_ShowPrefabNameInputPopup();

private:
	EditorEntityTreeNode();

	void MakeIndependent();
	void OverwritePrefab();
	void MakeIntoPrefab();

	void PropegateShowing(bool aShowing);

	ImVec2 myChildrenContentSize;

	std::weak_ptr<EditorEntityTreeNode> myInstance;

	bool myIsPrefabChild;
	bool myIsEditable;

	void GenerateChildren(const EntityTemplate& aTemplate);
	void RemoveWithoutDelete(std::shared_ptr<EditorEntityTreeNode> aNode);
	void AddChild(std::shared_ptr<EditorEntityTreeNode> aNode);

	std::shared_ptr<EditorEntity> myEditorEntity;
	std::weak_ptr<EditorEntityTreeNode> myParent;
	std::multiset<std::shared_ptr<EditorEntityTreeNode>> myChildren;
};