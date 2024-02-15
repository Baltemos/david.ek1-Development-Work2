#pragma once
#include "Component.h"
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix4x4.hpp>

namespace cu = CommonUtilities;

class Transform : public Component
{
public:
	Transform();

	// Inherited via Component
	virtual void Read(const nlohmann::json& someData) override;

	void SetLocalPosition(const cu::Vector3<float>& aPosition);
	void MoveLocalPosition(const cu::Vector3<float>& aVector);
	void SetWorldPosition(const cu::Vector3<float>& aPosition);
	void MoveWorldPosition(const cu::Vector3<float>& aVector);

	cu::Vector3<float> GetLocalRight();
	cu::Vector3<float> GetLocalUp();
	cu::Vector3<float> GetLocalForward();

	cu::Vector3<float> GetLocalPosition() const;
	const cu::Vector3<float>& GetWorldPosition() const;

	cu::Vector3<float> GetLocalEulerAngles() const;
	void SetLocalEulerAngles(const cu::Vector3<float>& someAngles);

	void SetWorldEulerAngles(const cu::Vector3<float>& someAngles);
	cu::Vector3<float> GetWorldEulerAngles() const;

	cu::Vector3<float> LocalRotateVector(cu::Vector3<float> aVector) const;
	cu::Vector3<float> WorldRotateVector(cu::Vector3<float> aVector) const;

	cu::Vector3<float> LocalToWorld(const cu::Vector3<float>& aLocal) const;
	cu::Vector3<float> WorldToLocal(const cu::Vector3<float>& aWorld) const;

	void SetParent(Transform* aParent, bool aKeepWorldSpace);

	cu::Matrix4x4<float> GetSummedMatrix() const;
	cu::Matrix4x4<float> GetMatrix() const;

	cu::Matrix4x4<float> GetScale() const;
	cu::Matrix4x4<float> GetTranslation() const;
	cu::Matrix4x4<float> GetRotation() const;

	const std::vector<Transform*>& GetChildren() const;
protected:
	// Inherited via Component
	virtual void OnDestroy() override;

private:
	cu::Vector4<float> LocalToWorld(const cu::Vector4<float>& aLocal) const;
	cu::Vector4<float> WorldToLocal(const cu::Vector4<float>& aWorld) const;

	void Recalculate();

	Transform* myParent;
	std::vector<Transform*> myChildren;

	cu::Matrix4x4<float> myScale;
	cu::Matrix4x4<float> myTranslation;
	cu::Matrix4x4<float> myRotation;

	cu::Matrix4x4<float> myTransformMatrix;
	cu::Matrix4x4<float> myParentalTransform;

	cu::Vector3<float> myWorldPosition;
	cu::Vector3<float> myEuler;
};
