#include "EditorUnityManager.h"
#include "GameWorld.h"

void EditorUnityManager::LoadBatch(const nlohmann::json& aBatch)
{
	auto& manager = GameWorld::GetInstance()->GetEntityManager();
	auto& registry = GameWorld::GetInstance()->GetComponentRegistry();
	std::shared_ptr<Transform> rootTransform = myUnityRoot->GetComponent<Transform>();
	for (auto& entity : aBatch)
	{
		EntityTemplate tEntity;
		tEntity.Load(entity);

		manager.Add(tEntity, registry)->GetComponent<Transform>()->SetParent(rootTransform, true);
	}
}

void EditorUnityManager::Clear()
{
	std::shared_ptr<Transform> rootTransform = myUnityRoot->GetComponent<Transform>();
	const std::vector<std::weak_ptr<Transform>>& children = rootTransform->GetChildren();
	for (auto& child : children)
	{
		child.lock()->GetEntity()->Destroy();
	}
}

void EditorUnityManager::Read(const nlohmann::json&)
{
	EntityTemplate tUnityRoot;
	tUnityRoot.AddComponent("Transform");

	myUnityRoot = GameWorld::GetInstance()->GetEntityManager().Add(tUnityRoot, GameWorld::GetInstance()->GetComponentRegistry());
}
