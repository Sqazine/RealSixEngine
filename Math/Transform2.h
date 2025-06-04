#pragma once
#include "Vector3.h"
#include "Quaternion.h"
namespace RealSix
{
	template <typename T>
	class Vector2;

	template <typename T>
	class Vector4;

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
}