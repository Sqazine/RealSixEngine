#pragma once
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include <cmath>
#include <array>
namespace RealSix
{
	template <typename T>
	class Vector3;

	template <typename T>
	class Matrix3;

	template <typename T>
	class Matrix4;

	template <typename T>
	class Quaternion
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
				Vector3<T> vec;
				T scalar;
			};
			struct
			{
				std::array<T, 4> values;
			};
		};

		Quaternion();
		Quaternion(T x, T y, T z, T w);
		Quaternion(const Vector3<T> &axis, T radian);
		Quaternion(const Vector3<T> &angles);

		Quaternion<T> &operator+=(const Quaternion<T> &right);
		Quaternion<T> &operator-=(const Quaternion<T> &right);
		Quaternion<T> &operator=(const Quaternion<T> &right);
		Quaternion<T> &operator/=(const T &value);

		T SquareLength() const;
		T Length() const;

		static T Dot(const Quaternion<T> &a, const Quaternion<T> &b);
		static Quaternion<T> Inverse(const Quaternion<T> &q);
		static Quaternion<T> Conjugate(const Quaternion<T> &q);
		static Quaternion<T> Normalize(const Quaternion<T> &q);

		static Quaternion<T> FromTo(const Vector3<T> &from, const Vector3<T> &to);

		//先旋转b再旋转a
		static Quaternion<T> Concatenate(const Quaternion<T> &a, const Quaternion<T> &b);
		template <typename T2>
		static Quaternion<T> Lerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 factor);
		template <typename T2>
		static Quaternion<T> Slerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 factor);
		template <typename T2>
		static Quaternion<T> NLerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 factor);
		//从四元数变换到齐次矩阵
		static Matrix4<T> ToMatrix4(const Quaternion<T> &q);
		//从四元数变换到旋转矩阵
		static Matrix3<T> ToMatrix3(const Quaternion<T> &q);
		static const Quaternion<T> ZERO;
	};

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;
	typedef Quaternion<int32_t> Quaternioni32;
	typedef Quaternion<uint32_t> Quaternionu32;
	typedef Quaternion<int16_t> Quaternioni16;
	typedef Quaternion<uint16_t> Quaternionu16;
	typedef Quaternion<int8_t> Quaternioni8;
	typedef Quaternion<uint8_t> Quaternionu8;

	template <typename T>
	const Quaternion<T> Quaternion<T>::ZERO = Quaternion<T>();

	template <typename T>
	Quaternion<T>::Quaternion()
		: values({static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)})
	{
	}

	template <typename T>
	inline Quaternion<T>::Quaternion(T x, T y, T z, T w)
		: values({x, y, z, w})
	{
	}

	template <typename T>
	inline Quaternion<T>::Quaternion(const Vector3<T> &axis, T radian)
	{
		Vector3 axis_nor = Vector3<T>::Normalize(axis);
		float sca = Math::Sin(radian / 2);
		vec = axis_nor * sca;
		scalar = Math::Cos(radian / 2);
	}

	template <typename T>
	inline Quaternion<T>::Quaternion(const Vector3<T> &angles)
	{
		auto quatX = Quaternion<T>(Vector3<T>::UNIT_X, Math::ToRadian(angles.x));
		auto quatY = Quaternion<T>(Vector3<T>::UNIT_Y, Math::ToRadian(angles.y));
		auto quatZ = Quaternion<T>(Vector3<T>::UNIT_Z, Math::ToRadian(angles.z));

		*this = Quaternion<T>::Concatenate(quatZ, Quaternion<T>::Concatenate(quatY, quatX));
	}

	template <typename T>
	inline Quaternion<T> operator+(const Quaternion<T> &left, const Quaternion<T> &right)
	{
		Quaternion<T> tmpQ;
		tmpQ.x = left.x + right.x;
		tmpQ.y = left.y + right.y;
		tmpQ.z = left.z + right.z;
		tmpQ.w = left.w + right.w;
		return tmpQ;
	}

	template <typename T>
	inline Quaternion<T> operator-(const Quaternion<T> &left, const Quaternion<T> &right)
	{
		Quaternion<T> tmpQ;
		tmpQ.x = left.x - right.x;
		tmpQ.y = left.y - right.y;
		tmpQ.z = left.z - right.z;
		tmpQ.w = left.w - right.w;
		return tmpQ;
	}

	template <typename T>
	inline Quaternion<T> operator*(const Quaternion<T> &left, const Quaternion<T> &right)
	{
		return Quaternion<T>(
			right.x * left.w + right.y * left.z - right.z * left.y + right.w * left.x,
			-right.x * left.z + right.y * left.w + right.z * left.x + right.w * left.y,
			right.x * left.y - right.y * left.x + right.z * left.w + right.w * left.z,
			-right.x * left.x - right.y * left.y - right.z * left.z + right.w * left.w);
	}

	template <typename T>
	inline Vector3<T> operator*(const Quaternion<T> &left, const Vector3<T> &right)
	{
		return left.vec * 2.0f * Vector3<T>::Dot(left.vec, right) + right * (left.scalar * left.scalar - Vector3<T>::Dot(left.vec, left.vec)) + Vector3<T>::Cross(left.vec, right) * 2.0f * left.scalar;
	}

	template <typename T>
	inline Quaternion<T> operator*(const Quaternion<T> &left, T value)
	{
		return Quaternion<T>(left.x * value, left.y * value, left.z * value, left.w * value);
	}

	template <typename T>
	inline Quaternion<T> operator*(T value, const Quaternion<T> &right)
	{
		return right * value;
	}

	template <typename T>
	inline Quaternion<T> operator-(const Quaternion<T> &right)
	{
		Quaternion<T> tmp = right;
		tmp.vec *= -1.0f;
		tmp.scalar *= -1.0f;
		return tmp;
	}

	template <typename T>
	inline Quaternion<T> operator/(const Quaternion<T> &right, const T &value)
	{
		Quaternion<T> tmpQ;
		tmpQ.x = right.x / value;
		tmpQ.y = right.y / value;
		tmpQ.z = right.z / value;
		tmpQ.w = right.w / value;

		return tmpQ;
	}

	template <typename T>
	inline bool operator==(const Quaternion<T> &left, const Quaternion<T> &right)
	{
		return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
	}

	template <typename T>
	inline bool operator!=(const Quaternion<T> &left, const Quaternion<T> &right)
	{
		return !(left == right);
	}

	template <typename T>
	inline Quaternion<T> &Quaternion<T>::operator+=(const Quaternion<T> &right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		w += right.w;
		return *this;
	}

	template <typename T>
	inline Quaternion<T> &Quaternion<T>::operator-=(const Quaternion<T> &right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		w -= right.w;
		return *this;
	}

	template <typename T>
	inline Quaternion<T> &Quaternion<T>::operator=(const Quaternion<T> &right)
	{
		this->x = right.x;
		this->y = right.y;
		this->z = right.z;
		this->w = right.w;
		return *this;
	}

	template <typename T>
	inline Quaternion<T> &Quaternion<T>::operator/=(const T &value)
	{
		this.x /= value;
		this.y /= value;
		this.z /= value;
		this.w /= value;
		return *this;
	}

	template <typename T>
	inline T Quaternion<T>::SquareLength() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <typename T>
	inline T Quaternion<T>::Length() const
	{
		return Math::Sqrt(SquareLength());
	}

	template <typename T>
	inline T Quaternion<T>::Dot(const Quaternion<T> &a, const Quaternion<T> &b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::Inverse(const Quaternion<T> &q)
	{
		Quaternion<T> tmpQ = q;
		if (tmpQ.SquareLength() == 1.0f)
			return Conjugate(q);
		else if (tmpQ.SquareLength() <= std::numeric_limits<T>::epsilon()) //接近0
			return Quaternion<T>();
		else
			return Conjugate(tmpQ / tmpQ.SquareLength());
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::Conjugate(const Quaternion<T> &q)
	{
		return Quaternion<T>(-q.x, -q.y, -q.z, q.w);
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::Normalize(const Quaternion<T> &q)
	{
		Quaternion<T> tmpQ = q;
		float len = tmpQ.Length();
		if (len > 0)
		{
			tmpQ.x /= len;
			tmpQ.y /= len;
			tmpQ.z /= len;
			tmpQ.w /= len;
		}
		return tmpQ;
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::FromTo(const Vector3<T> &from, const Vector3<T> &to)
	{
		Vector3<T> f = Vector3<T>::Normalize(from);
		Vector3<T> t = Vector3<T>::Normalize(to);

		if (f == t)
		{
			return Quaternion<T>();
		}
		else if (f == t * -1.0f)
		{
			Vector3<T> ortho = Vector3<T>(1, 0, 0);
			if (fabsf(f.y) < fabsf(f.x))
			{
				ortho = Vector3<T>::UNIT_Y;
			}
			if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x))
			{
				ortho = Vector3<T>::UNIT_Z;
			}

			Vector3<T> axis = Vector3<T>::Normalize(Vector3<T>::Cross(f, ortho));
			return Quaternion<T>(axis.x, axis.y, axis.z, 0);
		}

		Vector3<T> half = Vector3<T>::Normalize(f + t);
		Vector3<T> axis = Vector3<T>::Cross(f, half);

		return Quaternion<T>(axis.x, axis.y, axis.z, Vector3<T>::Dot(f, half));
	}

	template <typename T>
	inline Quaternion<T> Quaternion<T>::Concatenate(const Quaternion<T> &a, const Quaternion<T> &b)
	{
		Quaternion<T> tmpQ;
		Vector3 av(a.x, a.y, a.z);
		Vector3 bv(b.x, b.y, b.z);
		Vector3 newVec = a.w * bv + b.w * av + Vector3<T>::Cross(av, bv);
		tmpQ.x = newVec.x;
		tmpQ.y = newVec.y;
		tmpQ.z = newVec.z;
		tmpQ.w = b.w * a.w - Vector3<T>::Dot(bv, av);
		return tmpQ;
	}

	template <typename T>
	template <typename T2>
	inline Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 factor)
	{
		Quaternion<T> tmpQ;
		tmpQ.x = Math::Lerp(a.x, b.x, factor);
		tmpQ.y = Math::Lerp(a.y, b.y, factor);
		tmpQ.z = Math::Lerp(a.z, b.z, factor);
		tmpQ.w = Math::Lerp(a.w, b.w, factor);
		return tmpQ;
	}

	template <typename T>
	template <typename T2>
	inline Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 factor)
	{
		float rawCosm = Quaternion<T>::Dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Math::ArcCos(cosom);
			const float invSin = 1.0f / Math::Sin(omega);
			scale0 = Math::Sin((1.0f - factor) * omega) * invSin;
			scale1 = Math::Sin(factor * omega) * invSin;
		}
		else
		{
			// Use linear interpolation if the quaternions
			// are collinear
			scale0 = 1.0f - factor;
			scale1 = factor;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion<T> retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		return Quaternion<T>::Normalize(retVal);
	}

	template <typename T>
	template <typename T2>
	inline Quaternion<T> Quaternion<T>::NLerp(const Quaternion<T> &a, const Quaternion<T> &b, T2 t)
	{
		return Quaternion<T>::Normalize((1 - t) * a + t * b);
	}

	template <typename T>
	inline Matrix4<T> Quaternion<T>::ToMatrix4(const Quaternion<T> &q)
	{
		Matrix4<T> tmpMat;
		tmpMat.elements[0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
		tmpMat.elements[4] = 2 * q.x * q.y - 2 * q.w * q.z;
		tmpMat.elements[8] = 2 * q.x * q.z + 2 * q.w * q.y;

		tmpMat.elements[1] = 2 * q.x * q.y + 2 * q.w * q.z;
		tmpMat.elements[5] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
		tmpMat.elements[9] = 2 * q.y * q.z - 2 * q.w * q.x;

		tmpMat.elements[2] = 2 * q.x * q.z - 2 * q.w * q.y;
		tmpMat.elements[6] = 2 * q.y * q.z + 2 * q.w * q.x;
		tmpMat.elements[10] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		return tmpMat;

		// Vector3<T> r = q * Vector3<T>::UNIT_X;
		// Vector3<T> u = q * Vector3<T>::UNIT_Y;
		// Vector3<T> f = q * Vector3<T>::UNIT_Z;

		// return Matrix4<T>(
		// 	r.x, r.y, r.z, 0,
		// 	u.x, u.y, u.z, 0,
		// 	f.x, f.y, f.z, 0,
		// 	0, 0, 0, 1
		// );
	}

	template <typename T>
	inline Matrix3<T> Quaternion<T>::ToMatrix3(const Quaternion<T> &q)
	{
		Matrix3<T> tmpMat;
		tmpMat.elements[0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
		tmpMat.elements[3] = 2 * q.x * q.y - 2 * q.w * q.z;
		tmpMat.elements[7] = 2 * q.x * q.z + 2 * q.w * q.y;

		tmpMat.elements[1] = 2 * q.x * q.y + 2 * q.w * q.z;
		tmpMat.elements[4] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
		tmpMat.elements[7] = 2 * q.y * q.z - 2 * q.w * q.x;

		tmpMat.elements[2] = 2 * q.x * q.z - 2 * q.w * q.y;
		tmpMat.elements[5] = 2 * q.y * q.z + 2 * q.w * q.x;
		tmpMat.elements[8] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
		return tmpMat;
	}
}