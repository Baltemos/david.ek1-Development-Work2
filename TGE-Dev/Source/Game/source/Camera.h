#pragma once
#include "Component.h"
#include <set>
#include "Transform.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	static std::multiset<Camera*> GetCameras();
	static Camera* GetActiveCamera();

	void SetProjection(const cu::Matrix4x4<float>& aProjection);
	cu::Matrix4x4<float> GetProjection() const;	

	void SetActive(bool aActive = true);
	bool IsActive() const;
	cu::Matrix4x4<float> GetMatrix() const;

	cu::Vector3<float> ScreenToWorld(cu::Vector3<float> aScreenPosition) const;

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;
	virtual void OnDestroy() override;
private:
	Transform* myTransform;

	cu::Matrix4x4<float> myProjection;
	bool myRotationLock;
	bool myIsActive;
};