#include "Transform.h"
#include "Entity.h"
#include "GameWorld.h"
#include "Camera.h"

Transform::Transform()
{
	myParent = nullptr;
}

void Transform::Read(const nlohmann::json& someData)
{
	auto& scale = someData["Scale"];
	myScale = cu::Matrix4x4<float>::CreateScale(cu::Vector3<float>(scale[0], scale[1], scale[2]));

	auto& translation = someData["Translation"];
	myTranslation = cu::Matrix4x4<float>::CreateTransform(cu::Vector3<float>(translation[0], translation[1], translation[2]), {});

	auto& rotation = someData["Rotation"];
	myEuler.x = rotation[0];
	myEuler.y = rotation[1];
	myEuler.z = rotation[2];

	SetSpace(static_cast<Space>(someData["Space"].get<int>()));

	Recalculate();
}

void Transform::OnDestroy()
{
	if (myParent)
	{
		SetParent(nullptr, false);
	}
	for (std::weak_ptr<Transform> child : myChildren)
	{
		if (child.expired() == false)
		{
			std::shared_ptr<Transform> transform = child.lock();
			if (transform->IsDestroyed() == false)
			{
				transform->GetEntity()->Destroy();
			}
		}
	}
}

void Transform::SetLocalPosition(const cu::Vector3<float>& aPosition)
{
	this->myTranslation(4, 1) = aPosition.x;
	this->myTranslation(4, 2) = aPosition.y;
	this->myTranslation(4, 3) = aPosition.z;

	Recalculate();
}

void Transform::MoveLocalPosition(const cu::Vector3<float>& aVector)
{
	SetLocalPosition(GetLocalPosition() + aVector);
}

void Transform::SetWorldPosition(const cu::Vector3<float>& aPosition)
{
	if (this->myParent != nullptr)
	{
		SetLocalPosition(this->myParent->WorldToLocal(aPosition));
	}
	else
	{
		SetLocalPosition(aPosition);
	}
}

void Transform::MoveWorldPosition(const cu::Vector3<float>& aVector)
{
	SetWorldPosition(GetLocalPosition() + aVector);
}

void Transform::SetSpace(Transform::Space aSpace)
{
	GetRoot()->PropegateSpace(aSpace);
}

Transform::Space Transform::GetSpace() const
{
	return mySpace;
}

RenderBuffer* Transform::GetSpaceRenderBuffer()
{
	GameWorld* gameWorld = GameWorld::GetInstance();
	switch (mySpace)
	{
	case Space::eWorld: return &gameWorld->GetRenderBuffer();
	case Space::eUserInterface: return &gameWorld->GetUIRenderBuffer();
	default: return nullptr;
	}
}

cu::Vector3<float> Transform::GetLocalRight()
{
	return cu::Vector3<float>(myTransformMatrix(1, 1), myTransformMatrix(1, 2), myTransformMatrix(1, 3));
}

cu::Vector3<float> Transform::GetLocalUp()
{
	return cu::Vector3<float>(myTransformMatrix(2, 1), myTransformMatrix(2, 2), myTransformMatrix(2, 3));
}

cu::Vector3<float> Transform::GetLocalForward()
{
	return cu::Vector3<float>(myTransformMatrix(3, 1), myTransformMatrix(3, 2), myTransformMatrix(3, 3));
}

cu::Vector3<float> Transform::GetLocalPosition() const
{
	return cu::Vector3<float>(this->myTransformMatrix(4, 1), this->myTransformMatrix(4, 2), this->myTransformMatrix(4, 3));
}

const cu::Vector3<float>& Transform::GetWorldPosition() const
{
	return this->myWorldPosition;
}

cu::Vector3<float> Transform::GetLocalEulerAngles() const
{
	return myEuler;
}

void Transform::SetLocalEulerAngles(const cu::Vector3<float>& someAngles)
{
	myEuler = someAngles;
	Recalculate();
}

void Transform::SetWorldEulerAngles(const cu::Vector3<float>& someAngles)
{
	cu::Vector3<float> parentAngles;
	if (myParent != nullptr)
	{
		parentAngles = myParent->GetWorldEulerAngles();
	}
	SetLocalEulerAngles(someAngles - parentAngles);
}

cu::Vector3<float> Transform::GetWorldEulerAngles() const
{
	cu::Vector3<float> parentAngles;
	if (myParent != nullptr)
	{
		parentAngles = myParent->GetWorldEulerAngles();
	}
	return parentAngles + myEuler;
}

cu::Vector3<float> Transform::LocalRotateVector(cu::Vector3<float> aVector) const
{
	cu::Vector4<float> vector = cu::Vector4<float>(aVector.x, aVector.y, aVector.z, 0);
	vector = vector * this->myTransformMatrix;
	return cu::Vector3<float>(vector.x, vector.y, vector.z);
}

cu::Vector3<float> Transform::WorldRotateVector(cu::Vector3<float> aVector) const
{
	cu::Vector4<float> vector = cu::Vector4<float>(aVector.x, aVector.y, aVector.z, 0);
	vector = vector * GetSummedMatrix();
	return cu::Vector3<float>(vector.x, vector.y, aVector.z);
}

