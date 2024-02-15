#pragma once
#include "Component.h"
class ExpirationTimer : public Component
{
public:

protected:

private:
	virtual void Read(const nlohmann::json& someData) override;
	void Update(float aDeltaTime) override;
	float myTimer;
};