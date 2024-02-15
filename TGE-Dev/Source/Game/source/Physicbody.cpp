#include "Physicbody.h"
#include "Entity.h"

void Physicbody::SetVelocity(const cu::Vector2<float>& aVelocity)
{
	myVelocity = aVelocity;
}

void Physicbody::AddVelocity(const cu::Vector2<float>& aVelocity)
{
	myVelocity += aVelocity;
}

void Physicbody::AddVelocity(const cu::Vector3<float>& aVelocity)
{
	myVelocity += aVelocity.ConvertToVector2XY();
}

const cu::Vector2<float>& Physicbody::GetVelocity() const
{
	return myVelocity;
}

const bool Physicbody::GetIsDashing() const
{
	return myDashing;
}

void Physicbody::SetIsDashing(bool aDashing)
{
	myDashing = aDashing;
	if (myDashing)
	{
		//HACK
		myDashDuration = .5f;
	}
}

void Physicbody::Read(const nlohmann::json& someData)
{
	auto& velocity = someData["Velocity"];
	myVelocity.x = velocity[0];
	myVelocity.y = velocity[1];

	auto& gravity = someData["Gravity"];
	myGravity.x = gravity[0];
	myGravity.y = gravity[1];

	auto& drag = someData["Drag"];
	myDrag.x = drag[0];
	myDrag.y = drag[1];

	myDashing = false;

	myTransform = GetEntity()->GetComponent<Transform>();

	for (auto& collider : GetEntity()->GetComponents<Collider>())
	{
		collider->OnCollision.Subscribe(this, &Physicbody::onCollision);
	}
}

void Physicbody::Update(float aDeltaTime)
{
	if (!myDashing)
	{
		myVelocity += myGravity * aDeltaTime;

		cu::Vector2<float> dragDelta = myDrag * aDeltaTime * aDeltaTime;
		dragDelta.x = (std::min)(dragDelta.x, 1.0f);
		dragDelta.y = (std::min)(dragDelta.y, 1.0f);

		myVelocity -= myVelocity * dragDelta;
	}
	else
	{
		myDashDuration -= aDeltaTime;
		myVelocity.y = 0;
		if (myDashDuration <= 0)
		{
			myDashing = false;
		}
	}

	myTransform.lock()->MoveLocalPosition(cu::Vector3<float>(myVelocity * aDeltaTime, 0));
}

void Physicbody::onCollision(const CollisionInfo2D& aInfo)
{
	if (aInfo.Intersects && !aInfo.OurCollider->IsTrigger() && !aInfo.OtherCollider->IsTrigger())
	{
		myTransform.lock()->MoveWorldPosition(cu::Vector3<float>(aInfo.Normal, 0));
		if (aInfo.Normal.x > 0 && myVelocity.x < 0 || aInfo.Normal.x < 0 && myVelocity.x > 0)
		{
			myVelocity.x = 0;
		}
		if (aInfo.Normal.y > 0 && myVelocity.y < 0 || aInfo.Normal.y < 0 && myVelocity.y > 0)
		{
			myVelocity.y = 0;
		}
		aInfo.OurCollider->UpdateBounds();
	}
}
