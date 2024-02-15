//#include "Collider.h"
//#include "CollisionGrid.h"
//#include "Entity.h"
//
//void Collider::SetStatic(bool aIsStatic)
//{
//	myIsStatic = aIsStatic;
//}
//
//bool Collider::IsStatic()
//{
//	return myIsStatic;
//}
//
//void Collider::SetIsTrigger(bool aIsTrigger)
//{
//	myIsTrigger = aIsTrigger;
//}
//bool Collider::IsTrigger()
//{
//	return myIsTrigger;
//}
//
//cu::Vector2<float>& Collider::GetMax()
//{
//	return myMax;
//}
//
//cu::Vector2<float>& Collider::GetMin()
//{
//	return myMin;
//}
//
//void Collider::SetMoveDir(cu::Vector2<float> aDir)
//{
//	myMoveDir = aDir;
//}
//
//Event<Collider&, cu::Vector2<float>>& Collider::GetOnTrigger()
//{
//	return OnTrigger;
//}
//
//Event<Collider&, cu::Vector2<float>>& Collider::GetOnCollision()
//{
//	return OnCollision;
//}
//
//void Collider::Read(const nlohmann::json& someData)
//{
//	myMin.x = someData["Min"]["x"];
//	myMin.y = someData["Min"]["y"];
//	myMax.x = someData["Max"]["x"];
//	myMax.y = someData["Max"]["y"];
//	myRadius = someData["Radius"];
//	myType = someData["Type"];
//	myIsStatic = someData["Static"];
//	myIsTrigger = someData["Trigger"];
//
//	AddColliderToGrid(GetEntity()->GetComponent<Collider>());
//}
