//En str�le definieras av en utg�ngspunkt, origin, och en riktning, direction.Den fungerar likt en linje, 
//men �r endast o�ndlig i en riktning(l�ngs med direction) till skillnad fr�n en linje som �r o�ndlig �t b�da h�ll.
#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Ray
	{
	public:
		// Default constructor: there is no ray, the origin and direction are the zero vector.
		Ray();
		// Copy constructor.
		Ray(const Ray<T>& aRay);
		// Constructor that takes origin and direction to define the ray
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);
		// Init the ray with two points.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>&
			aDirection);
		const CommonUtilities::Vector3<T> GetOrigin() const;
		const CommonUtilities::Vector3<T> GetDirection() const;

	private:
		CommonUtilities::Vector3<T> myOrigin;
		CommonUtilities::Vector3<T> myDirection;

	};

	// Default constructor: there is no ray, the origin and direction are the zero vector.
	template<class T>
	inline Ray<T>::Ray()
	{

	}

	// Copy constructor.
	template<class T>
	inline Ray<T>::Ray(const Ray<T>& aRay)
	{
		myOrigin = aRay.GetOrigin();
		myDirection = aRay.GetDirection();
		assert(myDirection.Length() > 0.999 && myDirection.Length() < 1.001);
	}

	// Constructor that takes origin and direction to define the ray
	template<class T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection;
		assert(myDirection.Length() > 0.999 && myDirection.Length() < 1.001);
	}

	// Init the ray with two points.
	template<class T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = aOrigin - aPoint;
		myDirection.Normalize();
	}

	// Init the ray with an origin and a direction.
	template<class T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = aDirection;
		assert(myDirection.Length() > 0.999 && myDirection.Length() < 1.001);
	}
	template<class T>
	inline const CommonUtilities::Vector3<T> Ray<T>::GetOrigin() const
	{
		return myOrigin;
	}

	template<class T>
	inline const CommonUtilities::Vector3<T> Ray<T>::GetDirection() const
	{
		return myDirection;
	}
}