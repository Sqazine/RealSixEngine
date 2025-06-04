#pragma once
#include <xmmintrin.h>
#include <array>
namespace RealSix
{

	template <typename T>
	class Vector2;
	template <typename T>
	class Matrix3;
	template <typename T>
	class Matrix2
	{
	public:
		union
		{
			struct
			{
				__m128 col;
			};
			struct
			{
				std::array<T, 4> elements;
			};
			struct
			{
				std::array<Vector2<T>, 2> columns;
			};
		};

		Matrix2();
		Matrix2(const T &value);
		Matrix2(const T &e00, const T &e01, const T &e10, const T &e11);
		Matrix2(const T &d00, const T &d11);
		Matrix2(const Vector2<T> &diagonal);
		~Matrix2();

		void Set(const T &e00, const T &e01, const T &e10, const T &e11);
		void Set(const T &d00, const T &d11);
		void Set(const Vector2<T> &diagonal);

		template <typename T2>
		Matrix2<T> &operator+=(const Matrix2<T2> &right);
		template <typename T2>
		Matrix2<T> &operator-=(const Matrix2<T2> &right);
		template <typename T2>
		Matrix2<T> &operator*=(const Matrix2<T2> &right);
		template <typename T2>
		Matrix2<T> &operator*=(const T2 &value);
		template <typename T2>
		Vector2<T> &operator[](T2 idx);

		template <typename T2>
		static Matrix2<T> Rotate(const T2 &radian);
		template <typename T2>
		static Matrix2<T> Scale(const T2 &factor);
		template <typename T2>
		static Matrix2<T> Scale(const Vector2<T2> &factor);
		template <typename T2>
		static T Determinant(const Matrix2<T2> &matrix);
		template <typename T2>
		static Matrix2<T> Inverse(const Matrix2<T2> &matrix);
		template <typename T2>
		static Matrix2<T> Adjoint(const Matrix2<T2> &matrix);
		template <typename T2>
		static Matrix3<T> ToMatrix3(const Matrix2<T2> &matrix);

		static const Matrix2<T> IDENTITY;
	};

	typedef Matrix2<float> Matrix2f;
	typedef Matrix2<double> Matrix2d;
	typedef Matrix2<int32_t> Matrix2i32;
	typedef Matrix2<uint32_t> Matrix2u32;

	template <typename T>
	const Matrix2<T> Matrix2<T>::IDENTITY = Matrix2<T>();

	template <typename T>
	inline Matrix2<T>::Matrix2()
		: col(_mm_set_ps(static_cast<T>(1.0f), static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(1.0f)))
	{
	}

	template <typename T>
	inline Matrix2<T>::Matrix2(const T &value)
		: col(_mm_set_ps(value, static_cast<T>(0.0f), static_cast<T>(0.0f), value))
	{
	}

	template <typename T>
	inline Matrix2<T>::Matrix2(const T &e00, const T &e01, const T &e10, const T &e11)
		: col(_mm_set_ps(e11, e10, e01, e00))
	{
	}

	template <typename T>
	inline Matrix2<T>::Matrix2(const T &d00, const T &d11)
	{
		Set(d00, d11);
	}

	template <typename T>
	inline Matrix2<T>::Matrix2(const Vector2<T> &diagonal)
	{
		Set(diagonal);
	}

	template <typename T>
	inline Matrix2<T>::~Matrix2()
	{
	}

	template <typename T>
	inline void Matrix2<T>::Set(const T &e00, const T &e01, const T &e10, const T &e11)
	{
		col = _mm_set_ps(e11, e10, e01, e00);
	}

	template <typename T>
	inline void Matrix2<T>::Set(const T &d00, const T &d11)
	{
		elements[0] = d00;
		elements[1] = d11;
	}

	template <typename T>
	inline void Matrix2<T>::Set(const Vector2<T> &diagonal)
	{
		Set(diagonal.x, diagonal.y);
	}

