#include "RangedEnemyObserver.h"
#include "Entity.h"
#include "CreatureInput.h"
#include "Entity.h"
#include "GameWorld.h"
#include "Projectile.h"

void RangedEnemyObserver::Read(const nlohmann::json& someData)
{
	CreatureMovementObserver::Read(someData);
}

void RangedEnemyObserver::Start()
{
	CreatureMovementObserver::Start();
	//GetEntity()->GetComponent<CreatureInput>()->GetOnAttack().Subscribe(this, &RangedEnemyObserve::onAttack);
}

void RangedEnemyObserver::onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange)
{
	aDeltaTime; //Lava flow
	if (myAttackTimer > myAttackCooldown)
	{
		GameWorld* gameWorld = GameWorld::GetInstance();
		EntityTemplate attackObject("Entities/RangedCrabAttack.json");
		std::shared_ptr<Transform> transform = myTransform.lock();
		CommonUtilities::Vector3<float> position = transform->GetWorldPosition();
		//attackObject.AddComponent("Transform");
		//attackObject.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/t_playerPlaceholder_2.png" } });
		//attackObject.AddComponent("RectangleCollider");
		//attackObject.AddComponent("ExpirationTimer");
		//attackObject.AddComponent("Tag", { {"Tag", "Attack"} });
		//attackObject.AddComponent("Tag", { {"Tag", "Hostile"} });
		std::shared_ptr<Entity> attack = gameWorld->GetEntityManager().Add(attackObject, gameWorld->GetComponentRegistry());
		std::shared_ptr<Transform> attackTransform = attack->GetComponent<Transform>();
		if (*myEntityDirection < 0)
		{
			attack->GetComponent<SpriteRenderer2D>()->SetFlippedX(true);
			attack->GetComponent<Projectile>()->SetDirectionSharedPointer(-1);
		}
		attackTransform->SetParent(transform, false);
		attackTransform->SetLocalPosition(CommonUtilities::Vector3<float>((anAttackRange - 0.5f * attack->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.x) * *myEntityDirection, 1.f * attack->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.y, 0));
		attack->GetComponent<Collider>()->SetTrigger(true);
		myAttackTimer = 0;
	}
}
