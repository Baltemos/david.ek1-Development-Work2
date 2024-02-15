#include "Paralax.h"
#include "Entity.h"
#include "Transform.h"
void Paralax::Start()
{
	myOffset = Camera::GetActiveCamera()->GetEntity()->GetComponent<Transform>()->GetWorldPosition();
	myTransform = GetEntity()->GetComponent<Transform>();
}

void Paralax::Read(const nlohmann::json& someData)
{
	myParalaxIntensity = someData["Intensity"];
}

void Paralax::Update(float aDeltaTime)
{
	aDeltaTime;
	std::shared_ptr<Transform> transform = myTransform.lock();
	cu::Vector3<float> position = Camera::GetActiveCamera()->GetEntity()->GetComponent<Transform>()->GetWorldPosition();
	transform->SetWorldPosition((position - myOffset) * myParalaxIntensity);
}


float Paralax::GetParalaxIntensity()
{
	return myParalaxIntensity;
}
