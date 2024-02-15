//Plane representerar ett riktat plan, dvs.det har en normal som definierar punkterna på ena sidan som 
// Utanför(de punkter som ligger i normalens riktning) och punkterna på andra sidan som Innanför.
// De punkter som ligger i planet räknas som Innanför. Om planet skapas med tre punkter ska normalen ha samma riktning som 
// kryssprodukten mellan aPoint1 - aPoint0 och aPoint2 - aPoint0.
#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Plane
	{
	public:
		// Default constructor.
		Plane();

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 - aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);

		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T>& aPosition) const;

		bool IsTouching(const Vector3<T>& aPosition) const;

		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const;

		const Vector3<T>& GetPoint() const;

	private:
		Vector3<T> myNormal;
		Vector3<T> myPoint;
	};

	// Default constructor.
	template<class T>
	inline Plane<T>::Plane()
	{

	}

	// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2 - aPoint0).
	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		CommonUtilities::Vector3<T> point0To1 = aPoint1 - aPoint0;
		//myNormal = point0To1.Cross(aPoint2 - aPoint0);
		myNormal = point0To1.Cross(aPoint2 - aPoint0).GetNormalized();
		assert(myNormal.Length() > 0.999 && myNormal.Length() < 1.001);
	}

	// Constructor taking a point and a normal.
	template<class T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
	{
		myPoint = aPoint0;
		myNormal = aNormal;
		assert(myNormal.Length() > 0.999 && myNormal.Length() < 1.001);
	}

	// Init the plane with three points, the same as the constructor above.
	template<class T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		myPoint = aPoint0;
		CommonUtilities::Vector3<T> point0To1 = aPoint1 - aPoint0;
		//myNormal = point0To1.Cross(aPoint2 - aPoint0);
		myNormal = point0To1.Cross(aPoint2 - aPoint0).GetNormalized();
		assert(myNormal.Length() > 0.999 && myNormal.Length() < 1.001);
	}

	// Init the plane with a point and a normal, the same as the constructor above.
	template<class T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
		assert(myNormal.Length() > 0.999 && myNormal.Length() < 1.001);
	}

	// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
	template<class T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if (myNormal.Dot(aPosition - myPoint) > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
	template<class T>
	inline bool Plane<T>::IsTouching(const Vector3<T>& aPosition) const
	{
		if (myNormal.Dot(aPosition - myPoint) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// Returns the normal of the plane.
	template<class T>
	inline const Vector3<T>& Plane<T>::GetNormal() const
	{
		//assert(myNormal.Length() > 0.999 && myNormal.Length() < 1.001);
		return myNormal;
	}
	template<class T>
	inline const Vector3<T>& Plane<T>::GetPoint() const
	{
		return myPoint;
	}
}