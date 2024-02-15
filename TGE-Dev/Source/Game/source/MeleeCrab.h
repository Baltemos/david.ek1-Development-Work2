#pragma once
#include "Component.h"
#include "CreatureInput.h"
#include "Transform.h"

class MeleeCrab : public CreatureInput
{
public:
protected:
	void Read(const nlohmann::json& someData) override;
	void Start() override;
	void Update(float aDeltaTime) override;

private:
	std::shared_ptr<Transform> myPlayerTransform;
	std::weak_ptr<Transform> myTransform;
};

