#include "Transform.h"
#include "Entity.h"
#include "GameWorld.h"

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

	Recalculate();
}

void Transform::OnDestroy()
{
	if (myParent)
	{
		SetParent(nullptr, false);
	}
	for (Transform* child : myChildren)
	{
		child->GetEntity()->Destroy();
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

void Transform::SetParent(Transform* aParent, bool aKeepWorldSpace)
{
	if (this->myParent != nullptr)
	{
		for (auto it = this->myParent->myChildren.begin(); it != this->myParent->myChildren.end(); it++)
		{
			if (*it == this)
			{
				this->myParent->myChildren.erase(it);
				break;
			}
		}
	}

	this->myParent = aParent;
	if (aParent != nullptr)
	{
		aParent->myChildren.push_back(this);
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

const std::vector<Transform*>& Transform::GetChildren() const
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

	for (Transform* transform : this->myChildren)
	{
		transform->Recalculate();
	}
}
