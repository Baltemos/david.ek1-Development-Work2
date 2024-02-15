#pragma once
#include "Component.h"
#include <set>

class Tag : public Component
{
public:
	Tag() = default;
	~Tag() = default;

	static Tag* FindFirst(const std::string& aTag);
	static std::multiset<Tag*> FindAll(const std::string& aTag);
	static bool HasTag(const std::shared_ptr<Entity>& aEntity, const std::string& aTag);

	void Tag::SetTag(const std::string& aTag);
	const std::string& GetTag() const;

	bool Compare(const std::string& aTag) const;
	bool Compare(const Tag& aTag) const;

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void OnDestroy() override;

private:
	void AddEntry();
	void RemoveEntry();

	std::string myTag;
};