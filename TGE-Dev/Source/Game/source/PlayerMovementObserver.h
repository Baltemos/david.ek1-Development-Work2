#pragma once
#include "CreatureMovementObserver.h"

class PlayerMovementObserver : public CreatureMovementObserver
{
public:
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;
private:
	void onJump(float aDeltaTime, float aSpeed) override;
	void onLetGoOfJump() override;
	void onDash(float aDeltaTime);
	void onLookVertical(int aDirection);

	void onCollision(const CollisionInfo2D& aInfo) override;

	void onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange) override;

	float myDashSpeed;
	float MyDashCooldown;
	//std::shared_ptr<int> myPlayerJumpsLeft;
};

