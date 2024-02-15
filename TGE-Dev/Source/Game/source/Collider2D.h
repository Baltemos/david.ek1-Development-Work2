#pragma once
#include "Component.h"
#include <CommonUtilities/Vector.hpp>
#include "CommonUtilities/Circle.hpp"
#include "Transform.h"
#include "Event.h"

namespace cu = CommonUtilities;

class Collider;

enum class ColliderType : unsigned int
{
	eNone,
	eRectangle,
	eCircle,
	eCount
};

struct CollisionInfo2D
{
	bool Intersects;
	cu::Vector2<float> Normal;
	Collider* OurCollider;
	Collider* OtherCollider;
};

class Collider : public Component
{
public:
	static void RunCollisions();
	
	void SetStatic(bool aIsStatic);
	void SetTrigger(bool aIsStatic);

	bool IsStatic() const;
	bool IsTrigger() const;

	virtual CollisionInfo2D Intersects(Collider* aCollider);
	virtual void UpdateBounds() = 0;
	ColliderType GetType() const;
	cu::Circle<float> GetBoundingCircle() const;

	Event<const CollisionInfo2D&> OnCollision;
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;

	void SetType(ColliderType aType);

	void SetBoundingCircle(cu::Circle<float> aBoundingCircle);
private:
	bool PreliminaryCheck(Collider* aCollider) const;
	static void CheckCollision(Collider* aCollider1, Collider* aCollide2);

	ColliderType myType;

	bool myIsStatic;
	bool myIsTrigger;

	cu::Circle<float> myBoundingCircle;
};