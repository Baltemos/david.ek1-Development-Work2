#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <nlohmann/json.hpp>

class Entity;

class Component
{
	friend class Entity;
	friend class EntityManager;
public:
	template<typename _ComponentTy>
	static std::shared_ptr<_ComponentTy> CreateInstance();
	Component();

	std::shared_ptr<Entity> GetEntity() const;
	bool IsDestroyed() const;
	void SetEnabled(bool aEnabled);
	bool IsEnabled() const;

	std::shared_ptr<Component> GetSharedPtr();
	std::shared_ptr<const Component> GetSharedPtr() const;
protected:
	virtual void Read(const nlohmann::json& someData) { someData; };
	virtual void Start() {};
	virtual void Update(float aTimeDelta) { aTimeDelta; };
	virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) { aGraphicsEngine; };
	virtual void OnDestroy() {};
private:
	void AssignEntity(std::shared_ptr<Entity> anEntity);

	bool myIsEnabled;
	std::weak_ptr<Component> mySharedPtr;
	std::weak_ptr<Entity> myEntity;
};

template<typename _ComponentTy>
inline std::shared_ptr<_ComponentTy> Component::CreateInstance()
{
	std::shared_ptr<_ComponentTy> component = std::make_shared<_ComponentTy>();
	component->mySharedPtr = component;
	return component;
}