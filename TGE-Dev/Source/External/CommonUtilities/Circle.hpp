#pragma once
#include "Vector.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Circle
	{
	public:
		Circle() = default;
		Circle(Vector2<_Ty> aCenter, _Ty aRadius);
		~Circle() = default;

		bool IsInside(const Vector2<_Ty>& aPosition) const;
		const Vector2<_Ty>& GetCenter() const;
		const _Ty GetRadius() const;

	private:
		Vector2<_Ty> myCenter;
		_Ty myRadius;
	};

	template<typename _Ty>
	inline Circle<_Ty>::Circle(Vector2<_Ty> aCenter, _Ty aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
	}

	template<typename _Ty>
	inline bool Circle<_Ty>::IsInside(const Vector2<_Ty>& aPosition) const
	{
		return (aPosition - myCenter).LengthSqr() <= myRadius * myRadius;
	}

	template<typename _Ty>
	inline const Vector2<_Ty>& Circle<_Ty>::GetCenter() const
	{
		return myCenter;
	}

	template<typename _Ty>
	inline const _Ty Circle<_Ty>::GetRadius() const
	{
		return myRadius;
	}

}