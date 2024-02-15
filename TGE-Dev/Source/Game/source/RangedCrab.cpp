#include "Entity.h"
#include "RangedCrab.h"

void RangedCrab::Read(const nlohmann::json& someData)
{
	//myMovementSpeed = someData["MovementSpeed"];
	//myMaxJumpChargeTime = someData["MaxJumpChargeTime"];
	//myJumpChargeTimer = 0;
	//myMaxHP = 2;
	//myCurrentHP = myMaxHP;
	someData;
}

void RangedCrab::Start()
{
	CreatureInput::Start();
}

void RangedCrab::Update(float aDeltaTime)
{
	GetOnMoveHorizontal()(-1, aDeltaTime);
}
