#include "QuadTree.h"
#include "Collider2D.h"
#include <CommonUtilities/Intersection.hpp>

////////////
// CHADTREE
////////////

ChadTree::ChadTree(const cu::AABB2D<float>& aBounds, size_t aCapacity, size_t aMaxDepth)
{
	myIsDivided = false;

	myItems.reserve(aCapacity);
	memset(myChildren, 0, sizeof(myChildren));

	myBounds = aBounds;
	myCapacity = aCapacity;
	myMaxDepth = aMaxDepth;

	myCenter = (myBounds.Min + myBounds.Max) * 0.5f;
	myWidth = aBounds.Max.x - aBounds.Min.x;
	myHeight = aBounds.Max.y - aBounds.Min.y;

	myQuads[0] = cu::AABB2D<float>({ myBounds.Min.x, myCenter.y }, { myCenter.x, myBounds.Max.y });
	myQuads[1] = cu::AABB2D<float>(myCenter, myBounds.Max);
	myQuads[2] = cu::AABB2D<float>(myBounds.Min, myCenter);
	myQuads[3] = cu::AABB2D<float>({ myCenter.x, myBounds.Min.y }, { myBounds.Max.x, myCenter.y });
}

ChadTree::~ChadTree()
{
	for (ChadTree* child : myChildren)
	{
		if (child != nullptr)
		{
			delete child;
		}
	}
}

std::vector<Collider*> ChadTree::GetItems() const
{
	if (myIsDivided)
	{
		std::vector<Collider*> items;
		std::vector<Collider*> childItems;
		for (ChadTree* child : myChildren)
		{
			childItems = child->GetItems();
			items.insert(items.end(), childItems.begin(), childItems.end());
		}
		return items;
	}
	else
	{
		return myItems;
	}
}

std::vector<std::vector<Collider*>> ChadTree::GetQuads() const
{
	std::vector<std::vector<Collider*>> quads;
	if (myIsDivided)
	{
		std::vector<std::vector<Collider*>> childQuads;
		for (ChadTree* child : myChildren)
		{
			childQuads = child->GetQuads();
			quads.insert(quads.end(), childQuads.begin(), childQuads.end());
		}
	}
	else
	{
		quads.push_back(myItems);
	}
	return quads;
}

void ChadTree::Emplace(Collider* aCollider)
{
	if (myMaxDepth == 0 || (!myIsDivided && myItems.size() < myCapacity))
	{
		myItems.push_back(aCollider);
	}
	else
	{
		if (!myIsDivided)
		{
			Subdivide();
			for (auto& item : myItems)
			{
				Emplace(item);
			}
			myItems.clear();
		}

		for (ChadTree* child : myChildren)
		{
			cu::Vector2<float> intersectionPoint;
			if (cu::IntersectionCircleAABB2D(aCollider->GetBoundingCircle(), child->myBounds, intersectionPoint))
			{
				child->Emplace(aCollider);
			}
		}
	}
}

void ChadTree::Update()
{
	if (myIsDivided)
	{
		if (Count() < myCapacity)
		{
			Recombine();
		}
		else
		{
			for (ChadTree* child : myChildren)
			{
				child->Update();
			}
		}
	}
	else
	{
		myItems.erase(
			std::remove_if(
				myItems.begin(),
				myItems.end(),
				[=](Collider* item) {
					cu::Vector2<float> intersectionPoint;
					return item->IsDestroyed() || !cu::IntersectionCircleAABB2D(item->GetBoundingCircle(), myBounds, intersectionPoint);
				}
			),
			myItems.end()
		);
	}
}

size_t ChadTree::Count() const
{
	if (myIsDivided)
	{
		size_t count = 0;
		for (ChadTree* child : myChildren)
		{
			count += child->Count();
		}
		return count;
	}
	else
	{
		return myItems.size();
	}
}

void ChadTree::Subdivide()
{
	for (size_t ind = 0; ind < 4; ind++)
	{
		myChildren[ind] = new ChadTree(myQuads[ind], myCapacity, myMaxDepth - 1);
	}
	myIsDivided = true;
}

void ChadTree::Recombine()
{
	myItems = GetItems();
	for (ChadTree*& child : myChildren)
	{
		delete child;
		child = nullptr;
	}
	myIsDivided = false;
}
