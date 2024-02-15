#pragma once
#include "Component.h"
#include "EditorEntity.h"

class EditorEntityManager : public Component
{
public:	
	EditorEntityManager();

	std::shared_ptr<EditorEntity> AddEditorEntity();
	void ClearEditorEntities();

	void LoadBatch(const nlohmann::json& aBatch);
	nlohmann::json GetBatch() const;

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta) override;

private:
	void HandleClickedObjects();
	void Select(std::shared_ptr<EditorEntity> aEditorEntity);

	void ShowSidebar();
	void ShowToolbox();

	void onClick(ClickableObject* aObject);

	cu::InputHandler* myInputHandler;

	std::multiset<ClickableObject*> myClickedObjects;
	std::vector<std::shared_ptr<EditorEntity>> myEditorEntities;
	std::shared_ptr<EditorEntity> mySelectedObject;
	std::shared_ptr<EditorEntity> myCopiedObject;
};