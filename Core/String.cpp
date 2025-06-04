#include "String.hpp"

namespace RealSix
{
    String::String(std::string_view str)
        : mString(str), mHash(HashString(mString))
    {
    }

    String::String(StringView str)
        : mString(str.CString()), mHash(str.GetHash())
    {
    }

    String::String(const std::string &str)
        : mString(str), mHash(HashString(mString))
    {
    }

    String::String(const char *str)
        : mString(str), mHash(HashString(mString))
    {
    }

    String::String(size_t count, char ch)
        : mString(count, ch), mHash(HashString(mString))
    {
    }

    String::String(const String &other)
        : mString(other.mString), mHash(other.mHash)
    {
    }

    String::String(String &&other) noexcept
        : mString(other.mString), mHash(other.mHash)
    {
    }

    bool String::Empty() const
    {
        return mString.empty();
    }

    size_t String::Size() const
    {
        return mString.size();
    }

    size_t String::FindFirstOf(const char *subStr, size_t offset)
    {
        return mString.find_first_of(subStr,offset);
    }

    size_t String::FindLastOf(const String &subStr, size_t offset)
    {
        return mString.find_last_of(subStr.mString, offset);
    }

    size_t String::Find(const String &subStr, size_t offset)
    {
        return mString.find(subStr.mString, offset);
    }

    size_t String::Find(char ch)
    {
        return mString.find(ch);
    }

    String &String::Append(const String &str, size_t idx, size_t count)
    {
        mString.append(str.GetRawData(), idx, count);
        return *this;
    }

    String &String::Append(size_t idx, char ch)
    {
        mString.append(idx, ch);
        return *this;
    }

    bool String::IsAtLast(size_t index) const
    {
        return index >= mString.size();
    }

    String String::SubStr(size_t offset, size_t len)
    {
        return mString.substr(offset, len);
    }

    String &String::Replace(size_t pos, size_t len, const String &str)
    {
        mString.replace(pos, len, str.GetRawData());
        return *this;
    }

    String &String::Erase(size_t index)
    {
        mString.erase(mString.begin() + index);
        return *this;
    }

    String &String::Insert(size_t index, const String &data)
    {
        std::string newData = data.GetRawData();
        mString.insert(mString.begin() + index, newData.begin(), newData.end());
        return *this;
    }

    String &String::Insert(size_t index, size_t count, char ch)
    {
        mString.insert(index, count, ch);
        return *this;
    }

    void String::Clear()
    {
        mString.clear();
    }

    const std::string &String::GetRawData() const
    {
        return mString;
    }

    const char *String::CString() const
    {
        return mString.c_str();
    }

    uint64_t String::GetHash() const
    {
        return mHash;
    }

    double String::ToDouble() const
    {
        return std::stod(mString);
    }

    int64_t String::ToInt64() const
    {
        return std::stoll(mString);
    }

    String &String::operator=(const String &other)
    {
        mString = other.mString;
        mHash = other.mHash;
        return *this;
    }

    String &String::operator=(String &&other)
    {
        mString = other.mString;
        mHash = other.mHash;
        return *this;
    }

    String &String::operator=(StringView other)
    {
        mString = other.GetRawData();
        mHash = other.GetHash();
        return *this;
    }

    String &String::operator=(const char *other)
    {
        mString = other;
        mHash = HashString(mString);
        return *this;
    }

    bool String::operator<(const String &other) const
    {
        return mString < other.mString;
    }

    String &String::operator+=(const String &other)
    {
        mString += other.mString;
        mHash = HashString(mString);
        return *this;
    }

    char &String::operator[](size_t index)
    {
        return mString[index];
    }

    String String::operator+(const char *other) const
    {
        return String(mString + std::string(other));
    }

    String String::operator+(const std::string &other) const
    {
        return String(mString + other);
    }

    String String::operator+(std::string_view other) const
    {
        return String(mString + std::string(other));
    }

    bool String::operator==(const String &other) const
    {
        return mHash == other.mHash;
    }

    bool String::operator!=(const String &other) const
    {
        return mHash != other.mHash;
    }

    String operator+(const String &lhs, const String &rhs)
    {
        return String(lhs.GetRawData() + rhs.GetRawData());
    }

    String operator+(const char *lhs, const String &rhs)
    {
        String result = String(lhs);
        result += rhs;
        return result;
    }

    String operator+(const char *lhs, StringView rhs)
    {
        String result = String(lhs);
        result += rhs;
        return result;
    }

    bool operator==(const String &lhs, const StringView &rhs)
    {
        return lhs.GetHash() == rhs.GetHash();
    }

    bool operator==(const String &lhs, const char *rhs)
    {
        return strcmp(lhs.CString(), rhs) == 0;
    }

    std::ostream &operator<<(std::ostream &os, const String &str)
    {
        os << str.GetRawData();
        return os;
    }

    StringView::StringView(std::string_view str)
        : mStringView(str), mHash(HashString(mStringView))
    {
    }
    StringView::StringView(const std::string &str)
        : mStringView(str), mHash(HashString(mStringView))
    {
    }
    StringView::StringView(const String &str)
        : mStringView(str.GetRawData()), mHash(str.GetHash())
    {
    }

    StringView::StringView(const char *str)
        : mStringView(str), mHash(HashString(mStringView))
    {
    }

    StringView::StringView(StringView &&other)
        : mStringView(other.mStringView), mHash(other.mHash)
    {
    }

    StringView::StringView(const StringView &other)
        : mStringView(other.mStringView), mHash(other.mHash)
    {
    }

    bool StringView::Empty() const
    {
        return mStringView.empty();
    }

    size_t StringView::Size() const
    {
        return mStringView.size();
    }

    uint64_t StringView::GetHash() const
    {
        return mHash;
    }

    std::string_view StringView::GetRawData() const
    {
        return mStringView;
    }

    const char *StringView::CString() const
    {
        return mStringView.data();
    }

   StringView StringView::SubStr(size_t offset, size_t len) const
    {
        return  mStringView.substr(offset,len);
    }

    StringView &StringView::operator=(const StringView &other)
    {
        this->mStringView = other.mStringView;
        this->mHash = other.mHash;
        return *this;
    }
    StringView &StringView::operator=(StringView &&other)
    {
        this->mStringView = other.mStringView;
        this->mHash = other.mHash;
        return *this;
    }

    bool StringView::operator==(StringView other) const
    {
        return mHash == other.mHash;
    }

    bool StringView::operator!=(StringView other) const
    {
        return mHash != other.mHash;
    }

    const char &StringView::operator[](size_t index) const
    {
        return mStringView[index];
    }

    bool operator==(const StringView &lhs, const char *rhs)
    {
        return strcmp(lhs.CString(), rhs) == 0;
    }

    std::ostream &operator<<(std::ostream &os, StringView str)
    {
        os << str.GetRawData();
        return os;
    }
}