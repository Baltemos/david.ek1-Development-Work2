#include "Wind.h"
#include "Entity.h"
#include "Tag.h"
#include "Physicbody.h"
void Wind::Start()
{
	myCollider = GetEntity()->GetComponent<Collider>();
	if (myCollider.lock() == nullptr)
	{
		MessageBox(NULL, L"WINDBOX assigned to object without Collider.", L"Error", MB_ICONERROR);
	}
	myCollider.lock()->OnCollision.Subscribe(this, &Wind::Push);
}

void Wind::Read(const nlohmann::json& someData)
{
	try 
	{
		myDirection.x = someData["Direction"]["x"];
		myDirection.y = someData["Direction"]["y"];
		myIntesity = someData["Intensity"];
		
	}
	catch (const std::exception&)
	{

	}
	
}

void Wind::Push(const CollisionInfo2D& aInfo)
{
	std::shared_ptr<Entity> entity = aInfo.OtherCollider->GetEntity();

	if (Tag::HasTag(entity, "Player"))
	{
		entity->GetComponent<Physicbody>()->AddVelocity(myDirection*myIntesity);
	}
}
