#include "PlayerMovementObserver.h"
#include "CreatureInput.h"
#include "PlayerInput.h"
#include "Entity.h"
#include "GameWorld.h"
#include "PlayerInput.h"

void PlayerMovementObserver::Read(const nlohmann::json& someData)
{
	CreatureMovementObserver::Read(someData);
	myDashSpeed = someData["DashSpeed"];
	MyDashCooldown = someData["DashCooldown"];
}

void PlayerMovementObserver::Start()
{
	CreatureMovementObserver::Start();
	//myPlayerJumpsLeft = GetEntity()->GetComponent<PlayerInput>()->AccessJumpsLeft();
	GetEntity()->GetComponent<PlayerInput>()->GetOnDash().Subscribe(this, &PlayerMovementObserver::onDash);
	GetEntity()->GetComponent<PlayerInput>()->GetOnLookVertical().Subscribe(this, &PlayerMovementObserver::onLookVertical);

}
void PlayerMovementObserver::onJump(float aDeltaTime, float aSpeed)
{
	/*if (myIsGrounded)
	{
		myJumpChargeTimer = 0;
	}*/
	if (myJumpChargeTimer < myMaxJumpChargeTime)
	{
		myJumpChargeTimer += aDeltaTime;
		std::shared_ptr<Physicbody> body = myPhysicbody.lock();
		body->SetVelocity(CommonUtilities::Vector2<float>(body->GetVelocity().x, aSpeed));
	}
}

void PlayerMovementObserver::onLetGoOfJump()
{
	myJumpChargeTimer = 0;
}

void PlayerMovementObserver::onDash(float aDeltaTime)
{
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * myDashSpeed / aDeltaTime, 0, 0));

	//std::cout << "[PlayerMovementObserver]: Moved = " << aDirection << std::endl;
}

void PlayerMovementObserver::onLookVertical(int aDirection)
{
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(0, static_cast<float>(aDirection), 0));
}

void PlayerMovementObserver::onCollision(const CollisionInfo2D& aInfo)
{
	CreatureMovementObserver::onCollision(aInfo);
	if (myIsGroundedCheck)
	{
		GetEntity()->GetComponent<PlayerInput>()->ResetJumpsLeft();
	}
}

//void PlayerMovementObserver::Update(float aTimedelta)
//{
//	CreatureMovementObserver::Update(aTimedelta);
//	if(m)
//}

void PlayerMovementObserver::onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange)
{
	//Poltergeist and copy paste... but needed because of the component system?
	if (myAttackTimer > myAttackCooldown)
	{
		GameWorld* gameWorld = GameWorld::GetInstance();
		EntityTemplate attackObject("Entities/PlayerAttack.json");
		std::shared_ptr<Transform> transform = myTransform.lock();
		CommonUtilities::Vector3<float> position = transform->GetWorldPosition();
		//attackObject.AddComponent("Transform");
		//attackObject.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/t_playerPlaceholder_2.png" } });
		//attackObject.AddComponent("RectangleCollider");
		//attackObject.AddComponent("ExpirationTimer");
		//attackObject.AddComponent("Tag", { {"Tag", "Attack"} });
		//attackObject.AddComponent("Tag", { {"Tag", "Friendly"} });
		std::shared_ptr<Entity> attack = gameWorld->GetEntityManager().Add(attackObject, gameWorld->GetComponentRegistry());
		std::shared_ptr<Transform> attackTransform = attack->GetComponent<Transform>();
		if (*myEntityDirection < 0)
		{
			attack->GetComponent<SpriteRenderer2D>()->SetFlippedX(true);
		}
		attackTransform->SetParent(transform, false);
		attackTransform->SetLocalPosition(CommonUtilities::Vector3<float>((anAttackRange - 0.5f * attack->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.x) * *myEntityDirection, 0, 0));
		attack->GetComponent<Collider>()->SetTrigger(true);
		aDeltaTime;
		//myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * aDeltaTime, 0, 0));
		myAttackTimer = 0;
	}
}
