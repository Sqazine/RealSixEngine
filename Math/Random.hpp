#pragma once
#include <random>
#include "Vector2.hpp"
#include "Vector3.hpp"
namespace RealSix
{
	class Random
	{
	public:
		static void Init();

		static float GetFloat(float min, float max);

		static float GetFloat01();

		static int GetInt(int min, int max);
		template <typename T>
		static Vector2<T> GetVector2(const Vector2<T> &min, const Vector2<T> &max);
		template <typename T>
		static Vector3<T> GetVector3(const Vector3<T> &min, const Vector3<T> &max);
		template <typename T>
		Vector3<T> GetVector3InUnitDisk();

	private:
		static std::mt19937 mRandomNumGenerator;
	};
	template <typename T>
	Vector2<T> Random::GetVector2(const Vector2<T> &min, const Vector2<T> &max)
	{
		Vector2 tmp((max.x - min.x) * GetFloat(0.0f, 1.0f), (max.y - min.y) * GetFloat(0.0f, 1.0f));

		return min + tmp;
	}
	template <typename T>
	Vector3<T> Random::GetVector3(const Vector3<T> &min, const Vector3<T> &max)
	{
		Vector3<T> tmp((max.x - min.x) * GetFloat(0.0f, 1.0f), (max.y - min.y) * GetFloat(0.0f, 1.0f), (max.z - min.z) * GetFloat(0.0f, 1.0f));
		return min + tmp;
	}
	template <typename T>
	Vector3<T> Random::GetVector3InUnitDisk()
	{
		Vector3<T> result;
		do
		{
			result = GetVector3<T>(Vector3<T>(-1.0f, -1.0f, -1.0f), Vector3<T>(1.0f, 1.0f, 1.0f));
		} while (result.Length() > 1);
		return result;
	}
}