#pragma once
#include <array>
#include <cassert>
#include "Math.hpp"

namespace RealSix
{
	template <typename T>
	class Vector3;
	template <typename T>
	class Matrix4;

	template <typename T>
	class Vector4
	{
	public:
		union
		{
			struct
			{
				T x, y, z, w;
			};
			struct
			{
				std::array<T, 4> values;
			};
			struct
			{
				 T valuesRawArray[4];
			};
		};

		Vector4();
		Vector4(const T &value);
		Vector4(const T &x, const T &y, const T &z, const T &w = static_cast<T>(1.0f));
		Vector4(const Vector3<T> &vec, const T &w = 1.0f);

		template <typename T2>
		Vector4<T> &operator+=(const T2 &value);
		template <typename T2>
		Vector4<T> &operator+=(const Vector4<T2> &right);
		template <typename T2>
		Vector4<T> &operator-=(const T2 &value);
		template <typename T2>
		Vector4<T> &operator-=(const Vector4<T2> &right);
		template <typename T2>
		Vector4<T> &operator*=(const T2 &value);
		template <typename T2>
		Vector4<T> &operator*=(const Vector4<T2> &right);
		template <typename T2>
		Vector4<T> &operator/=(const T2 &value);
		template <typename T2>
		Vector4<T> &operator/=(const Vector4<T2> &right);
		template <typename T2>
		Vector4<T> &operator=(const Vector4<T2> &right);
		template <typename T2>
		T &operator[](T2 idx);

		static Vector4<T> DivideByW(const Vector4<T> &vec);
		static Vector3<T> ToVector3(const Vector4<T> &vec);

		static Vector4<T> Abs(const Vector4<T> &v);

		static const Vector4<T> ZERO;
		static const Vector4<T> ONE;
		static const Vector4<T> UNIT_X;
		static const Vector4<T> UNIT_Y;
		static const Vector4<T> UNIT_Z;
		static const Vector4<T> UNIT_W;
	};

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<int64_t> Vector4i64;
	typedef Vector4<uint64_t> Vector4u64;
	typedef Vector4<int32_t> Vector4i32;
	typedef Vector4<uint32_t> Vector4u32;
	typedef Vector4<int16_t> Vector4i16;
	typedef Vector4<uint16_t> Vector4u16;
	typedef Vector4<int8_t> Vector4i8;
	typedef Vector4<uint8_t> Vector4u8;

	template <typename T>
	const Vector4<T> Vector4<T>::ZERO = Vector4<T>();

