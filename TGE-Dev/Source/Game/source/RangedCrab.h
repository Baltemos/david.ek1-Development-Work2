#pragma once
#include "Component.h"
#include "CreatureInput.h"

class RangedCrab : public CreatureInput
{
public:
protected:
	void Read(const nlohmann::json& someData) override;
	void Start() override;
	void Update(float aDeltaTime) override;

private:
};

