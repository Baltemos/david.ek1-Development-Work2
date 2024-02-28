#pragma once
#include "Component.h"
#include "Collider2D.h"

class Health : public Component
{
public:
protected:
	void Read(const nlohmann::json& someData) override;
	void Update(float aDeltaTime) override;

	virtual bool TakeDamage(const int someDamage);
	void Heal(const int someHealing);
	std::shared_ptr<Collider> myCollider;

	void GetOnCollision(const CollisionInfo2D& aInfo);

private:
	int myCurrentHP;
	int myMaxHP;
	bool myIsAlive;
	float myInvulnerabilityTimer;
	float myInvulnerabilityDuration;
};

