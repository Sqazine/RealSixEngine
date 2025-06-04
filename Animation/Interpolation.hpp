#pragma once
#include "Math/Math.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector4.hpp"
#include "Math/Quaternion.hpp"

namespace RealSix
{
	enum class Interpolation
	{
		Constant,
		Linear,
		Cubic
	};

	inline float Interpolate(float a, float b, float t)
	{
		return Math::Lerp(a, b, t);
	}

	inline Vector3f Interpolate(const Vector3f &a, const Vector3f &b, float t)
	{
		return Vector3f::Lerp(a, b, t);
	}

	inline Quaternionf Interpolate(const Quaternionf &a, const Quaternionf &b, float t)
	{
		Quaternionf result = Quaternionf::Lerp(a, b, t);
		if (Quaternionf::Dot(a, b) < 0)
			result = Quaternionf::Lerp(a, -b, t);
		return Quaternionf::Normalize(result);
	}

	inline float AdjustHermiteResult(float f)
	{
		return f;
	}

	inline Vector3f AdjustHermiteResult(const Vector3f &vec)
	{
		return vec;
	}

	inline Quaternionf AdjustHermiteResult(const Quaternionf &quaterion)
	{
		return Quaternionf::Normalize(quaterion);
	}
	inline void Neighborhood(const float &a, float &b)
	{
	}
	inline void Neighborhood(const Vector3f &a, Vector3f &b)
	{
	}
	inline void Neighborhood(const Quaternionf &a, Quaternionf &b)
	{
		if (Quaternionf::Dot(a, b) < 0)
			b = -b;
	}
}