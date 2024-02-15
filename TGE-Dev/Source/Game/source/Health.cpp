#include "Entity.h"
#include "Health.h"
#include "Tag.h"

void Health::Read(const nlohmann::json& someData)
{
	myMaxHP = someData["MaxHP"];
	myCurrentHP = myMaxHP;
	//GetEntity()->GetComponent<CreatureInput>()->GetOnMoveHorizontal().Subscribe(this, &CreatureMovementObserver::onMoveHorizontal);
	GetEntity()->GetComponent<Collider>()->OnCollision.Subscribe(this, &Health::GetOnCollision);
}

bool Health::TakeDamage(const int someDamage) 
{
	myCurrentHP -= someDamage;
	if (myCurrentHP <= 0)
	{
		myIsAlive = false;
		GetEntity()->Destroy();
	}
	return myIsAlive;
}

void Health::GetOnCollision(const CollisionInfo2D& aInfo)
{
	//Lava flow >:D
	//aPoint;
	//std::string tag = aCollider.GetEntity()->GetComponent<Tag>()->GetTag();
	//if ((tag == "Enemy" && GetEntity()->GetComponent<Tag>()->GetTag() == "Player")
	//	|| (tag == "Player" && GetEntity()->GetComponent<Tag>()->GetTag() == "Enemy"))
	//{
	//	TakeDamage(1);
	//}

	std::shared_ptr<Entity> other = aInfo.OtherCollider->GetEntity();
	if (other != nullptr)
	{
		if (Tag::HasTag(other, "Hostile") && Tag::HasTag(GetEntity(), "Friendly")/* && Tag::HasTag(GetEntity(), "Player") == false*/ ||
			(Tag::HasTag(other, "Friendly") && Tag::HasTag(other, "Player") == false && Tag::HasTag(GetEntity(), "Hostile")))
		{
			TakeDamage(1);
		}
	}
}