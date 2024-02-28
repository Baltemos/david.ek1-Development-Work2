
#pragma once
#include "Component.h"
#include "CreatureInput.h"
#include "Transform.h"

class Projectile : public CreatureInput
{
public:
	//void SetDirection(int aDirection);

protected:
	void Read(const nlohmann::json& someData) override;
	void Start() override;
	void Update(float aDeltaTime) override;

private:
	//int myDirection;
	//std::shared_ptr<Transform> myPlayerTransform;
	//std::weak_ptr<Transform> myTransform;
};