	template <typename T>
	const Vector4<T> Vector4<T>::ONE = Vector4<T>(static_cast<T>(1.0f));

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_X = Vector4<T>(static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f));

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_Y = Vector4<T>(static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f));

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_Z = Vector4<T>(static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f));

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_W = Vector4<T>(static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f));

	template <typename T>
	inline Vector4<T>::Vector4()
		: values({static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f)})
	{
	}

	template <typename T>
	inline Vector4<T>::Vector4(const T &value)
		: values({value, value, value, value})
	{
	}

	template <typename T>
	inline Vector4<T>::Vector4(const T &x, const T &y, const T &z, const T &w)
		: values({x, y, z, w})
	{
	}

	template <typename T>
	inline Vector4<T>::Vector4(const Vector3<T> &vec, const T &w)
		: values({vec.x, vec.y, vec.z, w})
	{
	}

	template <typename T, typename T2>
	inline Vector4<T> operator+(const Vector4<T> &left, const Vector4<T2> &right)
	{
		return Vector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator+(const Vector4<T> &left, const T2 &value)
	{
		return Vector4<T>(left.x + value, left.y + value, left.z + value, left.w + value);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator+(const T &value, const Vector4<T2> &right)
	{
		return right + value;
	}

	template <typename T, typename T2>
	inline Vector4<T> operator-(const Vector4<T> &left, const Vector4<T2> &right)
	{
		return Vector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator-(const Vector4<T> &left, const T2 &value)
	{
		return Vector4<T>(left.x - value, left.y - value, left.z - value, left.w - value);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator-(const T &value, const Vector4<T2> &right)
	{
		return Vector4<T>(value - right.x, value - right.y, value - right.z, value - right.w);
	}

	template <typename T>
	inline Vector4<T> operator-(const Vector4<T> &right)
	{
		return Vector4<T>(-right.x, -right.y, -right.z, -right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator*(const Vector4<T> &left, const Vector4<T2> &right)
	{
		return Vector4<T>(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator*(const T2 &value, const Vector4<T> &right)
	{
		return Vector4<T>(value * right.x, value * right.y, value * right.z, value * right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator*(const Vector4<T> &left, T2 value)
	{
		return value * left;
	}

	template <typename T, typename T2>
	inline Vector4<T> operator/(const Vector4<T> &left, T2 value)
	{
		if (!Math::IsNearZero(value))
			return Vector4<T>(left.x / value, left.y / value, left.z / value, left.w / value);
		return left;
	}

	template <typename T, typename T2>
	inline Vector4<T> operator/(const Vector4<T> &left, const Vector4<T2> &value)
	{

		return Vector4<T>(left.x / value.x, left.y / value.y, left.z / value.z, left.w / value.w);
	}

	template <typename T, typename T2>
	inline bool operator==(const Vector4<T> &left, const Vector4<T2> &right)
	{
		return Math::IsNearZero(left.x - right.x) && Math::IsNearZero(left.y - right.y) && Math::IsNearZero(left.z - right.z) && Math::IsNearZero(left.w - right.w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator*(const Matrix4<T> &matrix, const Vector4<T2> &vec)
	{
		T x = matrix.elements[0] * vec.x + matrix.elements[4] * vec.y + matrix.elements[8] * vec.z + matrix.elements[12] * vec.w;
		T y = matrix.elements[1] * vec.x + matrix.elements[5] * vec.y + matrix.elements[9] * vec.z + matrix.elements[13] * vec.w;
		T z = matrix.elements[2] * vec.x + matrix.elements[6] * vec.y + matrix.elements[10] * vec.z + matrix.elements[14] * vec.w;
		T w = matrix.elements[3] * vec.x + matrix.elements[7] * vec.y + matrix.elements[11] * vec.z + matrix.elements[15] * vec.w;

		return Vector4<T>(x, y, z, w);
	}

	template <typename T, typename T2>
	inline Vector4<T> operator*(const Vector4<T> &vec, const Matrix4<T2> &matrix)
	{
		T x = vec.x * matrix.elements[0] + vec.y * matrix.elements[1] + vec.z * matrix.elements[2] + vec.w * matrix.elements[3];
		T y = vec.x * matrix.elements[4] + vec.y * matrix.elements[5] + vec.z * matrix.elements[6] + vec.w * matrix.elements[7];
		T z = vec.x * matrix.elements[8] + vec.y * matrix.elements[9] + vec.z * matrix.elements[10] + vec.w * matrix.elements[11];
		T w = vec.x * matrix.elements[12] + vec.y * matrix.elements[13] + vec.z * matrix.elements[14] + vec.w * matrix.elements[15];
		return Vector4<T>(x, y, z, w);
	}

	template <typename T>
	inline bool operator>(const Vector4<T> &left, const Vector4<T> &right)
	{
		return left.x > right.x && left.y > right.y && left.z > right.z && left.w > right.w;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator+=(const T2 &value)
	{

		x += value;
		y += value;
		z += value;
		w += value;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator+=(const Vector4<T2> &right)
	{

		x += right.x;
		y += right.y;
		z += right.z;
		w += right.w;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator-=(const T2 &value)
	{

		x -= value;
		y -= value;
		z -= value;
		w -= value;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator-=(const Vector4<T2> &right)
	{

		x -= right.x;
		y -= right.y;
		z -= right.z;
		w -= right.w;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator/=(const T2 &value)
	{

		if (!Math::IsNearZero(value))
		{
			x /= value;
			y /= value;
			z /= value;
			w /= value;
		}
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Vector4<T> &Vector4<T>::operator=(const Vector4<T2> &right)
	{
		x = right.x;
		y = right.y;
		z = right.z;
		w = right.w;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline T &Vector4<T>::operator[](T2 idx)
	{
		assert(idx < 4);
		return values[idx];
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::DivideByW(const Vector4<T> &vec)
	{
		return Vector4<T>(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w, static_cast<T>(1.0f));
	}

	template <typename T>
	inline Vector3<T> Vector4<T>::ToVector3(const Vector4<T> &vec)
	{
		return Vector3<T>(vec.x, vec.y, vec.z);
	}

	template <typename T>
	inline Vector4<T> Vector4<T>::Abs(const Vector4<T> &v)
	{
		Vector4<T> result;
		result.x = Math::Abs(v.x);
		result.y = Math::Abs(v.y);
		result.z = Math::Abs(v.z);
		result.w = Math::Abs(v.w);
		return result;
	}
}