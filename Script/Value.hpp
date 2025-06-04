#pragma once
#include "String.hpp"
#include <unordered_map>
#include "Utils.hpp"
namespace RealSix::Script
{
	enum ValueKind : uint8_t
	{
		NIL,
		INT,
		FLOAT,
		BOOL,
		OBJECT,
	};

	struct REALSIX_API Value
	{
		Value() noexcept;
		Value(int64_t integer) noexcept;
		Value(double number) noexcept;
		Value(bool boolean) noexcept;
		Value(struct Object *object) noexcept;
		~Value() noexcept = default;

		String ToString() const;
		void Mark() const;
		void UnMark() const;

		std::vector<uint8_t> Serialize() const;
		void Deserialize(const std::vector<uint8_t> &data);

		ValueKind kind;
		Permission permission = Permission::MUTABLE;

		union
		{
			int64_t integer;
			double floating;
			bool boolean;
			struct Object *object;
		};
	};

	REALSIX_API bool operator==(const Value &left, const Value &right);
	REALSIX_API bool operator!=(const Value &left, const Value &right);

	struct REALSIX_API ValueHash
	{
		size_t operator()(const Value *v) const;
		size_t operator()(const Value &v) const;
	};

	using ValueUnorderedMap = std::unordered_map<Value, Value, ValueHash>;

	size_t HashValueList(Value *start, size_t count);
	size_t HashValueList(Value *start, Value *end);
}