#pragma once
#include "Component.h"
#include <CommonUtilities/Vector3.hpp>
#include <CommonUtilities/Matrix3x3.hpp>
#include <memory>
#include <vector>
#include "Transform.h"
#include "Collider.h"
#include "Collider2D.h"

namespace cu = CommonUtilities;

class Physicbody : public Component
{
public:
	void SetVelocity(const cu::Vector2<float>& aVelocity);
	void AddVelocity(const cu::Vector2<float>& aVelocity);
	void AddVelocity(const cu::Vector3<float>& aVelocity);
	const cu::Vector2<float>& GetVelocity() const;
	const bool GetIsDashing() const;
	void SetIsDashing(bool aDashing);

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aDeltaTime) override;
private:
	void onCollision(const CollisionInfo2D& aInfo);

	std::weak_ptr<Transform> myTransform;
	
	cu::Vector2<float> myVelocity;
	cu::Vector2<float> myGravity;
	cu::Vector2<float> myDrag;

	std::vector<std::shared_ptr<Collider>> myColliders;

	bool myDashing;
	float myDashDuration;
};