#include "Camera.h"
#include "Entity.h"
#include "GameWorld.h"
#include <tge/graphics/GraphicsStateStack.h>

static std::multiset<Camera*> locCameras;
static Camera* locActiveCamera = nullptr;

Camera::Camera()
{
	myIsActive = false;
}

std::multiset<Camera*> Camera::GetCameras()
{
	return locCameras;
}

Camera* Camera::GetActiveCamera()
{
	return locActiveCamera;
}

void Camera::SetActive(bool aActive)
{
	if (aActive)
	{
		if (locActiveCamera)
		{
			locActiveCamera->myIsActive = false;
		}
		locActiveCamera = this;
		locActiveCamera->myIsActive = true;
	}
	else
	{
		if (locActiveCamera == this)
		{
			locActiveCamera = nullptr;
			myIsActive = false;
		}
	}
}

bool Camera::IsActive() const
{
	return myIsActive;
}

cu::Matrix4x4<float> Camera::GetMatrix() const
{
	Tga::Matrix4x4f engineProjection = Tga::Engine::GetInstance()->GetGraphicsEngine().GetGraphicsStateStack().GetCamera().GetProjection();
	cu::Matrix4x4<float> projection;

	for (int row = 1; row < 5; row++)
	{
		for (int column = 1; column < 5; column++)
		{
			projection(row, column) = engineProjection(row, column);
		}
	}

	projection = myProjection * projection;

	cu::Matrix4x4<float> matrix;
	if (!myRotationLock)
	{
		matrix = myTransform->GetRotation();
	}
	matrix = matrix * projection * myTransform->GetTranslation();
	matrix = matrix.GetInverse();
	matrix(4, 1) += myProjection.GetRowVectors()[0].Length() / projection.GetRowVectors()[0].Length();
	matrix(4, 2) += myProjection.GetRowVectors()[1].Length() / projection.GetRowVectors()[1].Length();
	matrix(4, 1) = std::floor(matrix(4, 1));
	matrix(4, 2) = std::floor(matrix(4, 2));
	return matrix;
}

void Camera::SetProjection(const cu::Matrix4x4<float>& aProjection)
{
	myProjection = aProjection;
}

cu::Matrix4x4<float> Camera::GetProjection() const
{
	return myProjection;
}

cu::Vector3<float> Camera::ScreenToWorld(cu::Vector3<float> aScreenPosition) const
{
	Tga::Engine* engine = Tga::Engine::GetInstance();
	Tga::Vector2ui tRenderSize = engine->GetRenderSize();
	cu::Vector3<float> renderSize = { static_cast<float>(tRenderSize.x), static_cast<float>(tRenderSize.y), 1.f };
	cu::Vector3<float> windowSize(GameWorld::GetInstance()->GetViewportSize(), 1.f);

	aScreenPosition *= renderSize / windowSize;

	aScreenPosition.y = renderSize.y - aScreenPosition.y;
	cu::Vector4<float> worldPos = cu::Vector4<float>(aScreenPosition, 1) * GetMatrix().GetInverse();
	return cu::Vector3<float>(worldPos.x, worldPos.y, worldPos.z);
}

void Camera::Read(const nlohmann::json& someData)
{
	locCameras.emplace(this);
	if (locActiveCamera == nullptr)
	{
		SetActive();
	}

	myTransform = GetEntity()->GetComponent<Transform>().get();

	myRotationLock = someData["RotationLock"];
	auto& projection = someData["Projection"];
	for (int row = 1; row < 5; row++)
	{
		for (int column = 1; column < 5; column++)
		{
			myProjection(row, column) = projection[row - 1][column - 1];
		}
	}
}

void Camera::Start()
{
}

void Camera::OnDestroy()
{
	locCameras.erase(this);
	if (myIsActive)
	{
		locActiveCamera = nullptr;
	}
}
