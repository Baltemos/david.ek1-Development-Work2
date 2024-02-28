#pragma once
#include "Component.h"
#include "CommonUtilities/Vector.hpp"
#include "Collider2D.h"
#include <tge/graphics/GraphicsEngine.h>

class Wind : public Component
{
	public:
		void Start() override;
		void Read(const nlohmann::json& someData) override;
		void Push(const CollisionInfo2D& aInfo);
	protected:
	private:
		CommonUtilities::Vector3<float> myDirection;
		float myIntesity;
		std::weak_ptr<Collider> myCollider;
};

