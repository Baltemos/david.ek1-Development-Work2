#include "Paralax.h"
#include "Entity.h"
#include "Transform.h"
void Paralax::Start()
{
	Camera* activeCamera = Camera::GetActiveCamera();
	if (activeCamera)
	{
		myCameraTransform = activeCamera->GetEntity()->GetComponent<Transform>();
		if (myCameraTransform)
		{
			myOffset = myTransform.lock()->GetWorldPosition() - myCameraTransform->GetWorldPosition();
			myOnCameraTransformValueChangedKey = myCameraTransform->OnValueChanged.Subscribe(this, &Paralax::onCameraTransformValueChanged);
		}
	}

}

void Paralax::Read(const nlohmann::json& someData)
{
	myTransform = GetEntity()->GetComponent<Transform>();

	myParalaxIntensity = someData["Intensity"];
}

void Paralax::Update(float aDeltaTime)
{
	aDeltaTime;
}

void Paralax::Render(Tga::GraphicsEngine&)
{

}

void Paralax::OnDestroy()
{
	myCameraTransform->OnValueChanged.Unsubscribe(myOnCameraTransformValueChangedKey);
}

float Paralax::GetParalaxIntensity()
{
	return myParalaxIntensity;
}

void Paralax::onCameraTransformValueChanged(Transform& aTransform)
{
	std::shared_ptr<Transform> transform = myTransform.lock();
	cu::Vector3<float> position = aTransform.GetWorldPosition() * myParalaxIntensity + myOffset;
	transform->SetWorldPosition(position);
}