	template <typename T, typename T2>
	inline Matrix2<T> operator+(const Matrix2<T> &left, const Matrix2<T2> &right)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_add_ps(left.col, right.col);
		return tmp;
	}
	template <typename T, typename T2>
	inline Matrix2<T> operator-(const Matrix2<T> &left, const Matrix2<T2> &right)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_sub_ps(left.col, right.col);
		return tmp;
	}

	template <typename T, typename T2>
	inline Matrix2<T> operator*(const Matrix2<T> &left, const Matrix2<T2> &right)
	{
		Matrix2<T> tmp;
		tmp.elements[0] = left.elements[0] * right.elements[0] + left.elements[2] * right.elements[1];
		tmp.elements[2] = left.elements[0] * right.elements[2] + left.elements[2] * right.elements[3];
		tmp.elements[1] = left.elements[1] * right.elements[0] + left.elements[3] * right.elements[1];
		tmp.elements[3] = left.elements[1] * right.elements[2] + left.elements[3] * right.elements[3];
		return tmp;
	}

	template <typename T, typename T2>
	inline Matrix2<T> operator/(const Matrix2<T> &left, const T2 &value)
	{
		if (!Math::IsNearZero(value))
		{
			Matrix2<T> tmp;
			tmp.col = _mm_sub_ps(left.col, _mm_set_ps1(value));
			return tmp;
		}
		return left;
	}

	template <typename T, typename T2>
	inline Matrix2<T> operator*(const T &value, const Matrix2<T2> &right)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_mul_ps(right.col, _mm_set_ps1(value));
		return tmp;
	}

	template <typename T, typename T2>
	inline Matrix2<T> operator*(const Matrix2<T> &left, const T2 &value)
	{
		return value * left;
	}

	template <typename T>
	inline bool operator==(const Matrix2<T> &left, const Matrix2<T> &right)
	{
		for (uint8_t i = 0; i < 4; ++i)
			if (left.elements[i] != right.elements[i])
				return false;
		return true;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> &Matrix2<T>::operator+=(const Matrix2<T2> &right)
	{

		*this = *this + right;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> &Matrix2<T>::operator-=(const Matrix2<T2> &right)
	{

		*this = *this - right;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> &Matrix2<T>::operator*=(const Matrix2<T2> &right)
	{

		*this = *this * right;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> &Matrix2<T>::operator*=(const T2 &value)
	{

		*this = *this * value;
		return *this;
	}

	template <typename T>
	template <typename T2>
	Vector2<T> &Matrix2<T>::operator[](T2 idx)
	{
		assert(idx < 2);
		return columns[idx];
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> Matrix2<T>::Rotate(const T2 &radian)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_set_ps(Math::Cos(radian), -Math::Sin(radian), Math::Sin(radian), Math::Cos(radian));
		return tmp;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> Matrix2<T>::Scale(const T2 &factor)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_set_ps(factor, static_cast<T>(0.0f), static_cast<T>(0.0f), factor);
		return tmp;
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> Matrix2<T>::Scale(const Vector2<T2> &factor)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_set_ps(factor.y, static_cast<T>(0.0f), static_cast<T>(0.0f), factor.x);
	}

	template <typename T>
	template <typename T2>
	inline T Matrix2<T>::Determinant(const Matrix2<T2> &matrix)
	{
		return matrix.elements[0] * matrix.elements[3] - matrix.elements[2] * matrix.elements[1];
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> Matrix2<T>::Inverse(const Matrix2<T2> &matrix)
	{
		return Matrix2<T>::Adjoint(matrix) / Matrix2<T>::Determinant(matrix);
	}

	template <typename T>
	template <typename T2>
	inline Matrix2<T> Matrix2<T>::Adjoint(const Matrix2<T2> &matrix)
	{
		Matrix2<T> tmp;
		tmp.col = _mm_set_ps(matrix.elements[0], -matrix.elements[1], -matrix.elements[2], matrix.elements[3]);
		return tmp;
	}

	template <typename T>
	template <typename T2>
	inline Matrix3<T> Matrix2<T>::ToMatrix3(const Matrix2<T2> &matrix)
	{
		Matrix3<T> tmp;
		tmp.elements[0] = matrix.elements[0];
		tmp.elements[1] = matrix.elements[1];
		tmp.elements[3] = matrix.elements[2];
		tmp.elements[4] = matrix.elements[3];
		return tmp;
	}
}