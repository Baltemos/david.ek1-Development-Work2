#include "EntityManager.h"
#include "JsonUtil.h"
#include <fstream>
#include "Transform.h"

EntityManager::EntityManager()
{
	myIsClearing = false;
}

EntityManager::~EntityManager()
{
	Clear();
}

std::shared_ptr<Entity> EntityManager::Add(const EntityTemplate2& aEntity, const ComponentRegistry& aRegistry)
{
	if (myIsClearing)
	{
		return nullptr;
	}

	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	std::vector<std::pair<std::shared_ptr<Component>, nlohmann::json>> components;

	for (const ComponentTemplate& componentTemplate : aEntity.GetComponents())
	{
		const ComponentRegistry::Entry& entry = aRegistry.GetEntry(componentTemplate.Type);
		std::shared_ptr<Component> component = entry.Constructor();
		components.push_back(std::pair(component, JsonMerge(entry.DefaultData, JsonMerge(componentTemplate.Defaults, componentTemplate.Overrides, true), true)));
		component->AssignEntity(entity);
		entity->AddComponent(component);
	}

	for (auto& comp : components)
	{
		comp.first->Read(comp.second);
	}

	std::shared_ptr<Transform> transform = entity->GetComponent<Transform>();
	if (transform != nullptr)
	{
		for (const EntityTemplate2& tChild : aEntity.GetChildren())
		{
			if (tChild.ContainsComponent("Transform"))
			{
				std::shared_ptr<Entity> child = Add(tChild, aRegistry);
				child->GetComponent<Transform>()->SetParent(transform.get(), false);
			}
		}
	}

	myPendingEntities.push_back(entity);

	myOnAddEntity(entity);

	return entity;
}

std::vector<std::shared_ptr<Entity>> EntityManager::LoadBatch(const std::string& aPath, const ComponentRegistry& aRegistry)
{
	nlohmann::json batch;
	std::ifstream file(Tga::Settings::ResolveGameAssetPath(aPath));
	file >> batch;
	std::vector<std::shared_ptr<Entity>> entities = LoadBatch(batch, aRegistry);
	return entities;
}

std::vector<std::shared_ptr<Entity>> EntityManager::LoadBatch(const nlohmann::json& aBatch, const ComponentRegistry& aRegistry)
{
	std::vector<std::shared_ptr<Entity>> entities;
	EntityTemplate2 tEntity;
	for (auto& entity : aBatch)
	{
		tEntity.Load(entity);
		entities.push_back(Add(tEntity, aRegistry));
	}
	return entities;
}

void EntityManager::DestroyAll()
{
	for (std::shared_ptr<Entity>& entity : myEntities)
	{
		entity->Destroy();
	}
}

void EntityManager::Clear()
{
	myIsClearing = true;
	AddPending();
	DestroyAll();
	myIsClearing = false;
}

void EntityManager::AddPending()
{
	std::vector<std::shared_ptr<Entity>> pending = myPendingEntities;
	myPendingEntities.clear();
	for (auto& entityPtr : pending)
	{
		myEntities.push_back(entityPtr);
	}
	for (auto& entityPtr : pending)
	{
		entityPtr->Start();
	}
}

void EntityManager::Update(float aDeltaTime)
{
	for (auto& entityPtr : myEntities)
	{
		entityPtr->Update(aDeltaTime);
	}
}

void EntityManager::Render(Tga::GraphicsEngine& aGraphicsEngine)
{
	for (auto& entityPtr : myEntities)
	{
		entityPtr->Render(aGraphicsEngine);
	}
}

void EntityManager::RemoveDestroyed()
{
	//auto it = myEntities.begin();
	//while (it < myEntities.end())
	//{
	//	if ((**it).IsDestroyed())
	//	{
	//		(**it).OnDestroy();
	//	}
	//	++it;
	//}

	auto it = myEntities.begin();
	while (it < myEntities.end())
	{
		if ((**it).IsDestroyed())
		{
			it = myEntities.erase(it);
			continue;
		}
		++it;
	}
}

Event<std::shared_ptr<Entity>>& EntityManager::GetOnAddEntity()
{
	return myOnAddEntity;
}
