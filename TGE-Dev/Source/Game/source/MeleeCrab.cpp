#include "Entity.h"
#include "MeleeCrab.h"
#include "Tag.h"

void MeleeCrab::Read(const nlohmann::json& someData)
{
	//myVariable = someData["Variable"];
	someData;


	CreatureInput::Read(someData);
	myMovementSpeed = someData["MovementSpeed"];
}

void MeleeCrab::Start()
{
	CreatureInput::Start();
	myTransform = GetEntity()->GetComponent<Transform>();
	myPlayerTransform = Tag::FindFirst("Player")->GetEntity()->GetComponent<Transform>();
}

void MeleeCrab::Update(float aDeltaTime)
{
	aDeltaTime; //lava flow
	std::shared_ptr<Transform> crabTransform = myTransform.lock();

	float xDifference = myPlayerTransform->GetWorldPosition().x - crabTransform->GetWorldPosition().x;
	if (fabs(xDifference) < myAttackRange
		&& ((xDifference < 0 && *myEntityDirection < 0) || (xDifference > 0 && *myEntityDirection > 0)))
	{
		GetOnAttack()(/*direction, */aDeltaTime, myAttackRange);
	}
	else if (fabs(myPlayerTransform->GetWorldPosition().x - crabTransform->GetWorldPosition().x) > myMovementSpeed * aDeltaTime * 2)
	{
		int direction = -1;
		if (myPlayerTransform->GetWorldPosition().x > crabTransform->GetWorldPosition().x)
		{
			direction = 1;
		}
		GetOnMoveHorizontal()(direction, myMovementSpeed);
	}
}
