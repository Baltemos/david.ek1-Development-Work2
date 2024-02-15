//Den enklaste formen, sf�r, definieras endast av positionen f�r dess centrum och dess radie.D�
//alla punkter p� sf�rens yta �r lika l�ngt ifr�n centrum(radien) blir intersektionsutr�kningarna
//billigare.Det �r d�rf�r en bra id� att anv�nda sf�rer om man kan.Nackdelen med dem �r att f�r
//m�nga objekt kommer en sf�r som omsluter hela objektet �ven att inneh�lla mycket tomrum
//(t�nk en box som ska omslutas av en sf�r).
#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Sphere
	{
	public:
		// Default constructor: there is no sphere, the radius is zero and the position is  the zero vector.
		Sphere();
		// Copy constructor.
		Sphere(const Sphere<T>& aSphere);
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);
		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;
		const CommonUtilities::Vector3<T> GetPosition() const;
		const T GetRadius() const;

	private:
		CommonUtilities::Vector3<T> myCenterPosition;
		T myRadius;

	};

	// Default constructor: there is no sphere, the radius is zero and the position is  the zero vector.
	template<class T>
	inline Sphere<T>::Sphere()
	{
		myRadius = 0;
	}

	// Copy constructor.
	template<class T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		myCenterPosition = aSphere.GetPosition();
		myRadius = aSphere.GetRadius();
	}

	// Constructor that takes the center position and radius of the sphere.
	template<class T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		myCenterPosition = aCenter;
		myRadius = aRadius;
	}

	// Init the sphere with a center and a radius, the same as the constructor above.
	template<class T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenterPosition = aCenter;
		myRadius = aRadius;
	}

	// Returns whether a point is inside the sphere: it is inside when the point is on the
	// sphere surface or inside of the sphere.
	template<class T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		CommonUtilities::Vector3<T> centerToPosition = aPosition - myCenterPosition;
		if (myRadius * myRadius >= centerToPosition.LengthSqr())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	template<class T>
	inline const CommonUtilities::Vector3<T> Sphere<T>::GetPosition() const
	{
		return myCenterPosition;
	}
	template<class T>
	inline const T Sphere<T>::GetRadius() const
	{
		return myRadius;
	}
}