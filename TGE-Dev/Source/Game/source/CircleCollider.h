#pragma once
#include "Collider2D.h"
#include <CommonUtilities/Vector2.hpp>

class CircleCollider : public Collider
{
public:
	// Inherited via Collider
	virtual void UpdateBounds() override;

	void SetBounds(cu::Circle<float> aBounds);

	const cu::Circle<float>& GetWorldBounds() const;
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta);

private:
	std::weak_ptr<Transform> myTransform;

	cu::Circle<float> myBounds;
	cu::Circle<float> myWorldBounds;
};
