//Ett Line - objekt representerar en riktad oändlig linje.
// Därmed har den en riktning(aDirection - argumentet, eller aPoint1 - aPoint0).
// Den har en normal som definieras som(-aDirection.y, aDirection.x).
// De punkter som ligger på den sida dit normalen pekar är Utanför, de punkter som ligger på linjen eller på sidan normalen pekar ifrån är Innanför.
//Medlemsvariabler väljer ni själva.Ett par olika förslag finns nedan(ett förslag kan räcka) :
//	•	2 punkter
//	•	Punkt + riktning
//	•	3 tal(konstanterna i linjens implicita ekvation)
//	•	Punkt + normal

#pragma once

#include "Vector2.hpp"
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Line
	{
	public:
		// Default constructor: there is no line, the normal is the zero vector.
		Line();

		// Copy constructor.
		Line(const Line <T>& aLine);

		// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with two points, the same as the constructor above.
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);

		// Init the line with a point and a direction.
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
		bool IsInside(const Vector2<T>& aPosition) const;

		// Returns the direction of the line.
		const Vector2<T>& GetDirection() const;

		// Returns the normal of the line, which is (-direction.y, direction.x).
		const Vector2<T> GetNormal() const;

		const Vector2<T>GetPoint() const;

	private:
		CommonUtilities::Vector2<T> myPoint;
		CommonUtilities::Vector2<T> myDirection;
	};

	// Default constructor: there is no line, the normal is the zero vector.
	template<class T>
	inline Line<T>::Line()
	{

	}

	// Copy constructor.
	template<class T>
	inline Line<T>::Line(const Line<T>& aLine)
	{
		myPoint = aLine.GetPoint();
		myDirection = aLine.GetDirection();
		assert(myDirection.Length() > 0.999 && myDirection.Length() < 1.001);
	}

	// Constructor that takes two points that define the line, the direction is aPoint1 - aPoint0.
	template<class T>
	inline Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		InitWithPointAndDirection(aPoint0, aPoint1 - aPoint0);
	}

	// Init the line with two points, the same as the constructor above.
	template<class T>
	inline void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		InitWithPointAndDirection(aPoint0, aPoint1 - aPoint0);
	}

	// Init the line with a point and a direction.
	template<class T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPoint = aPoint;
		myDirection = aDirection;
		myDirection.Normalize();
	}

	// Returns whether a point is inside the line: it is inside when the point is on the line or on the side the normal is pointing away from.
	template<class T>
	inline bool Line<T>::IsInside(const Vector2<T>& aPosition) const
	{
		CommonUtilities::Vector2<T> line = CommonUtilities::Vector2<T>(aPosition.x - myPoint.x, aPosition.y - myPoint.y);
		if (line.Dot(GetNormal()) > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// Returns the direction of the line.
	template<class T>
	inline const Vector2<T>& Line<T>::GetDirection() const
	{
		return myDirection;
	}

	// Returns the normal of the line, which is (-direction.y, direction.x).
	template<class T>
	inline const Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-myDirection.y, myDirection.x);
	}

	template<class T>
	inline const Vector2<T> Line<T>::GetPoint() const
	{
		return myPoint;
	}
}