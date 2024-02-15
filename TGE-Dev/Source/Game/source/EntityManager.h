#pragma once
#include "Entity.h"
#include "EntityTemplate.h"
#include "ComponentRegistry.h"
#include "Event.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	std::shared_ptr<Entity> Add(const EntityTemplate2& aEntity, const ComponentRegistry& aRegistry);
	std::vector<std::shared_ptr<Entity>> LoadBatch(const std::string& aPath, const ComponentRegistry& aRegistry);
	std::vector<std::shared_ptr<Entity>> LoadBatch(const nlohmann::json& aBatch, const ComponentRegistry& aRegistry);
	void DestroyAll();
	void Clear();

	void AddPending();
	void Update(float aDeltaTime);
	void Render(Tga::GraphicsEngine& aGraphicsEngine);
	void RemoveDestroyed();

	Event<std::shared_ptr<Entity>>& GetOnAddEntity();

private:
	bool myIsClearing;
	std::vector<std::shared_ptr<Entity>> myPendingEntities;
	std::vector<std::shared_ptr<Entity>> myEntities;

	Event<std::shared_ptr<Entity>> myOnAddEntity;
};