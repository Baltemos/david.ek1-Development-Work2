#pragma once
#include "CreatureMovementObserver.h"

class PlayerMovementObserver : public CreatureMovementObserver
{
public:
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;
private:
	void onDash(float aDeltaTime);
	void onLookVertical(int aDirection);

	void onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange) override;

	float myDashSpeed;
	float MyDashCooldown;
};

