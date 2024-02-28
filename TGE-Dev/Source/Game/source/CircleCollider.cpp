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
	static constexpr const size_t pointCount = 16;
	static constexpr const float TwoPi = 6.28318530718f;
	static constexpr float angleDelta = TwoPi / static_cast<float>(pointCount);

	Tga::Vector4f color = { 1.f, 0.f, 0.f, 1.f };

	float radius = GetWorldBounds().GetRadius();
	cu::Vector3<float> center = cu::Vector3<float>(GetWorldBounds().GetCenter(), 0);
	Tga::Vector3f lastPos = Tga::Vector3f(cu::Vector3<float>(radius, 0.f, 0.f) + center);

	float angle = 0.f;
	for (size_t ind = 0; ind < pointCount; ind++)
	{
		angle += angleDelta;
		Tga::LinePrimitive line;
		line.color = color;
		line.fromPosition = lastPos;
		lastPos = Tga::Vector3f(cu::Vector3<float>(std::cos(angle), std::sin(angle), 0.f) * radius + center);
		line.toPosition = lastPos;

		myTransform.lock()->GetSpaceRenderBuffer()->Push(line, 1000);
	}
#endif // _DEBUG

}
