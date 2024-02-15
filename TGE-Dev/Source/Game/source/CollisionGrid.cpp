//#include "CollisionGrid.h"
//#include "Collision.h"
//
//static QuadTree locCollisionGrid;
//
//void InitCollisionGrid(cu::Vector2<float>& aMin, cu::Vector2<float>& aMax)
//{
//	locCollisionGrid.Init(aMin, aMax);
//}
//
//void UpdateCollisionGridObject(std::shared_ptr<Collider> aCollider)
//{
//	locCollisionGrid.Remove(aCollider);
//	locCollisionGrid.Add(aCollider);
//}
//
//void CheckCollision()
//{
//	
//	std::vector<std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>> colliderPairs;
//	locCollisionGrid.RemoveDestroyed();
//	locCollisionGrid.CheckCollision(colliderPairs);
//
//	for (std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>& pair : colliderPairs)
//	{
//		pair.first->Intersects(*pair.second);
//	}
//}
//
//void AddColliderToGrid(std::shared_ptr<Collider> aCollider)
//{
//	locCollisionGrid.Add(aCollider);
//}