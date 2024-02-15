#include "RangedEnemyObserver.h"
#include "Entity.h"
#include "CreatureInput.h"

void RangedEnemyObserver::Read(const nlohmann::json& someData)
{
	CreatureMovementObserver::Read(someData);
}

void RangedEnemyObserver::Start()
{
	//GetEntity()->GetComponent<CreatureInput>()->GetOnAttack().Subscribe(this, &MeleeEnemyObserve::onAttack);
}

void RangedEnemyObserver::onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange)
{
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * aDeltaTime, anAttackRange, 0));
}
