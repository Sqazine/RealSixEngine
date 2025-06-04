#include "Random.hpp"
namespace RealSix
{
	std::mt19937 Random::mRandomNumGenerator;

	void Random::Init()
	{
		std::random_device rd;
		mRandomNumGenerator.seed(rd());
	}

	float Random::GetFloat(float min, float max)
	{
		std::uniform_real_distribution<float> dis(min, max);
		return dis(mRandomNumGenerator);
	}

	float Random::GetFloat01()
	{
		return GetFloat(0.0f, 1.0f);
	}

	int Random::GetInt(int min, int max)
	{
		std::uniform_int_distribution<int> dis(min, max);
		return dis(mRandomNumGenerator);
	}
}