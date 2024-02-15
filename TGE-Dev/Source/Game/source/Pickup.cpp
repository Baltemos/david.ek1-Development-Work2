#include "Pickup.h"
#include "Entity.h"
#include "Tag.h"

void Pickup::Start()
{
}

void Pickup::Read(const nlohmann::json& someData)
{
	someData;
}

void Pickup::Trigger(Collider& aCollider, cu::Vector2<float> aImpactPoint)
{
	aCollider.GetEntity()->GetComponent<Tag>();

	if (Tag::HasTag(aCollider.GetEntity(), "Player"))
	{
		GetEntity()->Destroy();
	}
	aImpactPoint; //Ta bort mig sen :)
}
