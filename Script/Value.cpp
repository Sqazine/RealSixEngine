#include "Value.hpp"
#include "Object.hpp"
namespace RealSix::Script
{
    Value::Value() noexcept
        : kind(ValueKind::NIL), object(nullptr)
    {
    }
    Value::Value(double number) noexcept
        : floating(number), kind(ValueKind::FLOAT)
    {
    }

    Value::Value(int64_t integer) noexcept
        : integer(integer), kind(ValueKind::INT)
    {
    }
    Value::Value(bool boolean) noexcept
        : boolean(boolean), kind(ValueKind::BOOL)
    {
    }

    Value::Value(Object *object) noexcept
        : object(object), kind(ValueKind::OBJECT)
    {
    }

    String Value::ToString() const
    {
        switch (kind)
        {
        case ValueKind::INT:
            return std::to_string(integer);
        case ValueKind::FLOAT:
            return std::to_string(floating);
        case ValueKind::BOOL:
            return boolean ? "true" : "false";
        case ValueKind::NIL:
            return "null";
        case ValueKind::OBJECT:
            return object->ToString();
        default:
            return "null";
        }
        return "null";
    }
    void Value::Mark() const
    {
        if (kind == ValueKind::OBJECT)
            object->Mark();
    }
    void Value::UnMark() const
    {
        if (kind == ValueKind::OBJECT)
            object->UnMark();
    }

    std::vector<uint8_t> Value::Serialize() const
    {
        std::vector<uint8_t> result;

        result.emplace_back(kind);
        result.emplace_back(static_cast<uint8_t>(std::underlying_type<Permission>::type(permission)));

        if (IS_INT_VALUE(*this) || IS_FLOAT_VALUE(*this) || IS_BOOL_VALUE(*this))
        {
            auto byteList = ByteConverter::ToU64ByteList(integer);
            result.insert(result.end(), byteList.begin(), byteList.end());
        }

        // TODO: Not finished yet, need to handle object serialization

        return result;
    }

    void Value::Deserialize(const std::vector<uint8_t> &data)
    {
        kind = (ValueKind)data[0];
        permission = (Permission)data[1];

        if (IS_INT_VALUE(*this) || IS_FLOAT_VALUE(*this) || IS_BOOL_VALUE(*this))
            integer = ByteConverter::GetU64Integer(data, 2);

        // TODO: Not finished yet, need to handle object deserialization
    }

    bool operator==(const Value &left, const Value &right)
    {
        switch (left.kind)
        {
        case ValueKind::INT:
        {
            if (IS_INT_VALUE(right))
                return TO_INT_VALUE(left) == TO_INT_VALUE(right);
            else if (IS_FLOAT_VALUE(right))
                return TO_INT_VALUE(left) == TO_FLOAT_VALUE(right);
            else
                return false;
        }
        case ValueKind::FLOAT:
        {
            if (IS_INT_VALUE(right))
                return TO_FLOAT_VALUE(left) == TO_INT_VALUE(right);
            else if (IS_FLOAT_VALUE(right))
                return TO_FLOAT_VALUE(left) == TO_FLOAT_VALUE(right);
            else
                return false;
        }
        case ValueKind::NIL:
            return IS_NULL_VALUE(right);
        case ValueKind::BOOL:
        {
            if (IS_BOOL_VALUE(right))
                return TO_BOOL_VALUE(left) == TO_BOOL_VALUE(right);
            else
                return false;
        }
        case ValueKind::OBJECT:
        {
            if (IS_OBJECT_VALUE(right))
                return TO_OBJECT_VALUE(left)->IsEqualTo(TO_OBJECT_VALUE(right));
            else
                return false;
        }
        default:
            return false;
        }
        return false;
    }

    bool operator!=(const Value &left, const Value &right)
    {
        return !(left == right);
    }

    size_t ValueHash::operator()(const Value *v) const
    {
        switch (v->kind)
        {
        case ValueKind::NIL:
            return std::hash<ValueKind>()(v->kind);
        case ValueKind::INT:
            return std::hash<ValueKind>()(v->kind) ^ std::hash<int64_t>()(v->integer);
        case ValueKind::FLOAT:
            return std::hash<ValueKind>()(v->kind) ^ std::hash<double>()(v->floating);
        case ValueKind::BOOL:
            return std::hash<ValueKind>()(v->kind) ^ std::hash<bool>()(v->boolean);
        case ValueKind::OBJECT:
            return std::hash<ValueKind>()(v->kind) ^ std::hash<Object *>()(v->object);
        default:
            return std::hash<ValueKind>()(v->kind);
        }
    }

    size_t ValueHash::operator()(const Value &v) const
    {
        return ValueHash()(&v);
    }

    size_t HashValueList(Value *start, size_t count)
    {
        std::size_t seed = count;
        for (size_t i = 0; i < count; ++i)
            seed ^= ValueHash()((start + i)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    size_t HashValueList(Value *start, Value *end)
    {
        return HashValueList(start, end - start);
    }

	Value *GetEndOfRefValuePtr(Value *v)
	{
		auto result = v;
		while (IS_REF_VALUE(*result))
			result = TO_REF_VALUE(*result)->pointer;
		return result;
	}

	void GetActualValueIfIsRefValue(const Value &v, Value &result)
	{
		result = v;
		while (IS_REF_VALUE(result))
			result = *TO_REF_VALUE(result)->pointer;
	}
}