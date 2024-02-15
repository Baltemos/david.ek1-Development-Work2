#pragma once
#include "Component.h"
#include "Collider2D.h"
class Pickup : public Component
{
public:
	void Start() override;
	void Read(const nlohmann::json& someData) override;
	void Trigger(Collider& aCollider, cu::Vector2<float> aImpactPoint);
protected:
private:
};