cu::Vector3<float> Transform::LocalToWorld(const cu::Vector3<float>& aLocal) const
{
	cu::Vector4<float> vec = LocalToWorld(cu::Vector4<float>(aLocal.x, aLocal.y, aLocal.z, 1));

	return cu::Vector3<float>(vec.x, vec.y, vec.z);
}

cu::Vector3<float> Transform::WorldToLocal(const cu::Vector3<float>& aWorld) const
{
	cu::Vector4<float> vec = WorldToLocal(cu::Vector4<float>(aWorld.x, aWorld.y, aWorld.z, 1));

	return cu::Vector3<float>(vec.x, vec.y, vec.z);
}

cu::Vector3<float> Transform::ScreenToWorld(const cu::Vector2<float>& aScreen) const
{
	switch (mySpace)
	{
	case Space::eWorld: {

		Camera* activeCamera = Camera::GetActiveCamera();
		if (activeCamera)
		{
			return activeCamera->ScreenToWorld({ static_cast<float>(aScreen.x), static_cast<float>(aScreen.y), 0 });
		}
		else return { NAN, NAN, NAN };
		break;
	}
	case Space::eUserInterface: {
		Tga::Vector2ui resolution = GameWorld::GetInstance()->GetResolution();
		cu::Vector2<float> viewportSize = GameWorld::GetInstance()->GetViewportSize();
		return cu::Vector3<float>(cu::Vector2<float>(aScreen.x, viewportSize.y - aScreen.y) * cu::Vector2<float>(static_cast<float>(resolution.x), static_cast<float>(resolution.y)) / viewportSize, 0.f);
	}
	default: return { NAN, NAN, NAN };
	}
}

void Transform::SetParent(std::shared_ptr<Transform> aParent, bool aKeepWorldSpace)
{
	if (aParent != nullptr && IsDestroyed())
	{
		return;
	}

	if (this->myParent != nullptr)
	{
		for (auto it = this->myParent->myChildren.begin(); it != this->myParent->myChildren.end(); it++)
		{
			if ((*it).lock().get() == this)
			{
				this->myParent->myChildren.erase(it);
				break;
			}
		}
	}

	this->myParent = nullptr;
	if (aParent != nullptr && aParent->IsDestroyed() == false)
	{
		this->myParent = aParent;
		aParent->myChildren.push_back(std::dynamic_pointer_cast<Transform>(GetSharedPtr()));
	}

	if (aKeepWorldSpace)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		Recalculate();
	}
}

std::shared_ptr<Transform> Transform::GetParent() const
{
	return myParent;
}

std::shared_ptr<Transform> Transform::GetRoot()
{
	if (myParent)
	{
		return myParent->GetRoot();
	}
	return std::dynamic_pointer_cast<Transform>(GetSharedPtr());
}

cu::Matrix4x4<float> Transform::GetSummedMatrix() const
{
	return this->myTransformMatrix * this->myParentalTransform;
}

cu::Matrix4x4<float> Transform::GetMatrix() const
{
	return this->myTransformMatrix;
}

cu::Matrix4x4<float> Transform::GetScale() const
{
	return myScale;
}

cu::Matrix4x4<float> Transform::GetTranslation() const
{
	return myTranslation;
}

cu::Matrix4x4<float> Transform::GetRotation() const
{
	return myRotation;
}

const std::vector<std::weak_ptr<Transform>>& Transform::GetChildren() const
{
	return myChildren;
}

cu::Vector4<float> Transform::LocalToWorld(const cu::Vector4<float>& aLocal) const
{
	cu::Vector4<float> vec = aLocal * GetSummedMatrix();

	return vec;
}

cu::Vector4<float> Transform::WorldToLocal(const cu::Vector4<float>& aWorld) const
{
	cu::Vector4<float> vec = aWorld * GetSummedMatrix().GetFastInverse();

	return vec;
}

void Transform::Recalculate()
{
	if (this->myParent != nullptr)
	{
		this->myParentalTransform = this->myParent->GetSummedMatrix();
	}
	else
	{
		this->myParentalTransform = cu::Matrix4x4<float>();
	}
	myRotation = cu::Matrix4x4<float>::CreateRotationAroundX(myEuler.x) * cu::Matrix4x4<float>::CreateRotationAroundY(myEuler.y) * cu::Matrix4x4<float>::CreateRotationAroundZ(myEuler.z);

	myTransformMatrix = myScale * myRotation * myTranslation;

	this->myWorldPosition = LocalToWorld(cu::Vector3<float>());

	for (std::weak_ptr<Transform> transform : this->myChildren)
	{
		transform.lock()->Recalculate();
	}

	OnValueChanged(*this);
}

void Transform::PropegateSpace(Space aSpace)
{
	mySpace = aSpace;
	for (auto& child : myChildren)
	{
		child.lock()->PropegateSpace(aSpace);
	}
}
