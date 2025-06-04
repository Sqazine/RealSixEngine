#include "Type.hpp"
#include "Logger.hpp"
namespace RealSix::Script
{
    constexpr struct
    {
        const char* name;
        TypeKind kind;
    } gPrimitiveTypeMap[] = {
        {"i8", TypeKind::I8},
        {"u8", TypeKind::U8},
        {"i16", TypeKind::I16},
        {"u16", TypeKind::U16},
        {"i32", TypeKind::I32},
        {"u32", TypeKind::U32},
        {"i64", TypeKind::I64},
        {"u64", TypeKind::U64},
        {"f32", TypeKind::F32},
        {"f64", TypeKind::F64},
        {"bool", TypeKind::BOOL},
        {"char", TypeKind::CHAR},
        {"str", TypeKind::STR},
        {"any", TypeKind::ANY},
    };

    Type::Type() noexcept
        : mKind(TypeKind::UNDEFINED), mName("undefined")
    {
    }

    Type::Type(TypeKind kind, const SourceLocation &sourceLocation) noexcept
        : mSourceLocation(sourceLocation)
    {
        for (const auto &p : gPrimitiveTypeMap)
        {
            if (p.kind == kind)
            {
                mName = p.name;
                mKind = p.kind;
                return;
            }
        }

        CheckIsValid();
    }

    Type::Type(StringView name, const SourceLocation &sourceLocation) noexcept
        : mSourceLocation(sourceLocation)
    {
        for (const auto &p : gPrimitiveTypeMap)
        {
            if (p.name == name)
            {
                mName = p.name;
                mKind = p.kind;
                return;
            }
        }

        CheckIsValid();
    }

    Type::Type(TypeKind kind, StringView name, const SourceLocation &sourceLocation) noexcept
        : mName(name), mKind(kind), mSourceLocation(sourceLocation)
    {
    }

    TypeKind Type::GetKind() const noexcept
    {
        return mKind;
    }

    bool Type::IsNumeric() const noexcept
    {
        return IsInteger() || IsFloating();
    }

    bool Type::IsInteger() const noexcept
    {
        return mKind <= TypeKind::U64;
    }

    bool Type::IsFloating() const noexcept
    {
        return mKind == TypeKind::F32 || mKind == TypeKind::F64;
    }

    StringView Type::GetName() const noexcept
    {
        return mName;
    }

    bool Type::IsPrimitiveType() const noexcept
    {
        return mKind >= TypeKind::I8 && mKind <= TypeKind::PRIMITIVE;
    }

    bool Type::IsCompositeType() const noexcept
    {
        return !IsPrimitiveType();
    }

    void Type::CheckIsValid() const
    {
        if (mKind == TypeKind::UNDEFINED)
        {
            REALSIX_SCRIPT_LOG_ERROR(mSourceLocation,"Type is undefined, please check your code.");
        }
        if (mName.Empty())
        {
            REALSIX_SCRIPT_LOG_ERROR(mSourceLocation,"Type name is empty, please check your code.");
        }
    }
}