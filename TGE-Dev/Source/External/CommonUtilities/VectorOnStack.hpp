#pragma once
#include <array>
#include <assert.h>
#include <string.h>
#include <string>
#include <type_traits>
#include <initializer_list>


namespace CommonUtilities
{
	template <typename Type, int SIZE, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const std::initializer_list<Type>& anInitList);
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		~VectorOnStack();
		VectorOnStack& operator=(const VectorOnStack& aVectorOnStack);
		inline const Type& operator[](const CountType anIndex) const;
		inline Type& operator[](const CountType anIndex);
		inline void Add(const Type& anObject);
		inline void Insert(const CountType anIndex, const Type& anObject);
		inline void RemoveCyclic(const Type& anObject);
		inline void RemoveCyclicAtIndex(const CountType anIndex);
		inline void Clear();
		inline CountType Size() const;

	private:
		static_assert(!(std::is_same<Type, std::string>::value) || UseSafeModeFlag == true);
		std::array<Type, SIZE> myArray;
		CountType myCountType;
		CountType myCurrentArraySize;
	};

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::VectorOnStack() : myArray(), myCountType(), myCurrentArraySize()           //Konstruktor
	{

	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::VectorOnStack(const std::initializer_list<Type>& anInitList) : myArray(), myCountType(), myCurrentArraySize() //Konstruktor initlist
	{
		assert(anInitList.size() <= myArray.size());
		for (Type anObject : anInitList)
		{
			Add(anObject);
		}
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)                 //Copy-konstruktor
	{
		if (UseSafeModeFlag)
		{
			*this = aVectorOnStack;
		}
		else
		{
			memcpy(this, &aVectorOnStack, sizeof(aVectorOnStack));
		}
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::~VectorOnStack()                                                   //Destruktor
	{
		Clear();
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator=(const VectorOnStack& aVectorOnStack)        //Operator=
	{
		if (UseSafeModeFlag)
		{
			myArray = aVectorOnStack.myArray;
			myCountType = aVectorOnStack.myCountType;
			myCurrentArraySize = aVectorOnStack.myCurrentArraySize;
		}
		else
		{
			memcpy(this, &aVectorOnStack, sizeof(aVectorOnStack));
		}
		return *this;
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline Type& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator[](const CountType anIndex)                           //Operator[]
	{
		if (anIndex < 0 || anIndex >= myCurrentArraySize)
		{
			assert(false);
		}

		return myArray[anIndex];
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Add(const Type& anObject)                                      //Add
	{
		assert(myCurrentArraySize < myArray.size());
		if (UseSafeModeFlag)
		{
			myArray[myCurrentArraySize] = anObject;
		}
		else
		{
			memcpy(&myArray[myCurrentArraySize], &anObject, sizeof(anObject));
		}
		myCurrentArraySize++;
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Insert(const CountType anIndex, const Type& anObject)            //Insert
	{
		assert(anIndex < myArray.size() && anIndex <= myCurrentArraySize && anIndex >= 0);

		if (anIndex == myCurrentArraySize)
		{
			Add(anObject);
		}
		else
		{
			for (int index = myCurrentArraySize - 1; index >= 0; index--)
			{
				if (UseSafeModeFlag)
				{
					myArray[index + 1] = myArray[index];
				}
				else
				{
					memcpy(&myArray[index + 1], &myArray[index], sizeof(myArray[index]));
				}
				if (index == anIndex)
				{
					if (UseSafeModeFlag)
					{
						myArray[index] = anObject;
					}
					else
					{
						memcpy(&myArray[myCurrentArraySize], &anObject, sizeof(anObject));
					}
					myCurrentArraySize++;
					break;
				}
			}
		}
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& anObject)                              //RemoveCyclic
	{
		for (int index = 0; index < myCurrentArraySize; index++)
		{
			if (anObject == myArray[index])
			{
				myArray[index] = myArray[--myCurrentArraySize];
				break;
			}
		}
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType anIndex)                    //RemoveCylicAtIndex
	{
		assert(anIndex < myCurrentArraySize && anIndex >= 0);
		myArray[anIndex] = myArray[--myCurrentArraySize];
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Clear()                                                        //Clear
	{
		myCurrentArraySize = 0;
	}

	template<typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	inline CountType VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Size() const                                              //Size
	{
		return myCurrentArraySize;
	}
}