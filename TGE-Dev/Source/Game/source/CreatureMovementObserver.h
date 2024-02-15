#pragma once
#include "Component.h"
#include "Physicbody.h"

class CreatureMovementObserver : public Component
{
	public:
	protected:
		virtual void Read(const nlohmann::json& someData) override;
		virtual void Start() override;
		virtual void Update(float aTimedelta) override;
		virtual void onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange);
		std::shared_ptr<int> myEntityDirection;
		std::weak_ptr<Physicbody> myPhysicbody;
		std::weak_ptr<Transform> myTransform;
		//std::weak_ptr<Transform> myTransform;

		bool GetGrounded() const;

	private:
		void onCollision(const CollisionInfo2D& aInfo);
		void onMoveHorizontal(int aDirection, float aSpeed);
		void onJump(float aDeltaTime, float aSpeed);
		void onLetGoOfJump();

		bool myIsGroundedCheck;
		bool myIsGrounded;

		float myDrag;
		float myMaxJumpChargeTime;
		float myJumpChargeTimer;
		float myAttackTimer;
};

