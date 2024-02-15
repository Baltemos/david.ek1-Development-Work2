#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <nlohmann/json.hpp>

class Entity;

class Component
{
	friend class Entity;
	friend class EntityManager;
public:
	Component();

	std::shared_ptr<Entity> GetEntity() const;
	bool IsDestroyed() const;
	void SetEnabled(bool aEnabled);
	bool IsEnabled() const;
protected:
	virtual void Read(const nlohmann::json& someData) { someData; };
	virtual void Start() {};
	virtual void Update(float aTimeDelta) { aTimeDelta; };
	virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) { aGraphicsEngine; };
	virtual void OnDestroy() {};
private:
	void AssignEntity(std::shared_ptr<Entity> anEntity);

	bool myIsEnabled;
	std::weak_ptr<Entity> myEntity;
};