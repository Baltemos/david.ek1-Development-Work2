#include "EditorUnityManager.h"
#include "GameWorld.h"

void EditorUnityManager::LoadBatch(const nlohmann::json& aBatch)
{
	auto& manager = GameWorld::GetInstance()->GetEntityManager();
	auto& registry = GameWorld::GetInstance()->GetComponentRegistry();
	std::shared_ptr<Transform> rootTransform = myUnityRoot->GetComponent<Transform>();
	for (auto& entity : aBatch)
	{
		EntityTemplate2 tEntity;
		tEntity.Load(entity);

		manager.Add(tEntity, registry)->GetComponent<Transform>()->SetParent(rootTransform.get(), true);
	}
}

void EditorUnityManager::Clear()
{
	std::shared_ptr<Transform> rootTransform = myUnityRoot->GetComponent<Transform>();
	std::vector<Transform*> children = rootTransform->GetChildren();
	for (auto child : children)
	{
		child->GetEntity()->Destroy();
	}
}

void EditorUnityManager::Read(const nlohmann::json&)
{
	EntityTemplate2 tUnityRoot;
	tUnityRoot.AddComponent("Transform");

	myUnityRoot = GameWorld::GetInstance()->GetEntityManager().Add(tUnityRoot, GameWorld::GetInstance()->GetComponentRegistry());
}
