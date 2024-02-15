//LineVolume representerar en konvex polygon.En punkt är inuti en LineVolume om den är innanför alla linjer som definierar volymen.

#pragma once

#include <vector>
#include "Line.hpp"
#include "Vector2.hpp"
//#include <cmath>
//#include <assert.h>
namespace CommonUtilities
{
	template <class T>
	class LineVolume
	{
	public:
		// Default constructor: empty LineVolume.
		LineVolume();

		// Constructor taking a list of Line that makes up the LineVolume.
		LineVolume(const std::vector<Line<T>>& aLineList);

		// Add a Line to the LineVolume.
		void AddLine(const Line<T>& aLine);

		// Returns whether a point is inside the LineVolume: it is inside when the point is
		// on the line or on the side the normal is pointing away from for all the lines in
		// the LineVolume.
		bool IsInside(const Vector2<T>& aPosition) const;

	private:
		std::vector<Line<T>> myLines;
	};

	// Default constructor: empty LineVolume.
	template<class T>
	inline LineVolume<T>::LineVolume()
	{
		myLines;
	}

	// Constructor taking a list of Line that makes up the LineVolume.
	template<class T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList) : myLines(aLineList)
	{
		//myLines = aLineList;
	}

	// Add a Line to the LineVolume.
	template<class T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		myLines.push_back(aLine);
	}

	// Returns whether a point is inside the LineVolume: it is inside when the point is
	// on the line or on the side. The normal is pointing away from for all the lines in the LineVolume.
	template<class T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition) const
	{
		bool isInside = true;
		for (int index = 0; index < myLines.size(); index++)
		{
			if (myLines[index].IsInside(aPosition) == false)
			{
				isInside = false;
				break;
			}
		}
		return isInside;
	}
}