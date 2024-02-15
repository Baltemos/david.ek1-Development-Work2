#include "Collider2D.h"
#include "QuadTree.h"
#include <set>
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include <CommonUtilities/Intersection.hpp>

static std::vector<Collider*> locColliders;
static std::vector<Collider*> locStaticColliders;

void Collider::CheckCollision(Collider* aCollider1, Collider* aCollide2)
{
	if (aCollider1->PreliminaryCheck(aCollide2))
	{
		CollisionInfo2D info = aCollider1->Intersects(aCollide2);
		if (info.Intersects)
		{
			aCollider1->OnCollision(info);
			std::swap(info.OurCollider, info.OtherCollider);
			info.Normal = -info.Normal;
			aCollide2->OnCollision(info);
		}
	}
}

void Collider::RunCollisions()
{
	locColliders.erase(
		std::remove_if(
			locColliders.begin(),
			locColliders.end(),
			[](Collider* collider) {
				return collider->IsDestroyed();
			}
		),
		locColliders.end()
	);

	locStaticColliders.erase(
		std::remove_if(
			locStaticColliders.begin(),
			locStaticColliders.end(),
			[](Collider* collider) {
				return collider->IsDestroyed();
			}
		),
		locStaticColliders.end()
	);

	Collider* collider1, * collider2;
	auto it1 = locColliders.begin();
	while (it1 < locColliders.end())
	{
		//Move static colliders to static list. Previous dynamic colliders will have checked with this one before this, so the move doesn't result in missed collisions.
		if ((**it1).IsStatic())
		{
			locStaticColliders.push_back(*it1);
			it1 = locColliders.erase(it1);
			continue;
		}

		auto it2 = std::next(it1);
		collider1 = *it1;
		while (it2 < locColliders.end())
		{
			collider2 = *it2;
			CheckCollision(collider1, collider2);
			++it2;
		}

		auto statIt = locStaticColliders.begin();
		while (statIt < locStaticColliders.end())
		{
			//Move non-static colliders to dynamic list. It is moved to end of list and previus dynamic colliders will have checked this one, so no missed collisions.
			if ((**statIt).IsStatic() == false)
			{
				locColliders.push_back(*statIt);
				statIt = locStaticColliders.erase(statIt);
				continue;
			}

			collider2 = *statIt;
			CheckCollision(collider1, collider2);

			++statIt;
		}

		++it1;
	}
}

void Collider::SetStatic(bool aIsStatic)
{
	myIsStatic = aIsStatic;
}

void Collider::SetTrigger(bool aIsStatic)
{
	myIsTrigger = aIsStatic;
}

bool Collider::IsStatic() const
{
	return myIsStatic;
}

bool Collider::IsTrigger() const
{
	return myIsTrigger;
}

#define ColliderHash(type1, type2) (static_cast<int>(type1) | (static_cast<int>(type2) << 16))

CollisionInfo2D Collider::Intersects(Collider* aCollider)
{
	CollisionInfo2D info;
	info.OurCollider = this;
	info.OtherCollider = aCollider;

	bool swapped = false;

	ColliderType ourType = info.OurCollider->GetType();
	ColliderType otherType = info.OtherCollider->GetType();

	switch (ColliderHash(ourType, otherType))
	{
	case ColliderHash(ColliderType::eRectangle, ColliderType::eRectangle):
	{
		RectangleCollider* ourRect = dynamic_cast<RectangleCollider*>(info.OurCollider);
		RectangleCollider* otherRect = dynamic_cast<RectangleCollider*>(info.OtherCollider);

		cu::AABB2D<float> bounds = ourRect->GetWorldBounds();
		cu::AABB2D<float> obounds = otherRect->GetWorldBounds();

		info.Intersects = bounds.Intersects(obounds);

		if (info.Intersects)
		{
			cu::Vector2<float> move = obounds.Max - bounds.Min;
			cu::Vector2<float> negMove = obounds.Min - bounds.Max;
			if (std::abs(move.x) > std::abs(negMove.x))
				move.x = negMove.x;
			if (std::abs(move.y) > std::abs(negMove.y))
				move.y = negMove.y;

			if (std::abs(move.y) > std::abs(move.x))
				move.y = 0;
			else
				move.x = 0;

			info.Normal = move;
		}
		break;
	}
	case ColliderHash(ColliderType::eCircle, ColliderType::eCircle):
	{
		CircleCollider* ourCircle = dynamic_cast<CircleCollider*>(info.OurCollider);
		CircleCollider* otherCircle = dynamic_cast<CircleCollider*>(info.OtherCollider);

		cu::Circle<float> bounds = ourCircle->GetWorldBounds();
		cu::Circle<float> obounds = otherCircle->GetWorldBounds();

		cu::Vector2<float> diff = bounds.GetCenter() - obounds.GetCenter();
		float lengthSqr = diff.LengthSqr();
		float radiusSum = bounds.GetRadius() + obounds.GetRadius();

		info.Intersects = lengthSqr <= radiusSum * radiusSum;

		if (info.Intersects)
		{
			float len = std::sqrt(lengthSqr);
			info.Normal = diff * radiusSum / len - diff;
		}
		break;
	}

	case ColliderHash(ColliderType::eRectangle, ColliderType::eCircle):
		std::swap(info.OurCollider, info.OtherCollider);
		swapped = true;
		[[fallthrough]];
	case ColliderHash(ColliderType::eCircle, ColliderType::eRectangle):
	{
		CircleCollider* ourCircle = dynamic_cast<CircleCollider*>(info.OurCollider);
		RectangleCollider* otherRect = dynamic_cast<RectangleCollider*>(info.OtherCollider);

		cu::Circle<float> bounds = ourCircle->GetWorldBounds();
		cu::AABB2D<float> obounds = otherRect->GetWorldBounds();

		cu::Vector2<float> intersectionPoint;

		info.Intersects = cu::IntersectionCircleAABB2D(bounds, obounds, intersectionPoint);

		if (info.Intersects)
		{
			info.Normal = bounds.GetCenter() - intersectionPoint;
			float length = info.Normal.Length();
			info.Normal /= length;
			info.Normal *= bounds.GetRadius() - length;
		}
		break;
	}
	default:
		break;
	}

	if (swapped)
	{
		std::swap(info.OurCollider, info.OtherCollider);
		info.Normal = -info.Normal;
	}

	return info;
}

void Collider::Read(const nlohmann::json& someData)
{
	myIsStatic = someData["IsStatic"];
	myIsTrigger = someData["IsTrigger"];
	//locColliderTree.Emplace(this);
}

void Collider::Start()
{
	locColliders.push_back(this);
}

void Collider::SetType(ColliderType aType)
{
	myType = aType;
}

ColliderType Collider::GetType() const
{
	return myType;
}

void Collider::SetBoundingCircle(cu::Circle<float> aBoundingCircle)
{
	myBoundingCircle = aBoundingCircle;
}

cu::Circle<float> Collider::GetBoundingCircle() const
{
	return myBoundingCircle;
}

bool Collider::PreliminaryCheck(Collider* aCollider) const
{
	float distanceSqr = myBoundingCircle.GetRadius() + aCollider->myBoundingCircle.GetRadius();
	distanceSqr *= distanceSqr;
	return (myBoundingCircle.GetCenter() - aCollider->myBoundingCircle.GetCenter()).LengthSqr() <= distanceSqr;
}
