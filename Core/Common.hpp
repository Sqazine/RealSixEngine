#pragma once
#include "String.hpp"
namespace RealSix
{
	template <typename T>
	struct TIsPointer
	{
		enum
		{
			Value = false
		};
	};

	template <typename T>
	struct TIsPointer<T *>
	{
		enum
		{
			Value = true
		};
	};

	template <typename T>
	struct TIsPointer<const T>
	{
		enum
		{
			Value = TIsPointer<T>::Value
		};
	};
	template <typename T>
	struct TIsPointer<volatile T>
	{
		enum
		{
			Value = TIsPointer<T>::Value
		};
	};
	template <typename T>
	struct TIsPointer<const volatile T>
	{
		enum
		{
			Value = TIsPointer<T>::Value
		};
	};

	template <typename T, size_t Count>
	inline constexpr size_t CountOf(T (&array)[Count])
	{
		return Count;
	}

	template <typename T>
	inline String GetPrettyTypeName()
	{
		String name = typeid(T).name();
#ifdef _MSC_VER
		size_t idx = name.FindLastOf("::") + 1;
		if (idx < name.Size())
		{
			name = name.SubStr(idx);
		}
#endif
		return name;
	}

	class NonCopyable
	{
	public:
		NonCopyable() = default;
		virtual ~NonCopyable() = default;

		NonCopyable(const NonCopyable &) = delete;
		const NonCopyable &operator=(const NonCopyable &) = delete;

		NonCopyable(NonCopyable &&) = default;
		NonCopyable &operator=(NonCopyable &&) = default;
	};

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
