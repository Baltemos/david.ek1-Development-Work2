#pragma once
#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include <CommonUtilities/InputHandler.h>

namespace cu = CommonUtilities;

class FreeCamera : public Component
{
public:

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta) override;

private:
	std::weak_ptr<Camera> myCamera;
	std::weak_ptr<Transform> myTransform;

	cu::InputHandler* myInputHandler;
};