#pragma once
#include <xmmintrin.h>
#include <cassert>
#include "Math.hpp"
namespace RealSix
{
	template <typename T>
	class Vector2;

	template <typename T>
	class Vector3;

	template <typename T>
	class Matrix2;

	template <typename T>
	class Matrix4;

	template <typename T>
	class Matrix3
	{
	public:
		union
		{
			struct
			{
				std::array<T, 9> elements;
			};
			struct
			{
				std::array<Vector3<T>, 3> columns;
			};
		};

		Matrix3();
		Matrix3(const T &value);
		Matrix3(const Vector3<T> &diagonal);
		Matrix3(const T &d00, const T &d11, const T &d22);
		Matrix3(const T &e00, const T &e10, const T &e20, const T &e01, const T &e11, const T &e21, const T &e02, const T &e12, const T &e22);

		void Set(const Vector3<T> &diagonal);
		void Set(const T &d00, const T &d11, const T &d22);
		void Set(const T &e00, const T &e10, const T &e20, const T &e01, const T &e11, const T &e21, const T &e02, const T &e12, const T &e22);

		template <typename T2>
		Matrix3<T> &operator+=(const Matrix3<T2> &right);
		template <typename T2>
		Matrix3<T> &operator-=(const Matrix3<T2> &right);
		template <typename T2>
		Matrix3<T> &operator*=(const Matrix3<T2> &value);
		template <typename T2>
		Matrix3<T> &operator*=(const T2 &value);
		template <typename T2>
		Vector3<T> &operator[](T2 idx);

		static Matrix3<T> Rotate(const Vector3<T> &axis, const T &radian);
		static Matrix3<T> Scale(const T &factor);
		static Matrix3<T> Scale(const Vector3<T> &factor);
		static Matrix3<T> Transpose(const Matrix3<T> &right);
		static Matrix3<T> Inverse(const Matrix3<T> &right);
		static T Determinant(const Matrix3<T> &right);
		static Matrix3<T> Adjoint(const Matrix3<T> &right);
		static Matrix4<T> ToMatrix4(const Matrix3<T> &matrix);

		static const Matrix3<T> IDENTITY;
	};

	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
	typedef Matrix3<int32_t> Matrix3i32;
	typedef Matrix3<uint32_t> Matrix3u32;

	template <typename T>
	const Matrix3<T> Matrix3<T>::IDENTITY = Matrix3();

