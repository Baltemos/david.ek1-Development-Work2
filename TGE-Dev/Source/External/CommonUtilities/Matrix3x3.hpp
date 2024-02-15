#include "Vector3.hpp"
#include <initializer_list>
#include <array>
#pragma once
namespace CommonUtilities
{
	template <class T>
	class Matrix4x4;

	template <class T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();
		//Creates a positioned matrix without rotation
		Matrix3x3<T>(const Vector2<T> aVector2Position);
		//Creates a positioned matrix without rotation
		Matrix3x3<T>(const Vector3<T> aVector3Position);
		Matrix3x3<T>(const std::initializer_list<T>& anInitList);
		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		// Creates a transposed copy of the matrix.
		const Matrix3x3<T> GetTranspose() const;
		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);
		const std::array<T, 9> GetOneDimensionalValues() const;
		const std::array<std::array<T, 3>, 3> GetTwoDimensionalValues() const;
		const std::array < Vector3<T>, 3> GetRowVectors() const;
		void SetOneDimensionalValue(int anIndex, T aValue);
		const Vector2<T> GetVector2Position() const;
		const Vector3<T> GetVector3Position() const;
		void SetVector2Position(const Vector2<T> aPosition);
		void SetVector3Position(const Vector3<T> aPosition);
		const std::array<std::array<T, 2>, 2> GetRotationArray() const;
		void SetRotationMatrix(const std::array<std::array<T, 2>, 2> aRotationArray);
		const Matrix3x3<T> GetFastInverse() const;
		// () operator for accessing element (row, column) for read/write or read, 	respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		void operator=(const Matrix3x3<T>& aMatrix3x3);
		bool operator==(const Matrix3x3<T>& aMatrix3x3) const;

	private:

		union
		{
			std::array<T, 9> myOneDimensionalValues;
			std::array<std::array<T, 3>, 3> myTwoDimensionalValues;
			std::array < Vector3<T>, 3> myRowVectors;
		};
	};


	template<class T>
	inline Matrix3x3<T>::Matrix3x3() : myOneDimensionalValues{ 1, 0, 0,
															   0, 1, 0,
															   0, 0, 1 }
	{

	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Vector2<T> aVector2Position) : myOneDimensionalValues{ 1, 0, 0,
																							   0, 1, 0,
																							   aVector3Position.x, aVector3Position.y, 1 }
	{
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Vector3<T> aVector3Position) : myOneDimensionalValues{ 1, 0, 0,
																							   0, 1, 0,
																							   aVector3Position.x, aVector3Position.y, aVector3Position.z }
	{
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const std::initializer_list<T>& anInitList)
	{
		assert(anInitList.size() == 9);
		int index = 0;
		for (T object : anInitList)
		{
			myOneDimensionalValues[index++] = object;
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix) : myOneDimensionalValues(aMatrix.GetOneDimensionalValues())
	{

	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		std::array<T, 16> oneDimensionalValues = aMatrix.GetOneDimensionalValues();
		myOneDimensionalValues[0] = oneDimensionalValues[0];
		myOneDimensionalValues[1] = oneDimensionalValues[1];
		myOneDimensionalValues[2] = oneDimensionalValues[2];
		myOneDimensionalValues[3] = oneDimensionalValues[4];
		myOneDimensionalValues[4] = oneDimensionalValues[5];
		myOneDimensionalValues[5] = oneDimensionalValues[6];
		myOneDimensionalValues[6] = oneDimensionalValues[8];
		myOneDimensionalValues[7] = oneDimensionalValues[9];
		myOneDimensionalValues[8] = oneDimensionalValues[10];
	}

	template<class T>
	inline const Matrix3x3<T> Matrix3x3<T>::GetTranspose() const
	{
		return Matrix3x3<T>({
			myOneDimensionalValues[0], myOneDimensionalValues[3], myOneDimensionalValues[6],
			myOneDimensionalValues[1], myOneDimensionalValues[4], myOneDimensionalValues[7],
			myOneDimensionalValues[2], myOneDimensionalValues[5], myOneDimensionalValues[8] });
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		return Matrix3x3<T>({
			1, 0, 0,
			0, cos(aAngleInRadians), sin(aAngleInRadians),
			0, -sin(aAngleInRadians), cos(aAngleInRadians) });
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		return Matrix3x3<T>({
			cos(aAngleInRadians), 0, -sin(aAngleInRadians),
			0, 1, 0,
			sin(aAngleInRadians), 0, cos(aAngleInRadians) });
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		return Matrix3x3<T>({
			cos(aAngleInRadians), sin(aAngleInRadians), 0,
			-sin(aAngleInRadians), cos(aAngleInRadians), 0,
			0, 0, 1 });
	}

	template<class T>
	inline const std::array<T, 9> Matrix3x3<T>::GetOneDimensionalValues() const
	{
		return myOneDimensionalValues;
	}

	template<class T>
	inline const std::array<std::array<T, 3>, 3> Matrix3x3<T>::GetTwoDimensionalValues() const
	{
		return myTwoDimensionalValues;
	}

	template<class T>
	inline const std::array < Vector3<T>, 3> Matrix3x3<T>::GetRowVectors() const
	{
		return myRowVectors;
	}

	template<class T>
	inline void Matrix3x3<T>::SetOneDimensionalValue(int anIndex, T aValue)
	{
		myOneDimensionalValues[anIndex] = aValue;
	}

	template<class T>
	inline const Vector2<T> Matrix3x3<T>::GetVector2Position() const
	{
		return Vector2<T>(myOneDimensionalValues[6], myOneDimensionalValues[7]);
	}

	template<class T>
	inline const Vector3<T> Matrix3x3<T>::GetVector3Position() const
	{
		return Vector3<T>(myOneDimensionalValues[6], myOneDimensionalValues[7], myOneDimensionalValues[8]);
	}

	template<class T>
	inline void Matrix3x3<T>::SetVector2Position(const Vector2<T> aPosition)
	{
		myOneDimensionalValues[6] = aPosition.x;
		myOneDimensionalValues[7] = aPosition.y;
	}

	template<class T>
	inline void Matrix3x3<T>::SetVector3Position(const Vector3<T> aPosition)
	{
		myOneDimensionalValues[6] = aPosition.x;
		myOneDimensionalValues[7] = aPosition.y;
		myOneDimensionalValues[8] = aPosition.z;
	}

	template<class T>
	inline const std::array<std::array<T, 2>, 2> Matrix3x3<T>::GetRotationArray() const
	{
		return Matrix3x3<T>(myOneDimensionalValues[0], myOneDimensionalValues[1],
			myOneDimensionalValues[3], myOneDimensionalValues[4]);
	}

	template<class T>
	inline void Matrix3x3<T>::SetRotationMatrix(const std::array<std::array<T, 2>, 2> aRotationArray)
	{
		myOneDimensionalValues[0][0] = aRotationArray[0][0];
		myOneDimensionalValues[0][1] = aRotationArray[0][1];
		myOneDimensionalValues[0][0] = aRotationArray[0][0];
		myOneDimensionalValues[0][1] = aRotationArray[0][1];
	}

	template<class T>
	inline const Matrix3x3<T> Matrix3x3<T>::GetFastInverse() const
	{
		Matrix3x3<T> rotationMatrix({
			myOneDimensionalValues[0], myOneDimensionalValues[2], static_cast<T>(0),
			myOneDimensionalValues[1], myOneDimensionalValues[3], static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });

		Matrix3x3<T> translationMatrix({
			static_cast<T>(1),static_cast<T>(0),  static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(1),  static_cast<T>(0),
			-myOneDimensionalValues[6], -myOneDimensionalValues[7], myOneDimensionalValues[8] });

		return translationMatrix * rotationMatrix;
	}

	template<class T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myTwoDimensionalValues[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myTwoDimensionalValues[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline void Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix3x3)
	{
		myOneDimensionalValues = aMatrix3x3.GetOneDimensionalValues();
	}

	template<class T>
	inline bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aMatrix3x3) const
	{
		if (myOneDimensionalValues == aMatrix3x3.GetOneDimensionalValues())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//Returns the Matrix3x3 sum of aMatrix3x3a and aMatrix3x3b
	template <class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix3x3a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = aMatrix3x3a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		return Matrix3x3<T>({
			oneDimensionalValuesA[0] + oneDimensionalValuesB[0], oneDimensionalValuesA[1] + oneDimensionalValuesB[1], oneDimensionalValuesA[2] + oneDimensionalValuesB[2],
			oneDimensionalValuesA[3] + oneDimensionalValuesB[3], oneDimensionalValuesA[4] + oneDimensionalValuesB[4], oneDimensionalValuesA[5] + oneDimensionalValuesB[5],
			oneDimensionalValuesA[6] + oneDimensionalValuesB[6], oneDimensionalValuesA[7] + oneDimensionalValuesB[7], oneDimensionalValuesA[8] + oneDimensionalValuesB[8] });
	}

	//Returns the Matrix3x3 difference of aMatrix3x3a and aMatrix3x3b
	template <class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix3x3a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = aMatrix3x3a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		return Matrix3x3<T>({
			oneDimensionalValuesA[0] - oneDimensionalValuesB[0], oneDimensionalValuesA[1] - oneDimensionalValuesB[1], oneDimensionalValuesA[2] - oneDimensionalValuesB[2],
			oneDimensionalValuesA[3] - oneDimensionalValuesB[3], oneDimensionalValuesA[4] - oneDimensionalValuesB[4], oneDimensionalValuesA[5] - oneDimensionalValuesB[5],
			oneDimensionalValuesA[6] - oneDimensionalValuesB[6], oneDimensionalValuesA[7] - oneDimensionalValuesB[7], oneDimensionalValuesA[8] - oneDimensionalValuesB[8] });
	}

	//Returns the Matrix3x3 aMatrix3x3a component-multiplied by aMatrix3x3b
	template <class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix3x3a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = aMatrix3x3a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		return Matrix3x3<T>({
			oneDimensionalValuesA[0] * oneDimensionalValuesB[0] + oneDimensionalValuesA[1] * oneDimensionalValuesB[3] + oneDimensionalValuesA[2] * oneDimensionalValuesB[6], oneDimensionalValuesA[0] * oneDimensionalValuesB[1] + oneDimensionalValuesA[1] * oneDimensionalValuesB[4] + oneDimensionalValuesA[2] * oneDimensionalValuesB[7], oneDimensionalValuesA[0] * oneDimensionalValuesB[2] + oneDimensionalValuesA[1] * oneDimensionalValuesB[5] + oneDimensionalValuesA[2] * oneDimensionalValuesB[8],
			oneDimensionalValuesA[3] * oneDimensionalValuesB[0] + oneDimensionalValuesA[4] * oneDimensionalValuesB[3] + oneDimensionalValuesA[5] * oneDimensionalValuesB[6], oneDimensionalValuesA[3] * oneDimensionalValuesB[1] + oneDimensionalValuesA[4] * oneDimensionalValuesB[4] + oneDimensionalValuesA[5] * oneDimensionalValuesB[7], oneDimensionalValuesA[3] * oneDimensionalValuesB[2] + oneDimensionalValuesA[4] * oneDimensionalValuesB[5] + oneDimensionalValuesA[5] * oneDimensionalValuesB[8],
			oneDimensionalValuesA[6] * oneDimensionalValuesB[0] + oneDimensionalValuesA[7] * oneDimensionalValuesB[3] + oneDimensionalValuesA[8] * oneDimensionalValuesB[6], oneDimensionalValuesA[6] * oneDimensionalValuesB[1] + oneDimensionalValuesA[7] * oneDimensionalValuesB[4] + oneDimensionalValuesA[8] * oneDimensionalValuesB[7], oneDimensionalValuesA[6] * oneDimensionalValuesB[2] + oneDimensionalValuesA[7] * oneDimensionalValuesB[5] + oneDimensionalValuesA[8] * oneDimensionalValuesB[8] });
	}

	//Returns the Matrix3x3 aMatrix3x3a component-multiplied by aVector3. Vector4 for Matrix4x4.
	template <class T>
	Vector3<T> operator*(const CommonUtilities::Vector3<T>& aVector, const Matrix3x3<T>& aMatrix3x3)
	{
		std::array<T, 9> oneDimensionalValues = aMatrix3x3.GetOneDimensionalValues();
		return Vector3<T>({
			aVector.x * oneDimensionalValues[0] + aVector.y * oneDimensionalValues[3] + aVector.z * oneDimensionalValues[6],
			aVector.x * oneDimensionalValues[1] + aVector.y * oneDimensionalValues[4] + aVector.z * oneDimensionalValues[7],
			aVector.x * oneDimensionalValues[2] + aVector.y * oneDimensionalValues[5] + aVector.z * oneDimensionalValues[8] });
	}

	//Equivalent to setting aMatrix3x3a to (aMatrix3x3a + aMatrix3x3b)
	template <class T>
	void operator+=(Matrix3x3<T>& aMatrix3x3a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = aMatrix3x3a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		aMatrix3x3a = Matrix3x3<T>({
			oneDimensionalValuesA[0] + oneDimensionalValuesB[0], oneDimensionalValuesA[1] + oneDimensionalValuesB[1], oneDimensionalValuesA[2] + oneDimensionalValuesB[2],
			oneDimensionalValuesA[3] + oneDimensionalValuesB[3], oneDimensionalValuesA[4] + oneDimensionalValuesB[4], oneDimensionalValuesA[5] + oneDimensionalValuesB[5],
			oneDimensionalValuesA[6] + oneDimensionalValuesB[6], oneDimensionalValuesA[7] + oneDimensionalValuesB[7], oneDimensionalValuesA[8] + oneDimensionalValuesB[8] });
	}

	//Equivalent to setting aMatrix3x3a to (aMatrix3x3a - aMatrix3x3b)
	template <class T>
	void operator-=(Matrix3x3<T>& aMatrix3x3a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = aMatrix3x3a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		aMatrix3x3a = Matrix3x3<T>({
			oneDimensionalValuesA[0] - oneDimensionalValuesB[0], oneDimensionalValuesA[1] - oneDimensionalValuesB[1], oneDimensionalValuesA[2] - oneDimensionalValuesB[2],
			oneDimensionalValuesA[3] - oneDimensionalValuesB[3], oneDimensionalValuesA[4] - oneDimensionalValuesB[4], oneDimensionalValuesA[5] - oneDimensionalValuesB[5],
			oneDimensionalValuesA[6] - oneDimensionalValuesB[6], oneDimensionalValuesA[7] - oneDimensionalValuesB[7], oneDimensionalValuesA[8] - oneDimensionalValuesB[8] });
	}

	//Equivalent to setting aMatrix3x3 to (aMatrix3x3 * aScalar)
	template <class T>
	void operator*=(Matrix3x3<T>& anOutMatrix4x4a, const Matrix3x3<T>& aMatrix3x3b)
	{
		std::array<T, 9> oneDimensionalValuesA = anOutMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 9> oneDimensionalValuesB = aMatrix3x3b.GetOneDimensionalValues();
		anOutMatrix4x4a = Matrix3x3<T>({
			oneDimensionalValuesA[0] * oneDimensionalValuesB[0] + oneDimensionalValuesA[1] * oneDimensionalValuesB[3] + oneDimensionalValuesA[2] * oneDimensionalValuesB[6], oneDimensionalValuesA[0] * oneDimensionalValuesB[1] + oneDimensionalValuesA[1] * oneDimensionalValuesB[4] + oneDimensionalValuesA[2] * oneDimensionalValuesB[7], oneDimensionalValuesA[0] * oneDimensionalValuesB[2] + oneDimensionalValuesA[1] * oneDimensionalValuesB[5] + oneDimensionalValuesA[2] * oneDimensionalValuesB[8],
			oneDimensionalValuesA[3] * oneDimensionalValuesB[0] + oneDimensionalValuesA[4] * oneDimensionalValuesB[3] + oneDimensionalValuesA[5] * oneDimensionalValuesB[6], oneDimensionalValuesA[3] * oneDimensionalValuesB[1] + oneDimensionalValuesA[4] * oneDimensionalValuesB[4] + oneDimensionalValuesA[5] * oneDimensionalValuesB[7], oneDimensionalValuesA[3] * oneDimensionalValuesB[2] + oneDimensionalValuesA[4] * oneDimensionalValuesB[5] + oneDimensionalValuesA[5] * oneDimensionalValuesB[8],
			oneDimensionalValuesA[6] * oneDimensionalValuesB[0] + oneDimensionalValuesA[7] * oneDimensionalValuesB[3] + oneDimensionalValuesA[8] * oneDimensionalValuesB[6], oneDimensionalValuesA[6] * oneDimensionalValuesB[1] + oneDimensionalValuesA[7] * oneDimensionalValuesB[4] + oneDimensionalValuesA[8] * oneDimensionalValuesB[7], oneDimensionalValuesA[6] * oneDimensionalValuesB[2] + oneDimensionalValuesA[7] * oneDimensionalValuesB[5] + oneDimensionalValuesA[8] * oneDimensionalValuesB[8] });
	}
}