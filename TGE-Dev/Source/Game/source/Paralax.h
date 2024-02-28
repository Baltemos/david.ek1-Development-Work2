#pragma once
#include "Component.h"
#include "Camera.h"
class Paralax : public Component
{
	public:
		void Start() override;
		void Read(const nlohmann::json& someData) override;
		void Update(float aDeltaTime) override;
		virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) override;
		virtual void OnDestroy() override;


		float GetParalaxIntensity();
	protected:
	private:
		void onCameraTransformValueChanged(Transform& aTransform);

		EventKey myOnCameraTransformValueChangedKey;
		std::shared_ptr<Transform> myCameraTransform;

		float myParalaxIntensity;
		std::weak_ptr<Transform> myTransform;
		cu::Vector3<float> myOffset;
};

