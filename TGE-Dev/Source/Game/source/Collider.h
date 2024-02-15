//#pragma once
//#include "Component.h"
//#include "Event.h"
//#include "CommonUtilities/Vector3.hpp"
//#include "CommonUtilities/Vector2.hpp"
//#include <memory>
//
//namespace cu = CommonUtilities;
//
//enum class ColliderType
//{
//	Circle,
//	Rectanlge,
//	Simple
//};
//
//class CircleCollider;
//class RectangleCollider;
//
//class Collider : public Component
//{
//	friend CircleCollider;
//	friend RectangleCollider;
//
//	public:
//		virtual ~Collider() = default;
//		virtual void OnUpdate(cu::Vector3<float>& aPosition) = 0;
//
//		void SetStatic(bool aIsStatic);
//		bool IsStatic();
//
//		void SetIsTrigger(bool aIsTrigger);
//		bool IsTrigger();
//
//		cu::Vector2<float>& GetMax();
//		cu::Vector2<float>& GetMin();
//
//		virtual void Intersects(Collider& aCollider) = 0;
//
//		void SetMoveDir(cu::Vector2<float> aDir);
//
//		Event<Collider&, cu::Vector2<float>>& GetOnTrigger();
//		Event<Collider&, cu::Vector2<float>>& GetOnCollision();
//
//	protected:
//		virtual void Read(const nlohmann::json& someData) override;
//
//		cu::Vector2<float> myMin;
//		cu::Vector2<float> myMax;
//
//		cu::Vector2<float> myMoveDir;
//
//		std::string myTag;
//
//		float myRadius;
//
//		ColliderType myType;
//
//		bool myIsStatic;
//		bool myIsTrigger;
//
//	private:
//		Event<Collider&, cu::Vector2<float>> OnTrigger;
//		Event<Collider&, cu::Vector2<float>> OnCollision;
//};