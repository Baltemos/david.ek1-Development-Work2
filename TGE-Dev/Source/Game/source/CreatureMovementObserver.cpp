#include "CreatureMovementObserver.h"
#include "Entity.h"
#include "CreatureInput.h"
#include "Entity.h"
#include "GameWorld.h"

void CreatureMovementObserver::Read(const nlohmann::json& someData)
{
	myMaxJumpChargeTime = someData["MaxJumpChargeTime"];
	myJumpChargeTimer = 0;
	myTransform = GetEntity()->GetComponent<Transform>();
}

void CreatureMovementObserver::Start()
{
	myIsGrounded = false;
	myIsGroundedCheck = false;

	//myTransform = GetEntity()->GetComponent<Transform>();
	myPhysicbody = GetEntity()->GetComponent<Physicbody>();
	GetEntity()->GetComponent<CreatureInput>()->GetOnMoveHorizontal().Subscribe(this, &CreatureMovementObserver::onMoveHorizontal);
	GetEntity()->GetComponent<CreatureInput>()->GetOnJump().Subscribe(this, &CreatureMovementObserver::onJump);
	GetEntity()->GetComponent<CreatureInput>()->GetOnLetGoOfJump().Subscribe(this, &CreatureMovementObserver::onLetGoOfJump);
	GetEntity()->GetComponent<CreatureInput>()->GetOnAttack().Subscribe(this, &CreatureMovementObserver::onAttack);
	myEntityDirection = GetEntity()->GetComponent<CreatureInput>()->GetDirectionSharedPointer();


	for (auto& collider : GetEntity()->GetComponents<Collider>())
	{
		collider->OnCollision.Subscribe(this, &CreatureMovementObserver::onCollision);
	}
}

void CreatureMovementObserver::Update(float)
{
	myIsGrounded = myIsGroundedCheck;
	myIsGroundedCheck = false;
}

bool CreatureMovementObserver::GetGrounded() const
{
	return myIsGrounded;
}

void CreatureMovementObserver::onCollision(const CollisionInfo2D& aInfo)
{
	if (!aInfo.OtherCollider->IsTrigger() && aInfo.Normal.y > 0)
	{
		myIsGroundedCheck = true;
	}
}

void CreatureMovementObserver::onMoveHorizontal(int aDirection, float aSpeed)
{
	std::shared_ptr<Physicbody> body = myPhysicbody.lock();
	if (body->GetIsDashing())
	{
		return;
	}
	body->SetVelocity(CommonUtilities::Vector2<float>(aDirection * aSpeed, body->GetVelocity().y));
	GetEntity()->GetComponent<CreatureInput>()->SetDirectionSharedPointer(aDirection);
	//std::cout << "[CreatureMovementObserver]: Moved = " << aDirection << std::endl;
}

void CreatureMovementObserver::onJump(float aDeltaTime, float aSpeed)
{
	aDeltaTime;
	aSpeed;
	if (myIsGrounded)
	{
		myJumpChargeTimer = 0;
	}
	if (myJumpChargeTimer < myMaxJumpChargeTime)
	{
		myJumpChargeTimer += aDeltaTime;
		std::shared_ptr<Physicbody> body = myPhysicbody.lock();
		body->SetVelocity(CommonUtilities::Vector2<float>(body->GetVelocity().x, aSpeed));
	}
}

void CreatureMovementObserver::onLetGoOfJump()
{
	myJumpChargeTimer = myMaxJumpChargeTime;
}

void CreatureMovementObserver::onAttack(/*int aDirection, */ float aDeltaTime, float anAttackRange)
{
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * aDeltaTime, anAttackRange /** aDirection*/, 0));//Lava flow. Make pure virtual?
}
