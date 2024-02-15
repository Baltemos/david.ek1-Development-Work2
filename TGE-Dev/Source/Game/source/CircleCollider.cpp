#include "CircleCollider.h"
#include "RectangleCollider.h"
#include <CommonUtilities/Intersection.hpp>
#include <tge/sprite/sprite.h>
#include "GameWorld.h"

void CircleCollider::UpdateBounds()
{
	cu::Vector2<float> offset = myTransform.lock()->GetWorldPosition().ConvertToVector2XY();
	myWorldBounds = cu::Circle<float>(myBounds.GetCenter() + offset, myBounds.GetRadius());

	SetBoundingCircle(myWorldBounds);
}

void CircleCollider::SetBounds(cu::Circle<float> aBounds)
{
	myBounds = aBounds;
}

const cu::Circle<float>& CircleCollider::GetWorldBounds() const
{
	return myWorldBounds;
}

void CircleCollider::Read(const nlohmann::json& someData)
{
	myTransform = GetEntity()->GetComponent<Transform>();

	SetType(ColliderType::eCircle);

	auto& bounds = someData["Bounds"];
	auto& offset = bounds["Offset"];
	myBounds = cu::Circle<float>(cu::Vector2<float>(offset[0], offset[1]), bounds["Radius"]);

	Collider::Read(someData);
}

void CircleCollider::Update(float)
{
	UpdateBounds();

#ifdef _DEBUG
	constexpr int repetitions = 8;
	constexpr float increment = 3.142f * 0.5f / static_cast<float>(repetitions);

	Tga::Sprite2DInstanceData instance;
	instance.myPosition = Tga::Vector2f(GetWorldBounds().GetCenter());
	instance.myPivot = { 0.5, 0.5 };
	instance.myColor = { 1, 0, 0, 0.5f };

	float diameter = 2.f * GetWorldBounds().GetRadius();
	for (int ind = 0; ind < repetitions; ind++)
	{
		float v = static_cast<float>(ind) * increment;
		float x = std::cos(v);
		float y = std::sin(v);

		instance.mySize = Tga::Vector2f(diameter * x, diameter * y);
		GameWorld::GetInstance()->GetRenderBuffer().Push({}, instance, 100);
	}
#endif // _DEBUG

}
