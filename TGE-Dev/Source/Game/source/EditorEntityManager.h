#pragma once
#pragma once
#include "Component.h"
#include "EditorEntity.h"
#include "EditorObjectDrawer.h"
//#include "EditorEntityTreeNode.h"
#include "EditorTree.h"

class EditorEntityManager : public Component
{
public:	
	EditorEntityManager();
	~EditorEntityManager();

	std::shared_ptr<EditorTree::Node> AddEditorEntity();
	void ClearEditorEntities();

	void LoadBatch(const nlohmann::json& aBatch);
	nlohmann::json GetBatch() const;

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta) override;

private:
	void HandleClickedObjects();
	void Select(std::weak_ptr<EditorTree::Node> aNode);

	void ShowSidebar();

	//typedef std::pair<std::shared_ptr<EditorEntity>, std::vector<std::shared_ptr<EditorEntity>>::const_iterator> payload_t;
	//void DragDropSource(const payload_t& aPayload);
	//bool DragDropTarget(payload_t* aOutPayload);
	//std::vector<std::shared_ptr<EditorEntity>>::const_iterator MoveEntity(
	//	std::shared_ptr<EditorEntity> aFirstParent,
	//	std::vector<std::shared_ptr<EditorEntity>>::const_iterator aFirstWhere,
	//	std::shared_ptr<EditorEntity> aSecondParent,
	//	std::vector<std::shared_ptr<EditorEntity>>::const_iterator aSecondWhere);
	//void ShowSidebarItems(const std::vector<std::shared_ptr<EditorEntity>>& aCollection, std::vector<std::shared_ptr<EditorEntity>>::const_iterator aWhere, std::shared_ptr<EditorEntity> aParent);
	void ShowToolbox();

	//void onClick(std::shared_ptr<EditorTree::Node> aNode);

	//EventKey myClickedEventKey;

	cu::InputHandler* myInputHandler;

	EditorObjectDrawer myEditorObjectDrawer;

	EditorTree myTree;

	std::vector<std::shared_ptr<EditorTree::Node>> myClickedNodes;
	//std::vector<std::shared_ptr<EditorEntity>> myEditorEntities;
	//std::shared_ptr<EditorEntityTreeNode> myRootNode;
	//std::vector<std::shared_ptr<EditorTree::Node>> myTreeNodes;
	std::weak_ptr<EditorTree::Node> mySelectedEntity;
	std::weak_ptr<EditorTree::Node> myCopiedObject;
};