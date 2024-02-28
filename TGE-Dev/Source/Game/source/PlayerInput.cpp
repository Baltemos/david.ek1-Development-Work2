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
	myMaxJumps = 1; //set to 1
	myUnlockedDash = false; //set to false
	myMovementSpeed = someData["MovementSpeed"];
	myJumpSpeed = someData["JumpSpeed"];
	GetEntity()->GetComponent<Collider>()->OnCollision.Subscribe(this, &PlayerInput::GetOnCollision);
	myShouldResetJumps = false;
	myHasKey = false;
	myMapToLoad = "";
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

	if (myInputHandler->IsKeyDown(static_cast<int>(CommonUtilities::Keys::SPACE)))
	{
  		//myJumpsLeft--;
	}
	if (myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::SPACE)) && myJumpsLeft > 0)
	{
		GetOnJump()(aDeltaTime, myJumpSpeed);
	}
	else if (myInputHandler->IsKeyUp(static_cast<int>(CommonUtilities::Keys::SPACE)))
	{
		myJumpsLeft--;
		GetOnLetGoOfJump()();
		//if (myShouldResetJumps)
		//{
		//	myJumpsLeft = myMaxJumps;
		//}
	}

	if (myInputHandler->IsKeyDown(static_cast<int>(CommonUtilities::Keys::CONTROL)))
	{
		GetOnAttack()(/*0,*/ aDeltaTime, myAttackRange);
	}
	if (myInputHandler->IsKeyDown(static_cast<int>(CommonUtilities::Keys::SHIFT)) && myUnlockedDash)
	{
		GetOnDash()(aDeltaTime);
	}

	if (myMapToLoad != "")
	{
		GameWorld::GetInstance()->LoadScene(myMapToLoad);
		myMapToLoad = "";
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

void PlayerInput::ResetJumpsLeft()
{
	//if (myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::SPACE)))
	//{
	//	myShouldResetJumps = true;
	//}
	//else
	//{
		myJumpsLeft = myMaxJumps;
		if (myInputHandler->IsKeyPressed(static_cast<int>(CommonUtilities::Keys::SPACE)))
		{
			myJumpsLeft++;
		}
	//}
}

void PlayerInput::ReduceJumpsLeft()
{
	myJumpsLeft--;
}

bool PlayerInput::GetHasKey() const
{
	return myHasKey;
}

//std::shared_ptr<int> PlayerInput::AccessJumpsLeft()
//{
//	return myJumpsLeft;
//}

void PlayerInput::GetOnCollision(const CollisionInfo2D& aInfo)
{
	std::shared_ptr<Entity> other = aInfo.OtherCollider->GetEntity();

	if (other != nullptr)
	{
		if (Tag::HasTag(other, "DashPickup"))
		{
			myUnlockedDash = true;
			other->Destroy();
		}
		if (Tag::HasTag(other, "DoubleJumpPickup"))
		{
			myMaxJumps = 2;
			other->Destroy();
			//myUnlockedDoubleJump = true;
		}
		if (Tag::HasTag(other, "Key"))
		{
			myHasKey = true;
			other->Destroy();
			//GetEntity()->AddComponent("Tag", {{"Tag", "Attack"}});
			//myUnlockedDoubleJump = true;
		}
		if (Tag::HasTag(other, "Door"))
		{
			if (myHasKey)
			{
				other->Destroy();
			}
			//GetEntity()->AddComponent("Tag", {{"Tag", "Attack"}});
			//myUnlockedDoubleJump = true;
		}
		if (Tag::HasTag(other, "GoalLevel1"))
		{
			//Add UI here
			myMapToLoad = "Scenes/Dexter.json";
		}
	}
}
