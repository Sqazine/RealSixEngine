#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <ostream>
#include "Marco.hpp"

namespace RealSix
{
	template <typename T>
	requires(std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) constexpr inline size_t HashString(T str)
	{
		size_t hash = 2166136261u;
		for (size_t i = 0; i < str.size(); i++)
		{
			hash ^= (uint8_t)str[i];
			hash *= 16777619;
		}
		return hash;
	}

	class String;
	class StringView;

	class REALSIX_API String
	{
	public:
		String() = default;
		String(std::string_view str);
		String(StringView str);
		String(const std::string &str);
		String(const char *str);
		String(size_t count, char ch);
		String(const String &other);
		String(String &&other) noexcept;
		~String() = default;

		bool Empty() const;
		size_t Size() const;
		size_t FindFirstOf(const char *subStr, size_t offset = 0);
		size_t FindLastOf(const String &subStr, size_t offset = std::string::npos);
		size_t Find(const String &subStr, size_t offset = 0);
		size_t Find(char ch);

		String &Append(const String &str, size_t idx, size_t count);
		String &Append(size_t idx, char ch);

		bool IsAtLast(size_t index) const;
		String SubStr(size_t offset, size_t len = std::string::npos);

		String &Replace(size_t pos, size_t len, const String &str);

		String &Erase(size_t index);
		String &Insert(size_t index, const String &data);
		String &Insert(size_t index, size_t count, char ch);

		void Clear();

		const std::string &GetRawData() const;
		const char *CString() const;
		uint64_t GetHash() const;

		double ToDouble() const;
		int64_t ToInt64() const;

		String &operator=(const String &other);
		String &operator=(String &&other);
		String &operator=(StringView other);
		String &operator=(const char *other);
		bool operator<(const String &other) const;
		String &operator+=(const String &other);
		char &operator[](size_t index);
		String operator+(const char *other) const;
		String operator+(const std::string &other) const;
		String operator+(std::string_view other) const;

		bool operator==(const String &other) const;
		bool operator!=(const String &other) const;

	private:
		std::string mString{""};
		uint64_t mHash{0};
	};

	class REALSIX_API StringView
	{
	public:
		StringView() = default;
		StringView(std::string_view str);
		StringView(const std::string &str);
		StringView(const String &str);
		StringView(const char *str);
		StringView(StringView &&other);
		StringView(const StringView &other);
		~StringView() = default;

		bool Empty() const;
		size_t Size() const;

		uint64_t GetHash() const;
		std::string_view GetRawData() const;

		const char *CString() const;
		
		StringView SubStr(size_t offset, size_t len = std::string_view::npos) const;

		StringView &operator=(const StringView &other);
		StringView &operator=(StringView &&other);
		bool operator==(StringView other) const;
		bool operator!=(StringView other) const;
		const char &operator[](size_t index) const ;

	private:
		std::string_view mStringView{};
		uint64_t mHash{0};
	};

	String operator+(const String &lhs, const String &rhs);
	String operator+(const char *lhs, const String &rhs);
	String operator+(const char *lhs, StringView rhs);
	bool operator==(const String &lhs, const StringView &rhs);
	bool operator==(const String &lhs, const char *rhs);
	bool operator==(const StringView &lhs, const char *rhs);
	std::ostream &operator<<(std::ostream &os, const String &str);
	std::ostream &operator<<(std::ostream &os, StringView str);
}

namespace std
{
	template <>
	struct hash<RealSix::StringView>
	{
		inline std::size_t operator()(RealSix::StringView v) const
		{
			return std::hash<std::string_view>()(v.GetRawData()) ^ std::hash<int64_t>()(v.GetHash());
		}
	};
}

namespace std
{
	template <>
	struct hash<RealSix::String>
	{
		inline std::size_t operator()(const RealSix::String &v) const
		{
			return std::hash<std::string>()(v.GetRawData()) ^ std::hash<int64_t>()(v.GetHash());
		}
	};
}