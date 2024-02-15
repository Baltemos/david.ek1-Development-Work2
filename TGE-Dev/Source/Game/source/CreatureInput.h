#pragma once
#include "Component.h"
#include "Event.h"
class CreatureInput : public Component
{
public:
	Event<int, float>& GetOnMoveHorizontal();
	Event<float, float>& GetOnJump();
	Event<>& GetOnLetGoOfJump();
	Event</*int, */float, float>& GetOnAttack();
	std::shared_ptr<int> GetDirectionSharedPointer();
	void SetDirectionSharedPointer(int aDirection);

protected:
	virtual void Start() override;
	virtual void Read(const nlohmann::json& someData) override;
	std::shared_ptr<int> myEntityDirection;
	float myMovementSpeed;
	float myJumpSpeed;
	float myAttackRange;
	float myAttackCooldown;
	float myAttackTimer;

private:
	Event<int, float> myOnMoveHorizontal;
	Event<float, float> myOnJump;
	Event<> myOnLetGoOfJump;
	Event</*int, */float, float> myOnAttack;
};