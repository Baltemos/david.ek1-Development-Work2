#include "Vector4.hpp"
#include <initializer_list>
#include <array>
#pragma once
namespace CommonUtilities
{
	template <class T>
	class Matrix3x3;

	template <class T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		Matrix4x4<T>(const std::initializer_list<T>& anInitList);
		//Creates a positioned matrix without rotation
		Matrix4x4<T>(const Vector3<T> aVector3Position);
		//Creates a positioned matrix without rotation
		Matrix4x4<T>(const Vector4<T> aVector4Position);
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);
		// Creates a transposed copy of the matrix.
		const Matrix4x4<T> GetTranspose() const;
		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T aAngleInRadians);

		// Author Erik
		static Matrix4x4<T> CreateScale(const Vector3<T>& someScales);
		static Matrix4x4<T> CreateTransform(const Vector3<T>& aPosition, const Vector3<T>& someAngles);

		Vector3<T> GetEulerAnglesZYX() const;

		const std::array<T, 16> GetOneDimensionalValues() const;
		const std::array<std::array<T, 4>, 4> GetTwoDimensionalValues() const;
		const std::array < Vector4<T>, 4> GetRowVectors() const;
		void SetOneDimensionalValue(int anIndex, T aValue);
		const Vector3<T> GetVector3Position() const;
		const Vector4<T> GetVector4Position() const;
		void SetVector3Position(const Vector3<T> aPosition);
		void SetVector4Position(const Vector4<T> aPosition);
		const Matrix3x3<T> GetRotationMatrix() const;
		void SetRotationMatrix(const Matrix3x3<T> aMatrix);
		const Matrix4x4<T> GetFastInverse() const;

		// Author Erik
		Matrix4x4<T> GetInverse() const;

		// () operator for accessing element (row, column) for read/write or read, 	respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		void operator=(const Matrix4x4<T>& aMatrix4x4);
		bool operator==(const Matrix4x4<T>& aMatrix4x4) const;

	private:

		union
		{
			std::array<T, 16> myOneDimensionalValues;
			std::array<std::array<T, 4>, 4> myTwoDimensionalValues;
			std::array < Vector4<T>, 4> myRowVectors;
		};
	};


	template<class T>
	inline Matrix4x4<T>::Matrix4x4() : myOneDimensionalValues{ 1, 0, 0, 0,
															   0, 1, 0, 0,
															   0, 0, 1, 0,
															   0, 0, 0, 1 }
	{

	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const std::initializer_list<T>& anInitList)
	{
		assert(anInitList.size() == 16);
		int index = 0;
		for (T object : anInitList)
		{
			myOneDimensionalValues[index++] = object;
		}
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Vector3<T> aVector3Position) : myOneDimensionalValues{ 1, 0, 0, 0,
																							   0, 1, 0, 0,
																							   0, 0, 1, 0,
																							   aVector3Position.x, aVector3Position.y, aVector3Position.z, 1 }
	{
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Vector4<T> aVector4Position) : myOneDimensionalValues{ 1, 0, 0, 0,
																							   0, 1, 0, 0,
																							   0, 0, 1, 0,
																							   aVector4Position.x, aVector4Position.y, aVector4Position.z, aVector4Position.w }
	{
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix) : myOneDimensionalValues(aMatrix.GetOneDimensionalValues())
	{

	}

	template<class T>
	inline const Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		return Matrix4x4<T>({
			myOneDimensionalValues[0], myOneDimensionalValues[4], myOneDimensionalValues[8], myOneDimensionalValues[12],
			myOneDimensionalValues[1], myOneDimensionalValues[5], myOneDimensionalValues[9], myOneDimensionalValues[13],
			myOneDimensionalValues[2], myOneDimensionalValues[6], myOneDimensionalValues[10], myOneDimensionalValues[14],
			myOneDimensionalValues[3], myOneDimensionalValues[7], myOneDimensionalValues[11], myOneDimensionalValues[15] });
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		return Matrix4x4<T>({
			1, 0, 0, 0,
			0, cos(aAngleInRadians), sin(aAngleInRadians), 0,
			0, -sin(aAngleInRadians), cos(aAngleInRadians), 0,
			0, 0, 0, 1 });
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		return Matrix4x4<T>({
			cos(aAngleInRadians), 0, -sin(aAngleInRadians), 0,
			0, 1, 0, 0,
			sin(aAngleInRadians), 0, cos(aAngleInRadians), 0,
			0, 0, 0, 1 });
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		return Matrix4x4<T>({
			cos(aAngleInRadians), sin(aAngleInRadians), 0,0,
			-sin(aAngleInRadians), cos(aAngleInRadians), 0,0,
			0, 0, 1, 0,
			0, 0, 0, 1 });
	}

	//Author Erik
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScale(const Vector3<T>& someScales)
	{
		Matrix4x4 matrix;
		matrix(1, 1) = someScales.x;
		matrix(2, 2) = someScales.y;
		matrix(3, 3) = someScales.z;
		return matrix;
	}
	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTransform(const Vector3<T>& aPosition, const Vector3<T>& someAngles)
	{
		Matrix4x4 matrix;

		matrix *= CreateRotationAroundY(someAngles.y);
		matrix *= CreateRotationAroundX(someAngles.x);
		matrix *= CreateRotationAroundZ(someAngles.z);

		matrix(4, 1) = aPosition.x;
		matrix(4, 2) = aPosition.y;
		matrix(4, 3) = aPosition.z;
		return matrix;
	}
	template<typename T>
	Vector3<T> Matrix4x4<T>::GetEulerAnglesZYX() const
	{
		const double EPS = 1.0e-6;
		Vector3<T> angles;

		angles.y = asin(myTwoDimensionalValues[0][2]);                      // Unique angle in [-pi/2,pi/2]

		if (abs(abs(myTwoDimensionalValues[0][2]) - 1.0) < EPS)				// Yuk! Gimbal lock. Infinite choice of X and Z
		{
			angles.x = atan2(myTwoDimensionalValues[2][1], myTwoDimensionalValues[1][1]);          // One choice amongst many
			angles.z = 0.0;
		}
		else                                       // Unique solutions in (-pi,pi]
		{
			angles.x = atan2(-myTwoDimensionalValues[1][2], myTwoDimensionalValues[2][2]);         // atan2 gives correct quadrant and unique solutions
			angles.z = atan2(-myTwoDimensionalValues[0][1], myTwoDimensionalValues[0][0]);
		}

		return angles;
	}

	template<class T>
	inline const std::array<T, 16> Matrix4x4<T>::GetOneDimensionalValues() const
	{
		return myOneDimensionalValues;
	}

	template<class T>
	inline const std::array<std::array<T, 4>, 4> Matrix4x4<T>::GetTwoDimensionalValues() const
	{
		return myTwoDimensionalValues;
	}

	template<class T>
	inline const std::array < Vector4<T>, 4> Matrix4x4<T>::GetRowVectors() const
	{
		return myRowVectors;
	}

	template<class T>
	inline void Matrix4x4<T>::SetOneDimensionalValue(int anIndex, T aValue)
	{
		myOneDimensionalValues[anIndex] = aValue;
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::GetVector3Position() const
	{
		return Vector3<T>(myOneDimensionalValues[12], myOneDimensionalValues[13], myOneDimensionalValues[14]);
	}

	template<class T>
	inline const Vector4<T> Matrix4x4<T>::GetVector4Position() const
	{
		return Vector4<T>(myOneDimensionalValues[12], myOneDimensionalValues[13], myOneDimensionalValues[14], myOneDimensionalValues[15]);
	}

	template<class T>
	inline void Matrix4x4<T>::SetVector3Position(const Vector3<T> aPosition)
	{
		myOneDimensionalValues[12] = aPosition.x;
		myOneDimensionalValues[13] = aPosition.y;
		myOneDimensionalValues[14] = aPosition.z;
	}

	template<class T>
	inline void Matrix4x4<T>::SetVector4Position(const Vector4<T> aPosition)
	{
		myOneDimensionalValues[12] = aPosition.x;
		myOneDimensionalValues[13] = aPosition.y;
		myOneDimensionalValues[14] = aPosition.z;
		myOneDimensionalValues[15] = aPosition.w;
	}

	template<class T>
	inline const Matrix3x3<T> Matrix4x4<T>::GetRotationMatrix() const
	{
		return Matrix3x3<T>(myOneDimensionalValues[0], myOneDimensionalValues[1], myOneDimensionalValues[2],
			myOneDimensionalValues[4], myOneDimensionalValues[5], myOneDimensionalValues[6],
			myOneDimensionalValues[8], myOneDimensionalValues[9], myOneDimensionalValues[10]);
	}

	template<class T>
	inline void Matrix4x4<T>::SetRotationMatrix(const Matrix3x3<T> aMatrix)
	{
		myOneDimensionalValues[0] = aMatrix.GetOneDimensionalValues()[0];
		myOneDimensionalValues[1] = aMatrix.GetOneDimensionalValues()[1];
		myOneDimensionalValues[2] = aMatrix.GetOneDimensionalValues()[2];
		myOneDimensionalValues[4] = aMatrix.GetOneDimensionalValues()[4];
		myOneDimensionalValues[5] = aMatrix.GetOneDimensionalValues()[5];
		myOneDimensionalValues[6] = aMatrix.GetOneDimensionalValues()[6];
		myOneDimensionalValues[8] = aMatrix.GetOneDimensionalValues()[8];
		myOneDimensionalValues[9] = aMatrix.GetOneDimensionalValues()[9];
		myOneDimensionalValues[10] = aMatrix.GetOneDimensionalValues()[10];
	}

	template<class T>
	inline const Matrix4x4<T> Matrix4x4<T>::GetFastInverse() const
	{
		Matrix4x4<T> rotationMatrix({
			myOneDimensionalValues[0], myOneDimensionalValues[4], myOneDimensionalValues[8], static_cast<T>(0),
			myOneDimensionalValues[1], myOneDimensionalValues[5], myOneDimensionalValues[9], static_cast<T>(0),
			myOneDimensionalValues[2], myOneDimensionalValues[6], myOneDimensionalValues[10], static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });

		Matrix4x4<T> translationMatrix({
			static_cast<T>(1),static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
			static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
			-myOneDimensionalValues[12], -myOneDimensionalValues[13], -myOneDimensionalValues[14], myOneDimensionalValues[15] });

		return translationMatrix * rotationMatrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetInverse() const
	{
		T A2323 = myTwoDimensionalValues[2][2] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[2][3] * myTwoDimensionalValues[3][2];
		T A1323 = myTwoDimensionalValues[2][1] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[2][3] * myTwoDimensionalValues[3][1];
		T A1223 = myTwoDimensionalValues[2][1] * myTwoDimensionalValues[3][2] - myTwoDimensionalValues[2][2] * myTwoDimensionalValues[3][1];
		T A0323 = myTwoDimensionalValues[2][0] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[2][3] * myTwoDimensionalValues[3][0];
		T A0223 = myTwoDimensionalValues[2][0] * myTwoDimensionalValues[3][2] - myTwoDimensionalValues[2][2] * myTwoDimensionalValues[3][0];
		T A0123 = myTwoDimensionalValues[2][0] * myTwoDimensionalValues[3][1] - myTwoDimensionalValues[2][1] * myTwoDimensionalValues[3][0];
		T A2313 = myTwoDimensionalValues[1][2] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[3][2];
		T A1313 = myTwoDimensionalValues[1][1] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[3][1];
		T A1213 = myTwoDimensionalValues[1][1] * myTwoDimensionalValues[3][2] - myTwoDimensionalValues[1][2] * myTwoDimensionalValues[3][1];
		T A2312 = myTwoDimensionalValues[1][2] * myTwoDimensionalValues[2][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[2][2];
		T A1312 = myTwoDimensionalValues[1][1] * myTwoDimensionalValues[2][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[2][1];
		T A1212 = myTwoDimensionalValues[1][1] * myTwoDimensionalValues[2][2] - myTwoDimensionalValues[1][2] * myTwoDimensionalValues[2][1];
		T A0313 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[3][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[3][0];
		T A0213 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[3][2] - myTwoDimensionalValues[1][2] * myTwoDimensionalValues[3][0];
		T A0312 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[2][3] - myTwoDimensionalValues[1][3] * myTwoDimensionalValues[2][0];
		T A0212 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[2][2] - myTwoDimensionalValues[1][2] * myTwoDimensionalValues[2][0];
		T A0113 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[3][1] - myTwoDimensionalValues[1][1] * myTwoDimensionalValues[3][0];
		T A0112 = myTwoDimensionalValues[1][0] * myTwoDimensionalValues[2][1] - myTwoDimensionalValues[1][1] * myTwoDimensionalValues[2][0];

		T det = myTwoDimensionalValues[0][0] * (myTwoDimensionalValues[1][1] * A2323 - myTwoDimensionalValues[1][2] * A1323 + myTwoDimensionalValues[1][3] * A1223)
			- myTwoDimensionalValues[0][1] * (myTwoDimensionalValues[1][0] * A2323 - myTwoDimensionalValues[1][2] * A0323 + myTwoDimensionalValues[1][3] * A0223)
			+ myTwoDimensionalValues[0][2] * (myTwoDimensionalValues[1][0] * A1323 - myTwoDimensionalValues[1][1] * A0323 + myTwoDimensionalValues[1][3] * A0123)
			- myTwoDimensionalValues[0][3] * (myTwoDimensionalValues[1][0] * A1223 - myTwoDimensionalValues[1][1] * A0223 + myTwoDimensionalValues[1][2] * A0123);
		det = T(1) / det;

		Matrix4x4<T> im;

		im.myTwoDimensionalValues[0][0] = det * (myTwoDimensionalValues[1][1] * A2323 - myTwoDimensionalValues[1][2] * A1323 + myTwoDimensionalValues[1][3] * A1223);
		im.myTwoDimensionalValues[0][1] = det * -(myTwoDimensionalValues[0][1] * A2323 - myTwoDimensionalValues[0][2] * A1323 + myTwoDimensionalValues[0][3] * A1223);
		im.myTwoDimensionalValues[0][2] = det * (myTwoDimensionalValues[0][1] * A2313 - myTwoDimensionalValues[0][2] * A1313 + myTwoDimensionalValues[0][3] * A1213);
		im.myTwoDimensionalValues[0][3] = det * -(myTwoDimensionalValues[0][1] * A2312 - myTwoDimensionalValues[0][2] * A1312 + myTwoDimensionalValues[0][3] * A1212);
		im.myTwoDimensionalValues[1][0] = det * -(myTwoDimensionalValues[1][0] * A2323 - myTwoDimensionalValues[1][2] * A0323 + myTwoDimensionalValues[1][3] * A0223);
		im.myTwoDimensionalValues[1][1] = det * (myTwoDimensionalValues[0][0] * A2323 - myTwoDimensionalValues[0][2] * A0323 + myTwoDimensionalValues[0][3] * A0223);
		im.myTwoDimensionalValues[1][2] = det * -(myTwoDimensionalValues[0][0] * A2313 - myTwoDimensionalValues[0][2] * A0313 + myTwoDimensionalValues[0][3] * A0213);
		im.myTwoDimensionalValues[1][3] = det * (myTwoDimensionalValues[0][0] * A2312 - myTwoDimensionalValues[0][2] * A0312 + myTwoDimensionalValues[0][3] * A0212);
		im.myTwoDimensionalValues[2][0] = det * (myTwoDimensionalValues[1][0] * A1323 - myTwoDimensionalValues[1][1] * A0323 + myTwoDimensionalValues[1][3] * A0123);
		im.myTwoDimensionalValues[2][1] = det * -(myTwoDimensionalValues[0][0] * A1323 - myTwoDimensionalValues[0][1] * A0323 + myTwoDimensionalValues[0][3] * A0123);
		im.myTwoDimensionalValues[2][2] = det * (myTwoDimensionalValues[0][0] * A1313 - myTwoDimensionalValues[0][1] * A0313 + myTwoDimensionalValues[0][3] * A0113);
		im.myTwoDimensionalValues[2][3] = det * -(myTwoDimensionalValues[0][0] * A1312 - myTwoDimensionalValues[0][1] * A0312 + myTwoDimensionalValues[0][3] * A0112);
		im.myTwoDimensionalValues[3][0] = det * -(myTwoDimensionalValues[1][0] * A1223 - myTwoDimensionalValues[1][1] * A0223 + myTwoDimensionalValues[1][2] * A0123);
		im.myTwoDimensionalValues[3][1] = det * (myTwoDimensionalValues[0][0] * A1223 - myTwoDimensionalValues[0][1] * A0223 + myTwoDimensionalValues[0][2] * A0123);
		im.myTwoDimensionalValues[3][2] = det * -(myTwoDimensionalValues[0][0] * A1213 - myTwoDimensionalValues[0][1] * A0213 + myTwoDimensionalValues[0][2] * A0113);
		im.myTwoDimensionalValues[3][3] = det * (myTwoDimensionalValues[0][0] * A1212 - myTwoDimensionalValues[0][1] * A0212 + myTwoDimensionalValues[0][2] * A0112);

		return im;
	}

	template<class T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myTwoDimensionalValues[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myTwoDimensionalValues[aRow - 1][aColumn - 1];
	}

	template<class T>
	inline void Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix4x4)
	{
		myOneDimensionalValues = aMatrix4x4.GetOneDimensionalValues();
	}

	template<class T>
	inline bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aMatrix4x4) const
	{
		if (myOneDimensionalValues == aMatrix4x4.GetOneDimensionalValues())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//Returns the Matrix4x4 sum of aMatrix4x4a and aMatrix4x4b
	template <class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = aMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		return Matrix4x4<T>({
			oneDimensionalValuesA[0] + oneDimensionalValuesB[0], oneDimensionalValuesA[1] + oneDimensionalValuesB[1], oneDimensionalValuesA[2] + oneDimensionalValuesB[2], oneDimensionalValuesA[3] + oneDimensionalValuesB[3],
			oneDimensionalValuesA[4] + oneDimensionalValuesB[4], oneDimensionalValuesA[5] + oneDimensionalValuesB[5], oneDimensionalValuesA[6] + oneDimensionalValuesB[6], oneDimensionalValuesA[7] + oneDimensionalValuesB[7],
			oneDimensionalValuesA[8] + oneDimensionalValuesB[8], oneDimensionalValuesA[9] + oneDimensionalValuesB[9], oneDimensionalValuesA[10] + oneDimensionalValuesB[10], oneDimensionalValuesA[11] + oneDimensionalValuesB[11],
			oneDimensionalValuesA[12] + oneDimensionalValuesB[12], oneDimensionalValuesA[13] + oneDimensionalValuesB[13], oneDimensionalValuesA[14] + oneDimensionalValuesB[14], oneDimensionalValuesA[15] + oneDimensionalValuesB[15] });
	}

	//Returns the Matrix4x4 difference of aMatrix4x4a and aMatrix4x4b
	template <class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = aMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		return Matrix4x4<T>({
			oneDimensionalValuesA[0] - oneDimensionalValuesB[0], oneDimensionalValuesA[1] - oneDimensionalValuesB[1], oneDimensionalValuesA[2] - oneDimensionalValuesB[2], oneDimensionalValuesA[3] - oneDimensionalValuesB[3],
			oneDimensionalValuesA[4] - oneDimensionalValuesB[4], oneDimensionalValuesA[5] - oneDimensionalValuesB[5], oneDimensionalValuesA[6] - oneDimensionalValuesB[6], oneDimensionalValuesA[7] - oneDimensionalValuesB[7],
			oneDimensionalValuesA[8] - oneDimensionalValuesB[8], oneDimensionalValuesA[9] - oneDimensionalValuesB[9], oneDimensionalValuesA[10] - oneDimensionalValuesB[10], oneDimensionalValuesA[11] - oneDimensionalValuesB[11],
			oneDimensionalValuesA[12] - oneDimensionalValuesB[12], oneDimensionalValuesA[13] - oneDimensionalValuesB[13], oneDimensionalValuesA[14] - oneDimensionalValuesB[14], oneDimensionalValuesA[15] - oneDimensionalValuesB[15] });
	}

	//Returns the Matrix4x4 aMatrix4x4a component-multiplied by aMatrix4x4b
	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = aMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		return Matrix4x4<T>({
			oneDimensionalValuesA[0] * oneDimensionalValuesB[0] + oneDimensionalValuesA[1] * oneDimensionalValuesB[4] + oneDimensionalValuesA[2] * oneDimensionalValuesB[8] + oneDimensionalValuesA[3] * oneDimensionalValuesB[12], oneDimensionalValuesA[0] * oneDimensionalValuesB[1] + oneDimensionalValuesA[1] * oneDimensionalValuesB[5] + oneDimensionalValuesA[2] * oneDimensionalValuesB[9] + oneDimensionalValuesA[3] * oneDimensionalValuesB[13], oneDimensionalValuesA[0] * oneDimensionalValuesB[2] + oneDimensionalValuesA[1] * oneDimensionalValuesB[6] + oneDimensionalValuesA[2] * oneDimensionalValuesB[10] + oneDimensionalValuesA[3] * oneDimensionalValuesB[14], oneDimensionalValuesA[0] * oneDimensionalValuesB[3] + oneDimensionalValuesA[1] * oneDimensionalValuesB[7] + oneDimensionalValuesA[2] * oneDimensionalValuesB[11] + oneDimensionalValuesA[3] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[4] * oneDimensionalValuesB[0] + oneDimensionalValuesA[5] * oneDimensionalValuesB[4] + oneDimensionalValuesA[6] * oneDimensionalValuesB[8] + oneDimensionalValuesA[7] * oneDimensionalValuesB[12], oneDimensionalValuesA[4] * oneDimensionalValuesB[1] + oneDimensionalValuesA[5] * oneDimensionalValuesB[5] + oneDimensionalValuesA[6] * oneDimensionalValuesB[9] + oneDimensionalValuesA[7] * oneDimensionalValuesB[13], oneDimensionalValuesA[4] * oneDimensionalValuesB[2] + oneDimensionalValuesA[5] * oneDimensionalValuesB[6] + oneDimensionalValuesA[6] * oneDimensionalValuesB[10] + oneDimensionalValuesA[7] * oneDimensionalValuesB[14], oneDimensionalValuesA[4] * oneDimensionalValuesB[3] + oneDimensionalValuesA[5] * oneDimensionalValuesB[7] + oneDimensionalValuesA[6] * oneDimensionalValuesB[11] + oneDimensionalValuesA[7] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[8] * oneDimensionalValuesB[0] + oneDimensionalValuesA[9] * oneDimensionalValuesB[4] + oneDimensionalValuesA[10] * oneDimensionalValuesB[8] + oneDimensionalValuesA[11] * oneDimensionalValuesB[12], oneDimensionalValuesA[8] * oneDimensionalValuesB[1] + oneDimensionalValuesA[9] * oneDimensionalValuesB[5] + oneDimensionalValuesA[10] * oneDimensionalValuesB[9] + oneDimensionalValuesA[11] * oneDimensionalValuesB[13], oneDimensionalValuesA[8] * oneDimensionalValuesB[2] + oneDimensionalValuesA[9] * oneDimensionalValuesB[6] + oneDimensionalValuesA[10] * oneDimensionalValuesB[10] + oneDimensionalValuesA[11] * oneDimensionalValuesB[14], oneDimensionalValuesA[8] * oneDimensionalValuesB[3] + oneDimensionalValuesA[9] * oneDimensionalValuesB[7] + oneDimensionalValuesA[10] * oneDimensionalValuesB[11] + oneDimensionalValuesA[11] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[12] * oneDimensionalValuesB[0] + oneDimensionalValuesA[13] * oneDimensionalValuesB[4] + oneDimensionalValuesA[14] * oneDimensionalValuesB[8] + oneDimensionalValuesA[15] * oneDimensionalValuesB[12], oneDimensionalValuesA[12] * oneDimensionalValuesB[1] + oneDimensionalValuesA[13] * oneDimensionalValuesB[5] + oneDimensionalValuesA[14] * oneDimensionalValuesB[9] + oneDimensionalValuesA[15] * oneDimensionalValuesB[13], oneDimensionalValuesA[12] * oneDimensionalValuesB[2] + oneDimensionalValuesA[13] * oneDimensionalValuesB[6] + oneDimensionalValuesA[14] * oneDimensionalValuesB[10] + oneDimensionalValuesA[15] * oneDimensionalValuesB[14], oneDimensionalValuesA[12] * oneDimensionalValuesB[3] + oneDimensionalValuesA[13] * oneDimensionalValuesB[7] + oneDimensionalValuesA[14] * oneDimensionalValuesB[11] + oneDimensionalValuesA[15] * oneDimensionalValuesB[15] });
	}

	//Returns the Matrix4x4 aMatrix4x4a component-multiplied by aVector3. Vector4 for Matrix4x4.
	template <class T>
	Vector4<T> operator*(const CommonUtilities::Vector4<T>& aVector, const Matrix4x4<T>& aMatrix4x4)
	{
		std::array<T, 16> oneDimensionalValues = aMatrix4x4.GetOneDimensionalValues();
		return Vector4<T>({
			aVector.x * oneDimensionalValues[0] + aVector.y * oneDimensionalValues[4] + aVector.z * oneDimensionalValues[8] + aVector.w * oneDimensionalValues[12],
			aVector.x * oneDimensionalValues[1] + aVector.y * oneDimensionalValues[5] + aVector.z * oneDimensionalValues[9] + aVector.w * oneDimensionalValues[13],
			aVector.x * oneDimensionalValues[2] + aVector.y * oneDimensionalValues[6] + aVector.z * oneDimensionalValues[10] + aVector.w * oneDimensionalValues[14],
			aVector.x * oneDimensionalValues[3] + aVector.y * oneDimensionalValues[7] + aVector.z * oneDimensionalValues[11] + aVector.w * oneDimensionalValues[15] });
	}

	//Equivalent to setting aMatrix4x4a to (aMatrix4x4a + aMatrix4x4b)
	template <class T>
	void operator+=(Matrix4x4<T>& aMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = aMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		aMatrix4x4a = Matrix4x4<T>({
			oneDimensionalValuesA[0] + oneDimensionalValuesB[0], oneDimensionalValuesA[1] + oneDimensionalValuesB[1], oneDimensionalValuesA[2] + oneDimensionalValuesB[2], oneDimensionalValuesA[3] + oneDimensionalValuesB[3],
			oneDimensionalValuesA[4] + oneDimensionalValuesB[4], oneDimensionalValuesA[5] + oneDimensionalValuesB[5], oneDimensionalValuesA[6] + oneDimensionalValuesB[6], oneDimensionalValuesA[7] + oneDimensionalValuesB[7],
			oneDimensionalValuesA[8] + oneDimensionalValuesB[8], oneDimensionalValuesA[9] + oneDimensionalValuesB[9], oneDimensionalValuesA[10] + oneDimensionalValuesB[10], oneDimensionalValuesA[11] + oneDimensionalValuesB[11],
			oneDimensionalValuesA[12] + oneDimensionalValuesB[12], oneDimensionalValuesA[13] + oneDimensionalValuesB[13], oneDimensionalValuesA[14] + oneDimensionalValuesB[14], oneDimensionalValuesA[15] + oneDimensionalValuesB[15] });
	}

	//Equivalent to setting aMatrix4x4a to (aMatrix4x4a - aMatrix4x4b)
	template <class T>
	void operator-=(Matrix4x4<T>& aMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = aMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		aMatrix4x4a = Matrix4x4<T>({
			oneDimensionalValuesA[0] - oneDimensionalValuesB[0], oneDimensionalValuesA[1] - oneDimensionalValuesB[1], oneDimensionalValuesA[2] - oneDimensionalValuesB[2], oneDimensionalValuesA[3] - oneDimensionalValuesB[3],
			oneDimensionalValuesA[4] - oneDimensionalValuesB[4], oneDimensionalValuesA[5] - oneDimensionalValuesB[5], oneDimensionalValuesA[6] - oneDimensionalValuesB[6], oneDimensionalValuesA[7] - oneDimensionalValuesB[7],
			oneDimensionalValuesA[8] - oneDimensionalValuesB[8], oneDimensionalValuesA[9] - oneDimensionalValuesB[9], oneDimensionalValuesA[10] - oneDimensionalValuesB[10], oneDimensionalValuesA[11] - oneDimensionalValuesB[11],
			oneDimensionalValuesA[12] - oneDimensionalValuesB[12], oneDimensionalValuesA[13] - oneDimensionalValuesB[13], oneDimensionalValuesA[14] - oneDimensionalValuesB[14], oneDimensionalValuesA[15] - oneDimensionalValuesB[15] });
	}

	//Equivalent to setting aMatrix4x4 to (aMatrix4x4 * aMatrix4x4b)
	template <class T>
	void operator*=(Matrix4x4<T>& anOutMatrix4x4a, const Matrix4x4<T>& aMatrix4x4b)
	{
		std::array<T, 16> oneDimensionalValuesA = anOutMatrix4x4a.GetOneDimensionalValues();
		std::array<T, 16> oneDimensionalValuesB = aMatrix4x4b.GetOneDimensionalValues();
		anOutMatrix4x4a = Matrix4x4<T>({
						oneDimensionalValuesA[0] * oneDimensionalValuesB[0] + oneDimensionalValuesA[1] * oneDimensionalValuesB[4] + oneDimensionalValuesA[2] * oneDimensionalValuesB[8] + oneDimensionalValuesA[3] * oneDimensionalValuesB[12], oneDimensionalValuesA[0] * oneDimensionalValuesB[1] + oneDimensionalValuesA[1] * oneDimensionalValuesB[5] + oneDimensionalValuesA[2] * oneDimensionalValuesB[9] + oneDimensionalValuesA[3] * oneDimensionalValuesB[13], oneDimensionalValuesA[0] * oneDimensionalValuesB[2] + oneDimensionalValuesA[1] * oneDimensionalValuesB[6] + oneDimensionalValuesA[2] * oneDimensionalValuesB[10] + oneDimensionalValuesA[3] * oneDimensionalValuesB[14], oneDimensionalValuesA[0] * oneDimensionalValuesB[3] + oneDimensionalValuesA[1] * oneDimensionalValuesB[7] + oneDimensionalValuesA[2] * oneDimensionalValuesB[11] + oneDimensionalValuesA[3] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[4] * oneDimensionalValuesB[0] + oneDimensionalValuesA[5] * oneDimensionalValuesB[4] + oneDimensionalValuesA[6] * oneDimensionalValuesB[8] + oneDimensionalValuesA[7] * oneDimensionalValuesB[12], oneDimensionalValuesA[4] * oneDimensionalValuesB[1] + oneDimensionalValuesA[5] * oneDimensionalValuesB[5] + oneDimensionalValuesA[6] * oneDimensionalValuesB[9] + oneDimensionalValuesA[7] * oneDimensionalValuesB[13], oneDimensionalValuesA[4] * oneDimensionalValuesB[2] + oneDimensionalValuesA[5] * oneDimensionalValuesB[6] + oneDimensionalValuesA[6] * oneDimensionalValuesB[10] + oneDimensionalValuesA[7] * oneDimensionalValuesB[14], oneDimensionalValuesA[4] * oneDimensionalValuesB[3] + oneDimensionalValuesA[5] * oneDimensionalValuesB[7] + oneDimensionalValuesA[6] * oneDimensionalValuesB[11] + oneDimensionalValuesA[7] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[8] * oneDimensionalValuesB[0] + oneDimensionalValuesA[9] * oneDimensionalValuesB[4] + oneDimensionalValuesA[10] * oneDimensionalValuesB[8] + oneDimensionalValuesA[11] * oneDimensionalValuesB[12], oneDimensionalValuesA[8] * oneDimensionalValuesB[1] + oneDimensionalValuesA[9] * oneDimensionalValuesB[5] + oneDimensionalValuesA[10] * oneDimensionalValuesB[9] + oneDimensionalValuesA[11] * oneDimensionalValuesB[13], oneDimensionalValuesA[8] * oneDimensionalValuesB[2] + oneDimensionalValuesA[9] * oneDimensionalValuesB[6] + oneDimensionalValuesA[10] * oneDimensionalValuesB[10] + oneDimensionalValuesA[11] * oneDimensionalValuesB[14], oneDimensionalValuesA[8] * oneDimensionalValuesB[3] + oneDimensionalValuesA[9] * oneDimensionalValuesB[7] + oneDimensionalValuesA[10] * oneDimensionalValuesB[11] + oneDimensionalValuesA[11] * oneDimensionalValuesB[15],
			oneDimensionalValuesA[12] * oneDimensionalValuesB[0] + oneDimensionalValuesA[13] * oneDimensionalValuesB[4] + oneDimensionalValuesA[14] * oneDimensionalValuesB[8] + oneDimensionalValuesA[15] * oneDimensionalValuesB[12], oneDimensionalValuesA[12] * oneDimensionalValuesB[1] + oneDimensionalValuesA[13] * oneDimensionalValuesB[5] + oneDimensionalValuesA[14] * oneDimensionalValuesB[9] + oneDimensionalValuesA[15] * oneDimensionalValuesB[13], oneDimensionalValuesA[12] * oneDimensionalValuesB[2] + oneDimensionalValuesA[13] * oneDimensionalValuesB[6] + oneDimensionalValuesA[14] * oneDimensionalValuesB[10] + oneDimensionalValuesA[15] * oneDimensionalValuesB[14], oneDimensionalValuesA[12] * oneDimensionalValuesB[3] + oneDimensionalValuesA[13] * oneDimensionalValuesB[7] + oneDimensionalValuesA[14] * oneDimensionalValuesB[11] + oneDimensionalValuesA[15] * oneDimensionalValuesB[15] });
	}

}