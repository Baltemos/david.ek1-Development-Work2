#pragma once
#include <cmath>
#include <assert.h>
namespace CommonUtilities
{
	template <class T>
	class Vector2;
	template <class T>
	class Vector4;

	template <class T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;
		//Creates a null-vector
		Vector3<T>();
		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ);
		//Copy constructor (compiler generated)
		Vector3<T>(const Vector3<T>& aVector) = default;

		Vector3<T>(const Vector2<T> aVector2, const T& aZ);
		//Assignment operator (compiler generated)
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;
		//Destructor (compiler generated)
		~Vector3<T>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector3<T> from this CommonUtillities::Vector3<T>
		template <class OtherVectorClass>
		explicit operator OtherVectorClass() const;
		Vector2<T> ConvertToVector2XY() const;
		Vector4<T> ConvertToVector4XYZ0() const;
		//Returns a negated copy of the vector
		Vector3<T> operator-() const;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this vector
		Vector3<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;
		//Returns the cross product of this and aVector
		Vector3<T> Cross(const Vector3<T>& aVector) const;
	};

	template<class T>
	template<class OtherVectorType>
	inline Vector3<T>::operator OtherVectorType() const;

	//Returns the vector sum of aVector0 and aVector1
	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	//Returns the vector aVector0 component-multiplied by aVector1
	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z);
	}

	//Returns the vector aVector multiplied by the scalar aScalar. Vector * Scalar
	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	//Author Erik
	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(
			aVector0.x / aVector1.x,
			aVector0.y / aVector1.y,
			aVector0.z / aVector1.z
		);
	}

	//Returns the vector aVector multiplied by the scalar aScalar. Scalar * Vector
	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return aVector * aScalar;
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		double multiplier = 1.0 / static_cast<double>(aScalar);
		//aVector.x = static_cast<T>(static_cast<double>(aVector.x) * multiplier);
		//aVector.y = static_cast<T>(static_cast<double>(aVector.y) * multiplier);
		//aVector.z = static_cast<T>(static_cast<double>(aVector.z) * multiplier);
		return Vector3<T>(static_cast<T>(static_cast<double>(aVector.x) * multiplier), static_cast<T>(static_cast<double>(aVector.y) * multiplier), static_cast<T>(static_cast<double>(aVector.z) * multiplier));
		//return aVector / aScalar;
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
		//aVector0 = Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
		//aVector0 = Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
	}

	//Author Erik
	template <class T>
	Vector3<T>& operator*=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		aVector0.z *= aVector1.z;
		return aVector0;
	}
	template <class T>
	Vector3<T>& operator/=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		aVector0.z /= aVector1.z;
		return aVector0;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template<class T>
	inline Vector3<T>::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	template<class T>
	inline Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	//Author Erik
	template<class T>
	inline Vector3<T>::Vector3(const Vector2<T> aVector2, const T& aZ) :Vector3<T>(aVector2.x, aVector2.y, aZ)
	{
	}

	//Explicit Type operator, create a different vector with the same values.
	//Example creates a Tga::Vector3<T> from this CommonUtillities::Vector3<T>
	template<class T>
	template<class OtherVectorClass>
	inline Vector3<T>::operator OtherVectorClass() const
	{
		return OtherVectorClass(x, y, z);
	}

	//Creates a Tga::Vector2<T> from this CommonUtillities::Vector3<T>
	template<class T>
	inline Vector2<T> Vector3<T>::ConvertToVector2XY() const
	{
		return Vector2<T>(x, y);
	}

	//Creates a Tga::Vector4<T> from this CommonUtillities::Vector3<T>
	template<class T>
	inline Vector4<T> Vector3<T>::ConvertToVector4XYZ0() const
	{
		return Vector4<T>(x, y, z, 0);
	}

	//Returns a negated copy of the vector
	template<class T>
	inline Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	//Returns the squared length of the vector
	template<class T>
	inline T Vector3<T>::LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	//Returns the length of the vector
	template<class T>
	inline T Vector3<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	//Returns a normalized copy of this vector
	template<class T>
	inline Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length = Length();
		assert(length != 0);
		return Vector3<T>(x / length, y / length, z / length);
	}

	//Normalizes the vector
	template<class T>
	inline void Vector3<T>::Normalize()
	{
		T length = Length();
		assert(length != 0);
		x /= length;
		y /= length;
		z /= length;
	}

	//Returns the dot product of this and aVector
	template<class T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return T(x * aVector.x + y * aVector.y + z * aVector.z);
	}

	//Returns the cross product of this and aVector
	template<class T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		T otherX = aVector.x;
		T otherY = aVector.y;
		T otherZ = aVector.z;
		return Vector3<T>(y * otherZ - z * otherY,
			z * otherX - x * otherZ,
			x * otherY - y * otherX);
	}
}