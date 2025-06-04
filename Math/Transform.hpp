#pragma once
#include "Vector3.hpp"
#include "Quaternion.hpp"
namespace RealSix
{
	template <typename T>
	class Vector2;
	template <typename T>
	class Vector3;

	template <typename T>
	class Vector4;

	template <typename T>
	class Quaternion;

	template <typename T>
	class DualQuaternion;

	template <typename T>
	class Transform2
	{
	public:
		Vector2<T> position;
		T rotation;
		Vector2<T> scale;

		Transform2();
		Transform2(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s);

		static Matrix4<T> ToMatrix4(const Transform2<T> &trans);
	};

	using Transform2f = Transform2<float>;
	using Transform2d = Transform2<double>;
	using Transform2i32 = Transform2<int32_t>;
	using Transform2u32 = Transform2<uint32_t>;
	using Transform2i16 = Transform2<int16_t>;
	using Transform2u16 = Transform2<uint16_t>;
	using Transform2i8 = Transform2<int8_t>;
	using Transform2u8 = Transform2<uint8_t>;

	//TODO : not finished yet

	template <typename T>
	class Transform3
	{
	public:
		Vector3<T> position;
		Quaternion<T> rotation;
		Vector3<T> scale;

		Transform3();
		Transform3(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s);

		static Transform3<T> Combine(const Transform3<T> &a, const Transform3<T> &b);
		static Transform3<T> Inverse(const Transform3<T> &t);
		static Transform3<T> Interpolate(const Transform3<T> &a, const Transform3<T> &b, float t);

		static Matrix4<T> ToMatrix4(const Transform3<T> &trans);

		static Vector3<T> TransformPoint(const Transform3<T> &a, const Vector3<T> &b);
		static Vector3<T> TransformVector(const Transform3<T> &a, const Vector3<T> &b);

		static Vector3<T> TransformPoint(const DualQuaternion<T> &dq, const Vector3<T> &b);
		static Vector3<T> TransformVector(const DualQuaternion<T> &dq, const Vector3<T> &b);

		static DualQuaternion<T> ToDualQuaternion(const Transform3<T> &t);
	};

	using Transform3f = Transform3<float>;
	using Transform3d = Transform3<double>;
	using Transform3i32 = Transform3<int32_t>;
	using Transform3u32 = Transform3<uint32_t>;
	using Transform3i16 = Transform3<int16_t>;
	using Transform3u16 = Transform3<uint16_t>;
	using Transform3i8 = Transform3<int8_t>;
	using Transform3u8 = Transform3<uint8_t>;

	template <typename T>
	inline Transform3<T>::Transform3()
		: position(Vector3<T>::ZERO), rotation(Quaternion<T>::ZERO), scale(Vector3<T>(static_cast<T>(1.0f)))
	{
	}

	template <typename T>
	inline Transform3<T>::Transform3(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s)
		: position(p), rotation(r), scale(s)
	{
	}
	template <typename T>
	inline Transform3<T> Transform3<T>::Combine(const Transform3<T> &a, const Transform3<T> &b)
	{
		Transform3<T> tmp;
		tmp.scale = a.scale * b.scale;
		tmp.rotation = Quaternionf::Concatenate(a.rotation, b.rotation);
		tmp.position = a.rotation * (a.scale * b.position);
		tmp.position = a.position + tmp.position;
		return tmp;
	}
	template <typename T>
	inline Transform3<T> Transform3<T>::Inverse(const Transform3<T> &t)
	{
		Transform3<T> tmp;
		tmp.rotation = Quaternion<T>::Inverse(t.rotation);
		tmp.scale.x = Math::Abs(t.scale.x) < std::numeric_limits<float>::epsilon() ? 0.0f : 1.0f / t.scale.x;
		tmp.scale.y = Math::Abs(t.scale.y) < std::numeric_limits<float>::epsilon() ? 0.0f : 1.0f / t.scale.y;
		tmp.scale.z = Math::Abs(t.scale.z) < std::numeric_limits<float>::epsilon() ? 0.0f : 1.0f / t.scale.z;
		tmp.position = -(tmp.rotation * (tmp.scale * t.position));
		return tmp;
	}

	template <typename T>
	inline Transform3<T> Transform3<T>::Interpolate(const Transform3<T> &a, const Transform3<T> &b, float t)
	{
		Quaternion<T> bRot = b.rotation;
		if (Quaternion<T>::Dot(a.rotation, bRot) < 0.0f)
			bRot = -bRot;

		return Transform3<T>(Vector3<T>::Lerp(a.position, b.position, t), Quaternion<T>::NLerp(a.rotation, bRot, t), Vector3<T>::Lerp(a.scale, b.scale, t));
	}

	template <typename T>
	inline Matrix4<T> Transform3<T>::ToMatrix4(const Transform3<T> &trans)
	{
		Matrix4<T> world = Matrix4<T>::Translate(trans.position);
		world *= Quaternion<T>::ToMatrix4(trans.rotation);
		world *= Matrix4<T>::Scale(trans.scale);
		return world;
	}

	template <typename T>
	inline Vector3<T> Transform3<T>::TransformPoint(const Transform3<T> &a, const Vector3<T> &b)
	{
		Vector3<T> out;
		out = a.rotation * (a.scale * b);
		out = a.position + out;
		return out;
	}

	template <typename T>
	inline Vector3<T> Transform3<T>::TransformVector(const Transform3<T> &a, const Vector3<T> &b)
	{
		Vector3<T> out;
		out = a.rotation * (a.scale * b);
		return out;
	}

	template <typename T>
	inline Vector3<T> TransformPoint(const DualQuaternion<T> &dq, const Vector3<T> &b)
	{
		Quaternion<T> d = Quaternion<T>::Conjuate(dq.real) * (dq.dual * 2.0f);
		Vector3<T> t = d.vec;
		return dq.real * b + t;
	}

	template <typename T>
	inline Vector3<T> TransformVector(const DualQuaternion<T> &dq, const Vector3<T> &b)
	{
		return dq.real * b;
	}

	template <typename T>
	inline DualQuaternion<T> Transform3<T>::ToDualQuaternion(const Transform3<T> &t)
	{
		Quaternion<T> d;
		d.vec = t.position;
		d.scalar = 0.0f;

		Quaternion<T> qr = t.rotation;

		Quaternion<T> qd = qr * d * 0.5f;

		return DualQuaternion<T>(qr, qd);
	}
}