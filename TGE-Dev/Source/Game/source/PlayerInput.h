#pragma once
#include "CreatureInput.h"
#include "CommonUtilities/InputHandler.h"
#include "Collider2D.h"

class PlayerInput : public CreatureInput
{
public:
	Event<float>& GetOnDash();
	Event<int>& GetOnLookVertical();

protected:
	virtual void Start() override;
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aDeltaTime) override;

private:
	Event<float> myOnDash;
	Event<int> myOnLookVertical;
	CommonUtilities::InputHandler* myInputHandler;
	bool myUnlockedDash;
	bool myUnlockedDoubleJump;
	void GetOnCollision(const CollisionInfo2D& aInfo);
};

