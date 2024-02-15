#pragma once
#include "CreatureMovementObserver.h"

class MeleeEnemyObserver : public CreatureMovementObserver
{
public:
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;

private:
	void onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange) override;
};

