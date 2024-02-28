#pragma once
#include "Component.h"
#include "Event.h"
#include "UIManager.h"
#include "Transform.h"
#include <CommonUtilities/AABB2D.hpp>

class UIElement : public Component
{
public:
	UIElement();

	std::shared_ptr<Transform> GetTransform() const;
	std::shared_ptr<UIManager> GetUIManager() const;

	cu::AABB2D<float> GetBounds() const;

	bool IsHovered() const;

	Event<std::shared_ptr<UIElement>> OnClicked;
	Event<std::shared_ptr<UIElement>> OnClickedReleased;
protected:
	void UpdateBounds();

	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;
	virtual void Update(float aTimeDelta) override;
private:
	std::weak_ptr<UIManager> FindUIManager();

	cu::AABB2D<float> myLocalBounds;
	cu::AABB2D<float> myWorldBounds;

	bool myIsHovered;

	std::weak_ptr<Transform> myTransform;
	std::weak_ptr<UIManager> myUIManager;
};