	template <typename T>
	inline Matrix3<T>::Matrix3()
		: elements({static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f),
					static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f),
					static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f)})
	{
	}

	template <typename T>
	inline Matrix3<T>::Matrix3(const T &value)
		: elements({value, static_cast<T>(0.0f), static_cast<T>(0.0f),
					static_cast<T>(0.0f), value, static_cast<T>(0.0f),
					static_cast<T>(0.0f), static_cast<T>(0.0f), value})
	{
	}

	template <typename T>
	inline Matrix3<T>::Matrix3(const T &d00, const T &d11, const T &d22)
		: elements({d00, static_cast<T>(0.0f), static_cast<T>(0.0f),
					static_cast<T>(0.0f), d11, static_cast<T>(0.0f),
					static_cast<T>(0.0f), static_cast<T>(0.0f), d22})
	{
	}

	template <typename T>
	inline Matrix3<T>::Matrix3(const T &e00, const T &e10, const T &e20, const T &e01, const T &e11, const T &e21, const T &e02, const T &e12, const T &e22)
		: elements({e00, e10, e20, e01, e11, e21, e02, e12, e22})
	{
	}

	template <typename T>
	inline void Matrix3<T>::Set(const Vector3<T> &diagonal)
	{
		Set(diagonal.x, diagonal.y, diagonal.z);
	}

	template <typename T>
	inline void Matrix3<T>::Set(const T &d00, const T &d11, const T &d22)
	{
		elements[0] = d00;
		elements[4] = d11;
		elements[8] = d22;
	}

	template <typename T>
	inline void Matrix3<T>::Set(const T &e00, const T &e10, const T &e20, const T &e01, const T &e11, const T &e21, const T &e02, const T &e12, const T &e22)
	{
		elements[0] = e00;
		elements[1] = e10;
		elements[2] = e20;
		elements[3] = e01;
		elements[4] = e11;
		elements[5] = e21;
		elements[6] = e02;
		elements[7] = e12;
		elements[8] = e22;
	}

	template <typename T>
	inline Matrix3<T>::Matrix3(const Vector3<T> &diagonal)
		: elements({diagonal.x, static_cast<T>(0.0f), static_cast<T>(0.0f),
					static_cast<T>(0.0f), diagonal.y, static_cast<T>(0.0f),
					static_cast<T>(0.0f), static_cast<T>(0.0f), diagonal.z})
	{
	}

	template <typename T>
	template <typename T2>
	inline Matrix3<T> &Matrix3<T>::operator+=(const Matrix3<T2> &right)
	{

		for (uint8_t i = 0; i < 9; ++i)
			elements[i] += right.elements[i];
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix3<T> &Matrix3<T>::operator-=(const Matrix3<T2> &right)
	{

		for (uint8_t i = 0; i < 9; ++i)
			elements[i] -= right.elements[i];
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix3<T> &Matrix3<T>::operator*=(const Matrix3<T2> &right)
	{

		for (uint8_t i = 0; i < 9; ++i)
			elements[i] *= right.elements[i];
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix3<T> &Matrix3<T>::operator*=(const T2 &value)
	{
		for (uint8_t i = 0; i < 9; ++i)
			elements[i] *= value;
		return *this;
	}

	template <typename T>
	template <typename T2>
	Vector3<T> &Matrix3<T>::operator[](T2 idx)
	{
		assert(idx < 3);
		return columns[idx];
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Rotate(const Vector3<T> &axis, const T &radian)
	{
		Matrix3<T> tmp;
		float radian_cos = Math::Cos(radian);
		float radian_sin = Math::Sin(radian);
		float tmpNum = 1 - radian_cos;

		tmp.elements[0] = axis.x * axis.x * tmpNum + radian_cos;
		tmp.elements[1] = axis.y * axis.x * tmpNum + axis.z * radian_sin;
		tmp.elements[2] = axis.x * axis.z * tmpNum - axis.y * radian_sin;
		tmp.elements[3] = axis.y * axis.x * tmpNum - axis.z * radian_sin;
		tmp.elements[4] = axis.y * axis.y * tmpNum + radian_cos;
		tmp.elements[5] = axis.z * axis.y * tmpNum + axis.x * radian_sin;
		tmp.elements[6] = axis.x * axis.z * tmpNum + axis.y * radian_sin;
		tmp.elements[7] = axis.z * axis.y * tmpNum - axis.x * radian_sin;
		tmp.elements[8] = axis.z * axis.z * tmpNum + radian_cos;

		return tmp;
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Scale(const T &factor)
	{
		Matrix3<T> tmp;
		tmp.Set(Vector3<T>(factor));
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Scale(const Vector3<T> &factor)
	{
		Matrix3<T> tmp;
		tmp.Set(factor);
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Transpose(const Matrix3<T> &right)
	{
		Matrix3<T> tmp = right;
		tmp.elements[1] = right.elements[3];
		tmp.elements[2] = right.elements[6];
		tmp.elements[3] = right.elements[1];
		tmp.elements[5] = right.elements[7];
		tmp.elements[6] = right.elements[2];
		tmp.elements[7] = right.elements[5];
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Inverse(const Matrix3<T> &right)
	{
		return Matrix3<T>::Adjoint(right) / Matrix3<T>::Determinant(right);
	}

	template <typename T>
	inline T Matrix3<T>::Determinant(const Matrix3<T> &right)
	{
		return right.elements[0] * right.elements[4] * right.elements[8] +
			   right.elements[3] * right.elements[7] * right.elements[2] +
			   right.elements[6] * right.elements[1] * right.elements[5] -
			   right.elements[6] * right.elements[4] * right.elements[2] -
			   right.elements[5] * right.elements[7] * right.elements[0] -
			   right.elements[8] * right.elements[1] * right.elements[3];
	}

	template <typename T>
	inline Matrix3<T> Matrix3<T>::Adjoint(const Matrix3<T> &right)
	{
		Matrix3<T> tmp;
		tmp.elements[0] = Matrix2<T>::Determinant(Matrix2<T>(right.elements[4], right.elements[5], right.elements[7], right.elements[8]));
		tmp.elements[1] = -Matrix2<T>::Determinant(Matrix2<T>(right.elements[3], right.elements[5], right.elements[6], right.elements[8]));
		tmp.elements[2] = Matrix2<T>::Determinant(Matrix2<T>(right.elements[3], right.elements[4], right.elements[6], right.elements[7]));

		tmp.elements[3] = -Matrix2<T>::Determinant(Matrix2<T>(right.elements[1], right.elements[2], right.elements[7], right.elements[8]));
		tmp.elements[4] = Matrix2<T>::Determinant(Matrix2<T>(right.elements[0], right.elements[2], right.elements[6], right.elements[8]));
		tmp.elements[5] = -Matrix2<T>::Determinant(Matrix2<T>(right.elements[0], right.elements[1], right.elements[6], right.elements[7]));

		tmp.elements[6] = Matrix2<T>::Determinant(Matrix2<T>(right.elements[1], right.elements[2], right.elements[4], right.elements[5]));
		tmp.elements[7] = -Matrix2<T>::Determinant(Matrix2<T>(right.elements[0], right.elements[2], right.elements[3], right.elements[5]));
		tmp.elements[8] = Matrix2<T>::Determinant(Matrix2<T>(right.elements[0], right.elements[1], right.elements[3], right.elements[4]));
		return tmp;
	}

	template <typename T>
	inline Matrix4<T> Matrix3<T>::ToMatrix4(const Matrix3<T> &matrix)
	{
		Matrix4<T> tmp;
		tmp.col[0] = _mm_set_ps(static_cast<T>(0.0f), matrix.elements[2], matrix.elements[1], matrix.elements[0]);
		tmp.col[1] = _mm_set_ps(static_cast<T>(0.0f), matrix.elements[5], matrix.elements[4], matrix.elements[3]);
		tmp.col[2] = _mm_set_ps(static_cast<T>(0.0f), matrix.elements[8], matrix.elements[7], matrix.elements[6]);
		tmp.col[3] = _mm_set_ps(static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(0.0f));
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> operator+(const Matrix3<T> &left, const Matrix3<T> &right)
	{
		Matrix3<T> tmp;
		for (uint8_t i = 0; i < 9; ++i)
			tmp.elements[i] = left.elements[i] + right.elements[i];
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> operator-(const Matrix3<T> &left, const Matrix3<T> &right)
	{
		Matrix3<T> tmp;
		for (uint8_t i = 0; i < 9; ++i)
			tmp.elements[i] = left.elements[i] - right.elements[i];
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> operator*(const Matrix3<T> &left, const Matrix3<T> &right)
	{
		Matrix3<T> tmp;
		for (uint32_t i = 0; i < 9; ++i)
			tmp.elements[i] = left.elements[i] + right.elements[i];
		return tmp;
	}

	template <typename T>
	inline Matrix3<T> operator/(const Matrix3<T> &left, const T &value)
	{
		if (!Math::IsNearZero(value))
		{
			Matrix3<T> tmp;
			for (uint8_t i = 0; i < 9; ++i)
				tmp.elements[i] /= value;
			return tmp;
		}
		return left;
	}

	template <typename T, typename T2>
	inline Matrix3<T> operator*(const Matrix3<T> &left, const T2 &value)
	{
		Matrix3<T> tmp;
		for (uint8_t i = 0; i < 9; ++i)
			tmp.elements[i] *= value;
		return tmp;
	}

	template <typename T, typename T2>
	inline Matrix3<T> operator*(const T &value, const Matrix3<T2> &right)
	{
		return right * value;
	}

	template <typename T, typename T2>
	inline bool operator==(const Matrix3<T> &left, const Matrix3<T2> &right)
	{
		for (uint8_t i = 0; i < 9; ++i)
			if (left.elements[i] != right.elements[i])
				return false;
		return true;
	}
}