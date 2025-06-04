#pragma once
#include <string>
namespace RealSix
{
	template <typename T>
	struct TIsPointer
	{
		enum { Value = false };
	};

	template <typename T> struct TIsPointer<T *> { enum { Value = true }; };

	template <typename T> struct TIsPointer<const          T> { enum { Value = TIsPointer<T>::Value }; };
	template <typename T> struct TIsPointer<      volatile T> { enum { Value = TIsPointer<T>::Value }; };
	template <typename T> struct TIsPointer<const volatile T> { enum { Value = TIsPointer<T>::Value }; };

	template <typename T, size_t Count>
	inline constexpr size_t CountOf(T (&array)[Count]) 
	{
    	return Count;
	}

	template<typename T>
	inline std::string GetPrettyTypeName()
	{
		std::string name = typeid(T).name();
#ifdef _MSC_VER
		name = name.substr(name.find_last_of("::")+1);
#endif
		return name;
	}
}
