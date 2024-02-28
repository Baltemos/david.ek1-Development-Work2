#include "Entity.h"
#include "Projectile.h"
#include "Tag.h"
#include "SpriteRenderer2D.h"
#include "Animator.h"

//void Projectile::SetDirection(int aDirection)
//{
//	myDirection = aDirection;
//	assert(myDirection == 1 || myDirection == -1);
//}

void Projectile::Read(const nlohmann::json& someData)
{
	//myVariable = someData["Variable"];

	CreatureInput::Read(someData);
	myMovementSpeed = someData["MovementSpeed"];
}

void Projectile::Start()
{
	CreatureInput::Start();
	//myDirection = *myEntityDirection;
	//assert(myDirection == 1 || myDirection == -1);
	//myTransform = GetEntity()->GetComponent<Transform>();
	//myPlayerTransform = Tag::FindFirst("Player")->GetEntity()->GetComponent<Transform>();
	// 
	//std::shared_ptr<Animator> animator = GetEntity()->GetComponent<Animator>();
	//if (animator)
	//{
	//	if (myDirection < 0)
	//	{
	//		animator->Begin("CrabWalkingLeft", false);
	//	}
	//	else
	//	{
	//		animator->Begin("CrabWalkingRight", false);
	//	}
	//}
}

void Projectile::Update(float aDeltaTime)
{
	aDeltaTime;
	GetOnMoveHorizontal()(*myEntityDirection, myMovementSpeed);
}
