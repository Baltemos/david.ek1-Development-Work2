#include "MeleeEnemyObserver.h"
#include "Entity.h"
#include "CreatureInput.h"
#include "Entity.h"
#include "GameWorld.h"

void MeleeEnemyObserver::Read(const nlohmann::json& someData)
{
	CreatureMovementObserver::Read(someData);
}

void MeleeEnemyObserver::Start()
{
	CreatureMovementObserver::Start();
	//GetEntity()->GetComponent<CreatureInput>()->GetOnAttack().Subscribe(this, &MeleeEnemyObserve::onAttack);
}

void MeleeEnemyObserver::onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange)
{
	aDeltaTime; //Lava flow
	if (myAttackTimer > myAttackCooldown)
	{
		GameWorld* gameWorld = GameWorld::GetInstance();
		EntityTemplate attackObject("Entities/BasicCrabAttack.json");
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
		}
		attackTransform->SetParent(transform, false);
		attackTransform->SetLocalPosition(CommonUtilities::Vector3<float>((anAttackRange - 0.5f * attack->GetComponent<SpriteRenderer2D>()->GetInstance().mySize.x) * *myEntityDirection, 0, 0));
		attack->GetComponent<Collider>()->SetTrigger(true);
		myAttackTimer = 0;
	}
}
