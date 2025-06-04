#pragma once
#include <cmath>
#include <limits>
#include <cstdint>
namespace RealSix::Math
{
	constexpr float PI = 3.1415926535f;
	constexpr float TWO_PI = PI * 2.0f;

	template <typename T>
	inline T ToRadian(const T &degree)
	{
		return degree * PI / 180.0f;
	}

	template <typename T>
	inline T ToDegree(const T &radian)
	{
		return radian * 180.0f / PI;
	}

	template <typename T>
	inline bool IsNearZero(const T &value)
	{
		if (abs(value) <= std::numeric_limits<T>::epsilon())
			return true;
		else
			return false;
	}

	template<typename T>
	inline T Floor(const T& value)
	{
		return std::floor(value);
	}

	template<typename T>
	inline T Ceil(const T& value)
	{
		return ceil(value);
	}

	template <typename T>
	inline T Round(const T &value)
	{
		return std::round(value);
	}

	template <typename T>
	inline T RoundUp(T numToRound, T multiple)
	{
		return ((numToRound + multiple - 1) / multiple) * multiple;
	}

	template <typename T>
	inline T Abs(const T &value)
	{
		return std::abs(value);
	}

	template <typename T>
	inline T Max(const T &left, const T &right)
	{
		return left > right ? left : right;
	}

	template <typename T>
	inline T Min(const T &left, const T &right)
	{
		return left > right ? right : left;
	}

	inline bool IsOddNumber(int value)
	{
		return value / 2 == 1 ? true : false;
	}

	inline bool IsEvenNumber(int value)
	{
		return value / 2 == 0 ? true : false;
	}

	template <typename T>
	inline T Clamp(const T &value, const T &minNum, const T &maxNum)
	{
		return Min(maxNum, Max(minNum, value));
	}

	template <typename T>
	inline T Mix(const T &src, const T &dst, const T &percent)
	{
		return (1 - percent) * src + percent * dst;
	}

	template <typename T1, typename T2>
	inline T1 Pow(const T1 &radix, const T2 &power)
	{
		return std::pow(radix, power);
	}

	template <typename T>
	inline T Sqrt(const T &value)
	{
		return std::sqrt(value);
	}

	template <typename T>
	inline T Sin(const T &radian)
	{
		return std::sin(radian);
	}

	template <typename T>
	inline T Cos(const T &radian)
	{
		return std::cos(radian);
	}

	template <typename T>
	inline T Tan(const T &radian)
	{
		return std::tan(radian);
	}

	template <typename T>
	inline T Cot(const T &radian)
	{
		return 1.0f / Tan(radian);
	}

	template <typename T>
	inline T ArcSin(const T &value)
	{
		return std::asin(value);
	}

	template <typename T>
	inline T ArcCos(const T &value)
	{
		return std::acos(value);
	}

	template <typename T>
	inline T ArcTan(const T &value)
	{
		return std::atan(value);
	}

	template <typename T>
	inline T ArcTan2(const T &value)
	{
		return std::atan2(value);
	}

	template <typename T, typename T2>
	inline T Lerp(const T &src, const T &dst, T2 percent)
	{
		return src * (1 - percent) + dst * percent;
	}

	template <typename T>
	inline constexpr bool IsPowerOfTwo(T value)
	{
		return value != 0 && (value & (value - 1)) == 0;
	}
	template <typename T>
	inline constexpr T RoundToPowerOfTwo(T value, int32_t POT)
	{
		return (value + POT - 1) & -POT;
	}
	template <typename T>
	inline constexpr T NumMipmapLevels(T width, T height)
	{
		T levels = 1;
		while ((width | height) >> levels)
			++levels;
		return levels;
	}

	template <typename T>
	inline T GetBiggerTwoPower(T val)
	{
		if (val & (val - 1))
		{
			val |= val >> 1;
			val |= val >> 2;
			val |= val >> 4;
			val |= val >> 8;
			val |= val >> 16;
			return val + 1;
		}
		else
			return val == 0 ? 1 : val;
	}

	inline uint32_t AlignTo(uint32_t value, uint32_t alignment)
	{
		return (value + alignment - 1) & ~(alignment - 1);
	}
};
