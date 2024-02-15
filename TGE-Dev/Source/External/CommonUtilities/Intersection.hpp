#pragma once
#include <array>
#include "Ray.hpp"
#include "AABB3D.hpp"
#include "Sphere.hpp"
#include "Vector3.hpp"
#include "Plane.hpp"
#include "Line.hpp"
#include "Circle.hpp"
#include "AABB2D.hpp"
namespace CommonUtilities
{
	// If the ray is parallel to the plane, aOutIntersectionPoint remains unchanged. If
	// the ray is in the plane, true is returned, if not, false is returned. If the ray
	// isn't parallel to the plane and hits, the intersection point is stored in
	// aOutIntersectionPoint and true returned otherwise false is returned and
	// outIntersectionPoint is unchanged.
	template <class T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& outIntersectionPoint, const bool anIntersectWithFrontOnly = false)
	{
		bool returnValue = false;
		CommonUtilities::Vector3<T> rayOrigin = aRay.GetOrigin();
		T pointScalarNormal = aPlane.GetPoint().Dot(aPlane.GetNormal());
		T directionScalarNormal = aRay.GetDirection().Dot(aPlane.GetNormal());
		T t = (pointScalarNormal - rayOrigin.Dot(aPlane.GetNormal())) / directionScalarNormal;
		if (directionScalarNormal == 0)
		{
			if (aPlane.IsTouching(rayOrigin))
			{
				outIntersectionPoint = rayOrigin;
				returnValue = true;
			}
		}
		else if (t >= static_cast<T>(0))
		{
			if (directionScalarNormal < 0 || anIntersectWithFrontOnly == false)
			{
				outIntersectionPoint = rayOrigin + t * aRay.GetDirection();
				returnValue = true;
			}
		}
		return returnValue;
	}

	template <class T>
	bool IntersectionLineRay(const Line<T>& aLine, const Ray<T>& aRay, Vector3<T>& outIntersectionPoint, const bool anIntersectWithFrontOnly = false)
	{
		Vector3<T> lineOrigin(aLine.GetPoint().x, aLine.GetPoint().y, 0);
		Vector3<T> lineNormal(aLine.GetNormal().x, aLine.GetNormal().y, 0);
		bool returnValue = false;
		CommonUtilities::Vector3<T> rayOrigin = aRay.GetOrigin();
		T pointScalarNormal = lineOrigin.Dot(lineNormal);
		T directionScalarNormal = aRay.GetDirection().Dot(lineNormal);
		T t = (pointScalarNormal - rayOrigin.Dot(lineNormal)) / directionScalarNormal;
		if (directionScalarNormal == 0)
		{
			if (lineNormal.Dot(aRay.GetOrigin() - lineOrigin) == 0)
			{
				outIntersectionPoint = rayOrigin;
				returnValue = true;
			}
		}
		else if (t >= static_cast<T>(0))
		{
			if (directionScalarNormal < 0 || anIntersectWithFrontOnly == false)
			{
				outIntersectionPoint = rayOrigin + t * aRay.GetDirection();
				returnValue = true;
			}
		}
		return returnValue;
	}

	// If no collision, aOutIntersectionPoint remains unchanged.
	// If The sphere overlaps the AABB true is returned, if not, false is returned.
	// The point on the AABB closest to the sphere centre is saved in aOutIntersectionPoint.
	// A sphere touching the aabb is considered overlapping.
	template <class T>
	bool IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB3D<T>& anAABB3D, Vector3<T>& anOutIntersectionPoint)
	{
		CommonUtilities::Vector3<T> position = aSphere.GetPosition();
		if (position.x < anAABB3D.GetMinimumPoint().x)
		{
			position.x = anAABB3D.GetMinimumPoint().x;
		}
		else if (position.x > anAABB3D.GetMaximumPoint().x)
		{
			position.x = anAABB3D.GetMaximumPoint().x;
		}

		if (position.y < anAABB3D.GetMinimumPoint().y)
		{
			position.y = anAABB3D.GetMinimumPoint().y;
		}
		else if (position.y > anAABB3D.GetMaximumPoint().y)
		{
			position.y = anAABB3D.GetMaximumPoint().y;
		}

		if (position.z < anAABB3D.GetMinimumPoint().z)
		{
			position.z = anAABB3D.GetMinimumPoint().z;
		}
		else if (position.z > anAABB3D.GetMaximumPoint().z)
		{
			position.z = anAABB3D.GetMaximumPoint().z;
		}
		if (aSphere.IsInside(position))
		{
			anOutIntersectionPoint = position;
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename _Ty>
	bool IntersectionCircleAABB2D(const Circle<_Ty>& aCircle, const AABB2D<_Ty>& aAABB2D, Vector2<_Ty>& aOutIntersectionPoint)
	{
		aOutIntersectionPoint = aCircle.GetCenter();

		aOutIntersectionPoint.x = (std::min)(aOutIntersectionPoint.x, aAABB2D.Max.x);
		aOutIntersectionPoint.x = (std::max)(aOutIntersectionPoint.x, aAABB2D.Min.x);
		aOutIntersectionPoint.y = (std::min)(aOutIntersectionPoint.y, aAABB2D.Max.y);
		aOutIntersectionPoint.y = (std::max)(aOutIntersectionPoint.y, aAABB2D.Min.y);

		return aCircle.IsInside(aOutIntersectionPoint);
	}

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template <class T>
	bool IntersectionAABB3DRay(const AABB3D<T>& aAABB, const Ray<T>& aRay/*, Vector3<T>& anOutIntersectionPoint*/)
	{
		std::array< CommonUtilities::Plane<T>, 6> planeArray =
		{
			CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMinimumPoint().z)),

		CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMaximumPoint().z)),

		CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMinimumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMaximumPoint().z)),

		CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMaximumPoint().z)),

		CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMinimumPoint().z)),

		CommonUtilities::Plane<T>(CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y, aAABB.GetMaximumPoint().z),
										CommonUtilities::Vector3<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y, aAABB.GetMinimumPoint().z))
		};
		CommonUtilities::Vector3<T> position;
		for (int index = 0; index < planeArray.size(); index++)
		{
			if (IntersectionPlaneRay(planeArray[index], aRay, position/*anOutIntersectionPoint*/))
			{
				if (aAABB.IsInside(position))
				{
					return true;
				}
			}
		}

		return false;
	}

	// If the ray intersects the AABB, true is returned, if not, false is returned.
	// A ray in one of the AABB's sides is counted as intersecting it.
	template <class T>
	bool IntersectionAABB2DRay(const AABB3D<T>& aAABB, const Ray<T>& aRay/*, Vector3<T>& anOutIntersectionPoint*/)
	{
		std::array< CommonUtilities::Line<T>, 4> lineArray =
		{
			CommonUtilities::Line<T>(CommonUtilities::Vector2<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y),
									CommonUtilities::Vector2<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y)),

			CommonUtilities::Line<T>(CommonUtilities::Vector2<T>(aAABB.GetMinimumPoint().x, aAABB.GetMinimumPoint().y),
									CommonUtilities::Vector2<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y)),

			CommonUtilities::Line<T>(CommonUtilities::Vector2<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y),
									CommonUtilities::Vector2<T>(aAABB.GetMaximumPoint().x, aAABB.GetMinimumPoint().y)),

			CommonUtilities::Line<T>(CommonUtilities::Vector2<T>(aAABB.GetMaximumPoint().x, aAABB.GetMaximumPoint().y),
									CommonUtilities::Vector2<T>(aAABB.GetMinimumPoint().x, aAABB.GetMaximumPoint().y))
		};
		CommonUtilities::Vector3<T> position;
		for (int index = 0; index < lineArray.size(); index++)
		{
			if (IntersectionLineRay(lineArray[index], aRay, position/*anOutIntersectionPoint*/))
			{
				if (aAABB.IsInside(position))
				{
					return true;
				}
			}
		}

		return false;
	}

	// If the ray intersects the sphere, true is returned, if not, false is returned.
	// A ray intersecting the surface of the sphere is considered as intersecting it.
	template <class T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, Vector3<T>& anOutIntersectionPoint)
	{
		T radius = aSphere.GetRadius();
		CommonUtilities::Vector3<T> vectorFromOriginToCenter = aSphere.GetPosition() - aRay.GetOrigin();
		bool returnValue = false;
		if (vectorFromOriginToCenter.LengthSqr() < radius * radius)
		{
			anOutIntersectionPoint = aRay.GetOrigin();
			returnValue = true;
		}
		else
		{
			T projection = vectorFromOriginToCenter.Dot(aRay.GetDirection());
			T projectionEndToIntersection = radius * radius - vectorFromOriginToCenter.LengthSqr() + projection * projection;
			if (projectionEndToIntersection >= 0)
			{
				projectionEndToIntersection = static_cast<T>(std::sqrt(projectionEndToIntersection));
				T originToIntersection = projection - projectionEndToIntersection;
				if (originToIntersection >= 0)
				{
					anOutIntersectionPoint = aRay.GetOrigin() + originToIntersection * aRay.GetDirection();
					returnValue = true;
				}
			}
		}
		return returnValue;
	}
}