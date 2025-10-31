#pragma once
#include <string>
#include <locale>
#include <codecvt>
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
	inline std::string GetPrettyTypeName()
	{
		std::string name = typeid(T).name();
#ifdef _MSC_VER
		name = name.substr(name.find_last_of("::") + 1);
#endif
		return name;
	}

	namespace Utf8
	{
		inline std::string Encode(const std::wstring &str)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			return converter.to_bytes(str);
		}

		inline std::wstring Decode(const std::string &str)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			return converter.from_bytes(str);
		}
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
