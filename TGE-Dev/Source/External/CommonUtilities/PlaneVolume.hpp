//Ett PlaneVolume - objekt representerar en konvex polyeder.En punkt är inuti en PlaneVolume om den är innanför alla plan som definierar volymen.

#pragma once
#include <vector>
#include "Plane.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class PlaneVolume
	{
	public:
		// Default constructor: empty PlaneVolume.
		PlaneVolume();

		// Constructor taking a list of Plane that makes up the PlaneVolume.
		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);

		// Add a Plane to the PlaneVolume.
		void AddPlane(const Plane<T>& aPlane);

		// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
		// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
		bool IsInside(const Vector3<T>& aPosition) const;

	private:
		std::vector<Plane<T>> myPlanes;
	};

	// Default constructor: empty PlaneVolume.
	template<class T>
	inline PlaneVolume<T>::PlaneVolume()
	{

	}

	// Constructor taking a list of Plane that makes up the PlaneVolume.
	template<class T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList) : myPlanes(aPlaneList)
	{

	}

	// Add a Plane to the PlaneVolume.
	template<class T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		myPlanes.push_back(aPlane);
	}

	// Returns whether a point is inside the PlaneVolume: it is inside when the point is on the
	// plane or on the side the normal is pointing away from for all the planes in the PlaneVolume.
	template<class T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
	{
		bool isInside = true;
		for (int index = 0; index < myPlanes.size(); index++)
		{
			if (myPlanes[index].IsInside(aPosition) == false)
			{
				isInside = false;
				break;
			}
		}
		return isInside;
	}

}