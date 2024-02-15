#include "PlayerInput.h"
#include "GameWorld.h"
#include "Tag.h"

void PlayerInput::Start()
{
	CreatureInput::Start();
	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void PlayerInput::Read(const nlohmann::json& someData)
{
	CreatureInput::Read(someData);
	myMovementSpeed = someData["MovementSpeed"];
	myJumpSpeed = someData["JumpSpeed"];
	GetEntity()->GetComponent<Collider>()->OnCollision.Subscribe(this, &PlayerInput::GetOnCollision);
}

void PlayerInput::Update(float aDeltaTime)
{
	//int upDirection = static_cast<int>(myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::W)));
	int downDirection = static_cast<int>(myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::S)));
	int leftDirection = static_cast<int>(myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::A)));
	int rightDirection = static_cast<int>(myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::D)));

	int yDirection = /*upDirection*/ -downDirection;
	int xDirection = rightDirection - leftDirection;

	if (xDirection != 0)
	{
		GetOnMoveHorizontal()(xDirection, myMovementSpeed);
	}

	if (yDirection != 0)
	{
		GetOnLookVertical()(yDirection);
	}

	if (myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::SPACE)))
	{
		GetOnJump()(aDeltaTime, myJumpSpeed);
	}
	else if (myInputHandler->IsKeyUp(static_cast<int>(CommonUtilities::Keys::SPACE)))
	{
		GetOnLetGoOfJump()();
	}

	if (myInputHandler->IsKeyDown(static_cast<int>(CommonUtilities::Keys::CONTROL)))
	{
		GetOnAttack()(/*0,*/ aDeltaTime, myAttackRange);
	}
	if (myInputHandler->IsKeyDown(static_cast<int>(CommonUtilities::Keys::SHIFT)))
	{
		GetOnDash()(aDeltaTime);
	}
}

Event<float>& PlayerInput::GetOnDash()
{
	return myOnDash;
}

Event<int>& PlayerInput::GetOnLookVertical()
{
	return myOnLookVertical;
}

void PlayerInput::GetOnCollision(const CollisionInfo2D& aInfo)
{
	std::shared_ptr<Entity> other = aInfo.OtherCollider->GetEntity();

	if (other != nullptr)
	{
		if (Tag::HasTag(other, "DashPickup"))
		{
			myUnlockedDash = true;
		}
		if (Tag::HasTag(other, "DoubleJumpPickup"))
		{
			myUnlockedDoubleJump = true;
		}
	}
}
