#pragma once
#include <memory>
#include <mutex>
#include "NonCopyable.h"

namespace RealSix
{
	template <typename T>
	class Singleton : public NonCopyable
	{
	public:
		static T &GetInstance() //  Meyers' Singleton
		{
			static T sInstance;
			return sInstance;
		}

	protected:
		Singleton() = default;
		~Singleton() = default;
	};
}