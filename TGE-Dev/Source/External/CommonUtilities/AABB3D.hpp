//En Axis - Aligned Bounding Box(AABB) �r, som namnet antyder, en box, vilket ofta ger en
//b�ttre passform runt objekt, med mindre tomrum, �n till exempel en sf�r.Den variant som �r
//axis aligned har alltid sidorna parallella med x - , y - eller z - axeln vilket f�renklar
//intersektionsutr�kningarna(men kan inneb�ra mer tomrum).De definieras oftast av en
//minimum - och en maximumpunkt d�r minimumpunkten inneh�ller AABBns minsta v�rde f�r
//x, y samt z och maximumpunkten inneh�ller dess h�gsta v�rde f�r x, y och z.

#include "Vector3.hpp"
#pragma once

namespace CommonUtilities
{
	template <class T>
	class AABB3D
	{
	public:
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();
		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;
		const CommonUtilities::Vector3<T> GetMinimumPoint() const;
		const CommonUtilities::Vector3<T> GetMaximumPoint() const;

	private:
		CommonUtilities::Vector3<T> myMinimumPoint;
		CommonUtilities::Vector3<T> myMaximumPoint;
	};

	// Default constructor: there is no AABB, both min and max points are the zero vector.
	template<class T>
	inline AABB3D<T>::AABB3D()
	{

	}

	// Copy constructor.
	template<class T>
	inline AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		myMinimumPoint = aAABB3D.GetMinimumPoint();
		myMaximumPoint = aAABB3D.GetMaximumPoint();
	}

	// Constructor taking the positions of the minimum and maximum corners.
	template<class T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMinimumPoint = aMin;
		myMaximumPoint = aMax;
	}

	// Init the AABB with the positions of the minimum and maximum corners, same as the constructor above.
	template<class T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMinimumPoint = aMin;
		myMaximumPoint = aMax;
	}

	// Returns whether a point is inside the AABB: it is inside when the point is on any of the AABB's sides or inside of the AABB.
	template<class T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if (myMinimumPoint.x <= aPosition.x && aPosition.x <= myMaximumPoint.x
			&& myMinimumPoint.y <= aPosition.y && aPosition.y <= myMaximumPoint.y
			&& myMinimumPoint.z <= aPosition.z && aPosition.z <= myMaximumPoint.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template<class T>
	inline const CommonUtilities::Vector3<T> AABB3D<T>::GetMinimumPoint() const
	{
		return myMinimumPoint;
	}

	template<class T>
	inline const CommonUtilities::Vector3<T> AABB3D<T>::GetMaximumPoint() const
	{
		return myMaximumPoint;
	}
}