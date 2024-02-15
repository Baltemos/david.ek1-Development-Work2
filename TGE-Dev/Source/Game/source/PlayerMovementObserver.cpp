#include "PlayerMovementObserver.h"
#include "CreatureInput.h"
#include "PlayerInput.h"
#include "Entity.h"
#include "GameWorld.h"

void PlayerMovementObserver::Read(const nlohmann::json& someData)
{
	CreatureMovementObserver::Read(someData);
	myDashSpeed = someData["DashSpeed"];
	MyDashCooldown = someData["DashCooldown"];
}

void PlayerMovementObserver::Start()
{
	CreatureMovementObserver::Start();
	GetEntity()->GetComponent<PlayerInput>()->GetOnDash().Subscribe(this, &PlayerMovementObserver::onDash);
	GetEntity()->GetComponent<PlayerInput>()->GetOnLookVertical().Subscribe(this, &PlayerMovementObserver::onLookVertical);
}

void PlayerMovementObserver::onDash(float aDeltaTime)
{
	aDeltaTime;
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * myDashSpeed, 0, 0));

	//std::cout << "[PlayerMovementObserver]: Moved = " << aDirection << std::endl;
}

void PlayerMovementObserver::onLookVertical(int aDirection)
{
	myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(0, static_cast<float>(aDirection), 0));
}

void PlayerMovementObserver::onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange)
{
	//Poltergeist and copy paste... but needed because of the component system?
	GameWorld* gameWorld = GameWorld::GetInstance();
	EntityTemplate2 attackObject;
	std::shared_ptr<Transform> transform = myTransform.lock();
	CommonUtilities::Vector3<float> position = transform->GetWorldPosition();
	attackObject.AddComponent("Transform");
	attackObject.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/t_playerPlaceholder_2.png" } });
	attackObject.AddComponent("RectangleCollider");
	attackObject.AddComponent("ExpirationTimer");
	attackObject.AddComponent("Tag", { {"Tag", "Attack"} });
	attackObject.AddComponent("Tag", { {"Tag", "Friendly"} });
	std::shared_ptr<Entity> attack = gameWorld->GetEntityManager().Add(attackObject, gameWorld->GetComponentRegistry());
	std::shared_ptr<Transform> attackTransform = attack->GetComponent<Transform>();
	attackTransform->SetParent(transform.get(), false);
	float debug = anAttackRange * 0.5f * *myEntityDirection;
	debug;
	attackTransform->SetLocalPosition(CommonUtilities::Vector3<float>(anAttackRange * 0.5f * *myEntityDirection, 0, 0));
	attack->GetComponent<Collider>()->SetTrigger(true);
	aDeltaTime;
	//myPhysicbody.lock()->AddVelocity(CommonUtilities::Vector3<float>(*myEntityDirection * aDeltaTime, 0, 0));
}
