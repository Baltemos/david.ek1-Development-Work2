#pragma once
//#include "Collider.h"
#include "Collider2D.h"
#include <CommonUtilities/Vector2.hpp>
#include <CommonUtilities/AABB2D.hpp>

namespace cu = CommonUtilities;

class RectangleCollider : public Collider
{
public:
	// Inherited via Collider
	virtual void UpdateBounds() override;

	void SetBounds(cu::AABB2D<float> aBounds);

	const cu::AABB2D<float>& GetWorldBounds() const;
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta);

private:
	std::weak_ptr<Transform> myTransform;

	cu::AABB2D<float> myBounds;
	cu::AABB2D<float> myWorldBounds;
};