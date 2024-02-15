#pragma once
#include "CommonUtilities/Vector2.hpp"
#include "CommonUtilities/AABB2D.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace cu = CommonUtilities;

class Collider;

class ChadTree
{
public:
	ChadTree(const cu::AABB2D<float>& aBounds, size_t aCapacity, size_t aMaxDepth);
	~ChadTree();

	std::vector<Collider*> GetItems() const;
	std::vector<std::vector<Collider*>> GetQuads() const;

	void Emplace(Collider* aCollider);
	void Update();

	size_t Count() const;

private:
	void Subdivide();
	void Recombine();

	bool myIsDivided;

	float myWidth;
	float myHeight;
	cu::Vector2<float> myCenter;

	cu::AABB2D<float> myBounds;
	size_t myCapacity;
	size_t myMaxDepth;

	std::vector<Collider*> myItems;

	ChadTree* myChildren[4];
	cu::AABB2D<float> myQuads[4];
};