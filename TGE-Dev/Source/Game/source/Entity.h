#pragma once
#include "Component.h"
#include <vector>

class Entity
{
	friend class EntityManager;
public:
	Entity();
	~Entity() = default;

	template<typename _ComponentTy>
	std::vector<std::shared_ptr<_ComponentTy>> GetComponents();
	template<typename _ComponentTy>
	std::shared_ptr<_ComponentTy> GetComponent();
	
	void Destroy();
	bool IsDestroyed() const;

	void Start();
	void Update(float aTimeDelta);
	void Render(Tga::GraphicsEngine& aGraphicsEngine);
	void OnDestroy();

	void SetIndestructible(bool aIndestructible);
	bool IsIndestructible() const;

private:
	void AddComponent(std::shared_ptr<Component> aComponent);

	bool myIsDestroyed;
	bool myIsIndestructible;
	std::vector<std::shared_ptr<Component>> myComponents;
};

template<typename _ComponentTy>
inline std::vector<std::shared_ptr<_ComponentTy>> Entity::GetComponents()
{
	std::vector<std::shared_ptr<_ComponentTy>> components;
	std::shared_ptr<_ComponentTy> component;
	for (auto& compPtr : myComponents)
	{
		component = std::dynamic_pointer_cast<_ComponentTy>(compPtr);
		if (component != nullptr)
		{
			components.push_back(component);
		}
	}
	return components;
}

template<typename _ComponentTy>
inline std::shared_ptr<_ComponentTy> Entity::GetComponent()
{
	std::shared_ptr<_ComponentTy> component;
	for (auto& compPtr : myComponents)
	{
		component = std::dynamic_pointer_cast<_ComponentTy>(compPtr);
		if (component != nullptr)
		{
			return component;
		}
	}
	return nullptr;
}
