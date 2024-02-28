#pragma once
#include "Component.h"
#include "Physicbody.h"
#include "SpriteRenderer2D.h"

class CreatureMovementObserver : public Component
{
	public:
	protected:
		virtual void Read(const nlohmann::json& someData) override;
		virtual void Start() override;
		virtual void Update(const float aDeltaTime) override;
		virtual void onAttack(/*int aDirection, */float aDeltaTime, float anAttackRange);
		std::shared_ptr<int> myEntityDirection;
		std::weak_ptr<Physicbody> myPhysicbody;
		std::weak_ptr<Transform> myTransform;
		//std::weak_ptr<Transform> myTransform;
		bool myIsGrounded;
		bool GetGrounded() const;
		virtual void onCollision(const CollisionInfo2D& aInfo);
		bool myIsGroundedCheck;
		virtual void onLetGoOfJump();
		float myJumpChargeTimer;
		virtual void onJump(const float aDeltaTime, const float aSpeed);
		float myDrag;
		float myMaxJumpChargeTime;
		float myAttackTimer;
		float myAttackCooldown;

	private:
		void onMoveHorizontal(int aDirection, float aSpeed);
		//void onLetGoOfJump();
};

