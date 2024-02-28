#include "Entity.h"
#include "RangedCrab.h"
#include "Tag.h"
#include "SpriteRenderer2D.h"
#include "Animator.h"

void RangedCrab::Read(const nlohmann::json& someData)
{
	//myVariable = someData["Variable"];

	CreatureInput::Read(someData);
	myMovementSpeed = someData["MovementSpeed"];
}

void RangedCrab::Start()
{
	CreatureInput::Start();
	myTransform = GetEntity()->GetComponent<Transform>();
	myPlayerTransform = Tag::FindFirst("Player")->GetEntity()->GetComponent<Transform>();
}

void RangedCrab::Update(float aDeltaTime)
{
	std::shared_ptr<Transform> crabTransform = myTransform.lock();

	CommonUtilities::Vector3<float> distanceDifference = myPlayerTransform->GetWorldPosition() - crabTransform->GetWorldPosition();
	//CommonUtilities::Vector2<float> halfSizeSum = CommonUtilities::Vector2<float>(myPlayerTransform->GetEntity()->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.x + GetEntity()->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.x, myPlayerTransform->GetEntity()->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.y + GetEntity()->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.y) * 0.5f;

	if (fabs(distanceDifference.Length()) < myAttackRange)
	{
		//if (fabs(distanceDifference.x) </* halfSizeSum.x*/ +myAttackRange
		if ((distanceDifference.x < 0 && *myEntityDirection < 0) || (distanceDifference.x > 0 && *myEntityDirection > 0))
		//	&& fabs(distanceDifference.y) </* halfSizeSum.y +*/ myAttackRange)
		{
			bool attacking = true;
			//ANIMATION TEST CODE

			std::shared_ptr<Animator> animator = GetEntity()->GetComponent<Animator>();
			if (animator)
			{
				if (myPlayerTransform->GetWorldPosition().x > crabTransform->GetWorldPosition().x)
				{
					attacking = animator->Begin("CrabAttackRight", true);
				}
				else
				{
					attacking = animator->Begin("CrabAttackLeft", true);
				}
			}
			//---------

			if (attacking)
			{
				GetOnAttack()(/*direction, */aDeltaTime, /*myAttackRange*/0);
			}
		}
		else
		{
			*myEntityDirection *= -1;
		}
		//else if (fabs(myPlayerTransform->GetWorldPosition().x - crabTransform->GetWorldPosition().x) > myMovementSpeed * aDeltaTime * 2)
		//{
		//	int direction = -1;
		//	if (myPlayerTransform->GetWorldPosition().x > crabTransform->GetWorldPosition().x)
		//	{
		//		direction = 1;
		//	}

		//	bool walking = true;

		//	//ANIMATION TEST CODE

		//	std::shared_ptr<Animator> animator = GetEntity()->GetComponent<Animator>();
		//	if (animator)
		//	{
		//		if (direction < 0)
		//		{
		//			walking = animator->Begin("CrabWalkingLeft", false);
		//		}
		//		else
		//		{
		//			walking = animator->Begin("CrabWalkingRight", false);
		//		}
		//	}

		//	//---------

		//	if (walking)
		//	{
		//		GetOnMoveHorizontal()(direction, myMovementSpeed);
		//	}
		//}
	}
	//ANIMATION TEST CODE
	else
	{
		std::shared_ptr<Animator> animator = GetEntity()->GetComponent<Animator>();
		if (animator)
		{
			animator->Begin("CrabIdle", false);
		}
	}
	//---------
}
