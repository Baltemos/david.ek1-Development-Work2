#pragma once
#include "Component.h"
#include "Camera.h"
class Paralax : public Component
{
	public:
		void Start() override;
		void Read(const nlohmann::json& someData) override;
		void Update(float aDeltaTime) override;

		float GetParalaxIntensity();
	protected:
	private:
		float myParalaxIntensity;
		std::weak_ptr<Transform> myTransform;
		cu::Vector3<float> myOffset;
};

