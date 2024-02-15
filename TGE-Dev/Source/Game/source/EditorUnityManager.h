#pragma once
#include "Component.h"

class EditorUnityManager : public Component
{
public:

	void LoadBatch(const nlohmann::json& aBatch);
	void Clear();

protected:

	virtual void Read(const nlohmann::json& someData) override;

private:
	std::shared_ptr<Entity> myUnityRoot;
};