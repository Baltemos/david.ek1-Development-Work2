#pragma once
#include <cmath>
#include <assert.h>
namespace CommonUtilities
{
	template <class T>
	class Vector3;
	template <class T>
	class Vector4;

	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;
		//Creates a null-vector
		Vector2<T>();
		//Creates a vector (aX, aY)
		Vector2<T>(const T& aX, const T& aY);
		//Copy constructor (compiler generated)
		Vector2<T>(const Vector2<T>& aVector) = default;
		//Assignment operator (compiler generated)
		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;
		//Destructor (compiler generated)
		~Vector2<T>() = default;
		//Explicit Type operator, create a different vector with the same values.
		//Example creates a Tga::Vector2<T> from this CommonUtillities::Vector2<T>
		template <class OtherVectorClass>
		explicit operator OtherVectorClass() const;
		Vector3<T> ConvertToVector3XY0() const;
		Vector4<T> ConvertToVector4XY00() const;
		//Returns a negated copy of the vector
		Vector2<T> operator-() const;
		//Returns the squared length of the vector
		T LengthSqr() const;
		//Returns the length of the vector
		T Length() const;
		//Returns a normalized copy of this vector
		Vector2<T> GetNormalized() const;
		//Normalizes the vector
		void Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;
		//Returns the cross product of this and aVector
	};

	template<class T>
	template<class OtherVectorType>
	inline Vector2<T>::operator OtherVectorType() const;

	template<class T>
	bool operator==(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return aVector0.x == aVector1.x && aVector0.y == aVector1.y;
	}

	template<class T>
	bool operator!=(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return !(aVector0.x == aVector1.x && aVector0.y == aVector1.y);
	}

	template<class T>
	bool operator<(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return (aVector0.x < aVector1.x && aVector0.y < aVector1.y);
	}

	template<class T>
	bool operator>(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return (aVector0.x > aVector1.x && aVector0.y > aVector1.y);
	}

	//Returns the vector sum of aVector0 and aVector1
	template <class T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	//Returns the vector difference of aVector0 and aVector1
	template <class T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	//Returns the vector aVector0 component-multiplied by aVector1
	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y);
	}

	//Author Erik
	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(
			aVector0.x / aVector1.x,
			aVector0.y / aVector1.y
		);
	}

	//Returns the vector aVector multiplied by the scalar aScalar. Vector * Scalar
	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	//Returns the vector aVector multiplied by the scalar aScalar. Scalar * Vector
	template <class T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return aVector * aScalar;
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)
	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		double multiplier = 1.0 / static_cast<double>(aScalar);
		//aVector.x = static_cast<T>(static_cast<double>(aVector.x) * multiplier);
		//aVector.y = static_cast<T>(static_cast<double>(aVector.y) * multiplier);
		//aVector.z = static_cast<T>(static_cast<double>(aVector.z) * multiplier);
		return Vector2<T>(static_cast<T>(static_cast<double>(aVector.x) * multiplier), static_cast<T>(static_cast<double>(aVector.y) * multiplier));
		//return aVector / aScalar;
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <class T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 + aVector1;
		//aVector0 = Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <class T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0 = aVector0 - aVector1;
		//aVector0 = Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	//Author Erik
	template <class T>
	Vector2<T>& operator*=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x *= aVector1.x;
		aVector0.y *= aVector1.y;
		return aVector0;
	}
	template <class T>
	Vector2<T>& operator/=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x /= aVector1.x;
		aVector0.y /= aVector1.y;
		return aVector0;
	}

	//Equivalent to setting aVector to (aVector * aScalar)
	template <class T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector * aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)
	template <class T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector = aVector / aScalar;
	}

	template<class T>
	inline Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}

	template<class T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	//Explicit Type operator, create a different vector with the same values.
	//Example creates a Tga::Vector2<T> from this CommonUtillities::Vector2<T>
	template<class T>
	template<class OtherVectorClass>
	inline Vector2<T>::operator OtherVectorClass() const
	{
		return OtherVectorClass(x, y);
	}

	//Creates a Tga::Vector3<T> from this CommonUtillities::Vector2<T>
	template<class T>
	inline Vector3<T> Vector2<T>::ConvertToVector3XY0() const
	{
		return Vector3<T>(x, y, 0);
	}

	//Creates a Tga::Vector4<T> from this CommonUtillities::Vector2<T>
	template<class T>
	inline Vector4<T> Vector2<T>::ConvertToVector4XY00() const
	{
		return Vector4<T>(x, y, 0, 0);
	}


	//Returns a negated copy of the vector
	template<class T>
	inline Vector2<T> Vector2<T>::operator-() const
	{
		return Vector2<T>(-x, -y);
	}

	//Returns the squared length of the vector
	template<class T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	//Returns the length of the vector
	template<class T>
	inline T Vector2<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	//Returns a normalized copy of this vector
	template<class T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();
		assert(length != 0);
		return Vector2<T>(x / length, y / length);
	}

	//Normalizes the vector
	template<class T>
	inline void Vector2<T>::Normalize()
	{
		T length = Length();
		assert(length != 0);
		x /= length;
		y /= length;
	}

	//Returns the dot product of this and aVector
	template<class T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return T(x * aVector.x + y * aVector.y);
	}
}