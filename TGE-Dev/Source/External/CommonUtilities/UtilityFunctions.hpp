#pragma once
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	T Max(const T& aFirstNumber, const T& aSecondNumber)
	{
		T returnValue;
		if (aFirstNumber > aSecondNumber)
		{
			returnValue = aFirstNumber;
		}
		else
		{
			returnValue = aSecondNumber;
		}
		return returnValue;
	}

	template <class T>
	T Min(const T& aFirstNumber, const T& aSecondNumber)
	{
		T returnValue;
		if (aFirstNumber > aSecondNumber)
		{
			returnValue = aSecondNumber;
		}
		else
		{
			returnValue = aFirstNumber;
		}
		return returnValue;
	}

	template <class T>
	T Abs(const T& aValue)
	{
		T value = aValue;
		if (value < 0)
		{
			value = -value;
		}
		return value;
	}

	template <class T>
	T Clamp(const T& aValue, const T& aMinimum, const T& aMaximum)
	{
		T value = aValue;
		assert(aMaximum >= aMinimum);
		if (aMinimum > aMaximum)
		{
			//assert(false);
		}
		if (value < aMinimum)
		{
			value = aMinimum;
		}
		else if (value > aMaximum)
		{
			value = aMaximum;
		}
		return value;
	}

	template <class T>
	T Lerp(const T& aCurrentValue, const T& aTargetValue, const float aProgress)
	{
		T value = aCurrentValue + aProgress * (aTargetValue - aCurrentValue);
		return value;
	}

	template <class T>
	void Swap(T& anOutFirstNumber, T& anOutSecondNumber)
	{
		T numberToSwapTo = anOutFirstNumber;
		anOutFirstNumber = anOutSecondNumber;
		anOutSecondNumber = numberToSwapTo;
	}
}