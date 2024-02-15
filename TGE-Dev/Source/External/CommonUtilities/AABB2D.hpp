#pragma once
#include "Vector.hpp"
#include <cmath>

namespace CommonUtilities
{
	template<typename _Ty>
	class AABB2D
	{
	public:
		Vector2<_Ty> Min, Max;
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB2D() = default;
		// Copy constructor.
		AABB2D(const AABB2D<_Ty>& aAABB3D) = default;
		// Constructor taking the positions of the minimum and maximum corners.
		AABB2D(const Vector2<_Ty>& aMin, const Vector2<_Ty>& aMax);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector2<_Ty>& aMin, const Vector2<_Ty>& aMax);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector2<_Ty>& aPosition) const;

		bool Intersects(const AABB2D& aAABB2D) const;

		Vector2<_Ty> Size() const;

		AABB2D Intersection(const AABB2D& aAABB2D) const;
	};
	template<typename _Ty>
	inline AABB2D<_Ty>::AABB2D(const Vector2<_Ty>& aMin, const Vector2<_Ty>& aMax)
	{
		this->InitWithMinAndMax(aMin, aMax);
	}
	template<typename _Ty>
	inline void AABB2D<_Ty>::InitWithMinAndMax(const Vector2<_Ty>& aMin, const Vector2<_Ty>& aMax)
	{
		this->Min = aMin;
		this->Max = aMax;
	}
	template<typename _Ty>
	inline bool AABB2D<_Ty>::IsInside(const Vector2<_Ty>& aPosition) const
	{
		if (aPosition.x < this->Min.x || aPosition.y < this->Min.y)
		{
			return false;
		}
		if (aPosition.x > this->Max.x || aPosition.y > this->Max.y)
		{
			return false;
		}

		return true;
	}
	template<typename _Ty>
	inline bool AABB2D<_Ty>::Intersects(const AABB2D<_Ty>& aAABB2D) const
	{
		return !(Min.x > aAABB2D.Max.x ||
			Min.y > aAABB2D.Max.y ||
			Max.x < aAABB2D.Min.x ||
			Max.y < aAABB2D.Min.y);
	}
	template<typename _Ty>
	inline Vector2<_Ty> AABB2D<_Ty>::Size() const
	{
		return Vector2<_Ty>(Max.x - Min.x, Max.y - Min.y);
	}

	template<typename _Ty>
	inline AABB2D<_Ty> AABB2D<_Ty>::Intersection(const AABB2D<_Ty>& aAABB2D) const
	{
		AABB2D res;
		res.Min.x = (std::max)(Min.x, aAABB2D.Min.x);
		res.Min.y = (std::max)(Min.y, aAABB2D.Min.y);
		res.Max.x = (std::min)(Max.x, aAABB2D.Max.x);
		res.Max.y = (std::min)(Max.y, aAABB2D.Max.y);
		return res;
	}
}