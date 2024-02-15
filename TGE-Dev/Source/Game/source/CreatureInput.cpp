#include "CreatureInput.h"

void CreatureInput::Start()
{

}

void CreatureInput::Read(const nlohmann::json& someData)
{
	myAttackRange = someData["AttackRange"];
	myAttackCooldown = someData["AttackCooldown"];
	myAttackTimer = 0;
	myEntityDirection = std::make_shared<int>(1);
}

Event<int, float>& CreatureInput::GetOnMoveHorizontal()
{
	return myOnMoveHorizontal;
}

Event<float, float>& CreatureInput::GetOnJump()
{
	return myOnJump;
}

Event<>& CreatureInput::GetOnLetGoOfJump()
{
	return myOnLetGoOfJump;
}

Event</*int, */float, float>& CreatureInput::GetOnAttack()
{
	return myOnAttack;
}

std::shared_ptr<int> CreatureInput::GetDirectionSharedPointer()
{
	return myEntityDirection;
}

void CreatureInput::SetDirectionSharedPointer(int aDirection)
{
	*myEntityDirection = aDirection;
}
