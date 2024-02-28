#pragma once
#include "CreatureInput.h"
#include "CommonUtilities/InputHandler.h"
#include "Collider2D.h"
#include <memory>

class PlayerInput : public CreatureInput
{
public:
	Event<float>& GetOnDash();
	Event<int>& GetOnLookVertical();
	//std::shared_ptr<int> AccessJumpsLeft();
	void ResetJumpsLeft();
	void ReduceJumpsLeft();
	bool GetHasKey() const;

protected:
	virtual void Start() override;
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aDeltaTime) override;

private:
	Event<float> myOnDash;
	Event<int> myOnLookVertical;
	CommonUtilities::InputHandler* myInputHandler;
	//std::shared_ptr<int> myJumpsLeft;
	bool myUnlockedDash;
	//bool myUnlockedDoubleJump;
	void GetOnCollision(const CollisionInfo2D& aInfo);
	//std::shared_ptr<int> myPlayerJumpsLeft;
	int myJumpsLeft;
	int myMaxJumps;
	bool myShouldResetJumps;
	bool myHasKey;
	std::string myMapToLoad;
};

