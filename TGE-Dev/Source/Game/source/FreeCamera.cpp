#include "FreeCamera.h"
#include "Entity.h"
#include "GameWorld.h"

void FreeCamera::Read(const nlohmann::json&)
{
	myTransform = GetEntity()->GetComponent<Transform>();
	myCamera = GetEntity()->GetComponent<Camera>();

	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void FreeCamera::Update(float aTimeDelta)
{
	std::shared_ptr<Camera> camera = myCamera.lock();
	std::shared_ptr<Transform> transform = myTransform.lock();

	if (camera->IsActive())
	{
		int scrollDelta = myInputHandler->GetDeltaMouseScroll();
		if (scrollDelta != 0)
		{
			float scale = std::pow(1.001f, static_cast<float>(-scrollDelta));
			camera->SetProjection(camera->GetProjection() * cu::Matrix4x4<float>::CreateScale({ scale, scale, 1 }));
		}

		float cameraWidth = camera->GetProjection().GetRowVectors()[0].Length();

		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::A)) || myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::LEFT))) transform->MoveLocalPosition({ -cameraWidth * aTimeDelta, 0, 0 });
		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::D)) || myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::RIGHT))) transform->MoveLocalPosition({ cameraWidth * aTimeDelta, 0, 0 });
		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::W)) || myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::UP))) transform->MoveLocalPosition({ 0, cameraWidth * aTimeDelta, 0 });
		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::S)) || myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::DOWN))) transform->MoveLocalPosition({ 0, -cameraWidth * aTimeDelta, 0 });

		if (myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::MBUTTON)))
		{
			POINT mouseDelta = myInputHandler->GetDeltaMouseMove();
			cu::Vector3<float> worldDiff = camera->ScreenToWorld({ 0, 0, 0 }) - camera->ScreenToWorld({ static_cast<float>(mouseDelta.x), static_cast<float>(mouseDelta.y), 0 });

			transform->MoveWorldPosition(worldDiff);
		}
	}
}
