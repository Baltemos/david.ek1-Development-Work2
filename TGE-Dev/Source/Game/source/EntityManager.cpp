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
	for (std::shared_ptr<Entity>& entity : myEntities)
	{
		entity->SetIndestructible(false);
	}
	Clear();

	AddPending();
	RemoveDestroyed();

	myEntities.clear();
}

std::shared_ptr<Entity> EntityManager::Add(const EntityTemplate& aEntity, const ComponentRegistry& aRegistry)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	std::vector<std::pair<std::shared_ptr<Component>, nlohmann::json>> components;

	for (const ComponentTemplate& componentTemplate : aEntity.GetComponents())
	{
		const ComponentRegistry::Entry& entry = aRegistry.GetEntry(componentTemplate.Type);
		std::shared_ptr<Component> component = entry.Constructor();
		components.push_back(std::pair(component, JsonMerge(entry.DefaultData, JsonMerge(componentTemplate.Defaults, componentTemplate.Overrides, true))));
		component->AssignEntity(entity);
		entity->AddComponent(component);
	}

	for (auto& comp : components)
	{
		comp.first->Read(comp.second);
	}

	std::shared_ptr<Transform> transform = entity->GetComponent<Transform>();

	std::vector<EntityTemplate> children = aEntity.GetChildren();

	if (transform)
	{
		for (const EntityTemplate& tChild : children)
		{
			if (tChild.ContainsComponent("Transform"))
			{
				std::shared_ptr<Entity> child = Add(tChild, aRegistry);
				std::shared_ptr<Transform> childTransform = child->GetComponent<Transform>();
				if (transform)
				{
					childTransform->SetParent(transform, false);
				}
			}
			else
			{
				std::cout << "[EntityManager]: Child did not have transform. Skipping creating child." << std::endl;
			}
		}
	}
	else if (children.empty() == false)
	{
		std::cout << "[EntityManager]: Did not have transform. Skipping creating " << children.size() << " children." << std::endl;
	}

	myPendingEntities.push_back(entity);

	myOnAddEntity(entity);

	if (myIsClearing)
	{
		entity->Destroy();
	}

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
	EntityTemplate tEntity;
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

	if (myIsClearing)
	{
		DestroyAll();
		myIsClearing = false;
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
