#pragma once
#include "String.hpp"
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include "Chunk.hpp"
#include "Token.hpp"
#include "Value.hpp"
namespace RealSix::Script
{
#define IS_STR_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::STR)
#define IS_ARRAY_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::ARRAY)
#define IS_TABLE_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::DICT)
#define IS_STRUCT_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::STRUCT)
#define IS_FUNCTION_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::FUNCTION)
#define IS_UPVALUE_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::UPVALUE)
#define IS_CLOSURE_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::CLOSURE)
#define IS_NATIVE_FUNCTION_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::NATIVE_FUNCTION)
#define IS_REF_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::REF)
#define IS_CLASS_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::CLASS)
#define IS_CLASS_INSTANCE_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::CLASS_INSTANCE)
#define IS_CLASS_CLOSURE_BIND_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::CLASS_CLOSURE_BIND)
#define IS_ENUM_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::ENUM)
#define IS_MODULE_OBJ(obj) ((obj)->kind == ::RealSix::Script::ObjectKind::MODULE)

#define TO_STR_OBJ(obj) ((::RealSix::Script::StrObject *)(obj))
#define TO_ARRAY_OBJ(obj) ((::RealSix::Script::ArrayObject *)(obj))
#define TO_TABLE_OBJ(obj) ((::RealSix::Script::DictObject *)(obj))
#define TO_STRUCT_OBJ(obj) ((::RealSix::Script::StructObject *)(obj))
#define TO_FUNCTION_OBJ(obj) ((::RealSix::Script::FunctionObject *)(obj))
#define TO_UPVALUE_OBJ(obj) ((::RealSix::Script::UpValueObject *)(obj))
#define TO_CLOSURE_OBJ(obj) ((::RealSix::Script::ClosureObject *)(obj))
#define TO_NATIVE_FUNCTION_OBJ(obj) ((::RealSix::Script::NativeFunctionObject *)(obj))
#define TO_REF_OBJ(obj) ((::RealSix::Script::RefObject *)(obj))
#define TO_CLASS_OBJ(obj) ((::RealSix::Script::ClassObject *)(obj))
#define TO_CLASS_INSTANCE_OBJ(obj) ((::RealSix::Script::ClassInstanceObject *)(obj))
#define TO_CLASS_CLOSURE_BIND_OBJ(obj) ((::RealSix::Script::ClassClosureBindObject *)(obj))
#define TO_ENUM_OBJ(obj) ((::RealSix::Script::EnumObject *)(obj))
#define TO_MODULE_OBJ(obj) ((::RealSix::Script::ModuleObject *)(obj))

#define IS_NULL_VALUE(v) ((v).kind == ::RealSix::Script::ValueKind::NIL)
#define IS_INT_VALUE(v) ((v).kind == ::RealSix::Script::ValueKind::INT)
#define IS_FLOAT_VALUE(v) ((v).kind == ::RealSix::Script::ValueKind::FLOAT)
#define IS_BOOL_VALUE(v) ((v).kind == ::RealSix::Script::ValueKind::BOOL)
#define IS_OBJECT_VALUE(v) ((v).kind == ::RealSix::Script::ValueKind::OBJECT)
#define IS_STR_VALUE(v) (IS_OBJECT_VALUE(v) && IS_STR_OBJ((v).object))
#define IS_ARRAY_VALUE(v) (IS_OBJECT_VALUE(v) && IS_ARRAY_OBJ((v).object))
#define IS_DICT_VALUE(v) (IS_OBJECT_VALUE(v) && IS_TABLE_OBJ((v).object))
#define IS_STRUCT_VALUE(v) (IS_OBJECT_VALUE(v) && IS_STRUCT_OBJ((v).object))
#define IS_FUNCTION_VALUE(v) (IS_OBJECT_VALUE(v) && IS_FUNCTION_OBJ((v).object))
#define IS_UPVALUE_VALUE(v) (IS_OBJECT_VALUE(v) && IS_UPVALUE_OBJ((v).object))
#define IS_CLOSURE_VALUE(v) (IS_OBJECT_VALUE(v) && IS_CLOSURE_OBJ((v).object))
#define IS_NATIVE_FUNCTION_VALUE(v) (IS_OBJECT_VALUE(v) && IS_NATIVE_FUNCTION_OBJ((v).object))
#define IS_REF_VALUE(v) (IS_OBJECT_VALUE(v) && IS_REF_OBJ((v).object))
#define IS_CLASS_VALUE(v) (IS_OBJECT_VALUE(v) && IS_CLASS_OBJ((v).object))
#define IS_CLASS_INSTANCE_VALUE(v) (IS_OBJECT_VALUE(v) && IS_CLASS_INSTANCE_OBJ((v).object))
#define IS_CLASS_CLOSURE_BIND_VALUE(v) (IS_OBJECT_VALUE(v) && IS_CLASS_CLOSURE_BIND_OBJ((v).object))
#define IS_ENUM_VALUE(v) (IS_OBJECT_VALUE(v) && IS_ENUM_OBJ((v).object))
#define IS_MODULE_VALUE(v) (IS_OBJECT_VALUE(v) && IS_MODULE_OBJ((v).object))

#define TO_INT_VALUE(v) ((v).integer)
#define TO_FLOAT_VALUE(v) ((v).floating)
#define TO_BOOL_VALUE(v) ((v).boolean)
#define TO_OBJECT_VALUE(v) ((v).object)
#define TO_STR_VALUE(v) (TO_STR_OBJ((v).object))
#define TO_ARRAY_VALUE(v) (TO_ARRAY_OBJ((v).object))
#define TO_DICT_VALUE(v) (TO_TABLE_OBJ((v).object))
#define TO_STRUCT_VALUE(v) (TO_STRUCT_OBJ((v).object))
#define TO_FUNCTION_VALUE(v) (TO_FUNCTION_OBJ((v).object))
#define TO_UPVALUE_VALUE(v) (TO_UPVALUE_OBJ((v).object))
#define TO_CLOSURE_VALUE(v) (TO_CLOSURE_OBJ((v).object))
#define TO_NATIVE_FUNCTION_VALUE(v) (TO_NATIVE_FUNCTION_OBJ((v).object))
#define TO_REF_VALUE(v) (TO_REF_OBJ((v).object))
#define TO_CLASS_VALUE(v) (TO_CLASS_OBJ((v).object))
#define TO_CLASS_INSTANCE_VALUE(v) (TO_CLASS_INSTANCE_OBJ((v).object))
#define TO_CLASS_CLOSURE_BIND_VALUE(v) (TO_CLASS_CLOSURE_BIND_OBJ((v).object))
#define TO_ENUM_VALUE(v) (TO_ENUM_OBJ((v).object))
#define TO_MODULE_VALUE(v) (TO_MODULE_OBJ((v).object))

    enum REALSIX_API ObjectKind : uint8_t
    {
        STR,
        ARRAY,
        DICT,
        STRUCT,
        FUNCTION,
        UPVALUE,
        CLOSURE,
        NATIVE_FUNCTION,
        REF,
        CLASS,
        CLASS_INSTANCE,
        CLASS_CLOSURE_BIND,
        ENUM,
        MODULE
    };

    struct REALSIX_API Object
    {
        Object(ObjectKind kind);
        virtual ~Object() = default;

        void Mark();
        void UnMark();

        virtual String ToString() const = 0;
        virtual void Blacken();
        virtual bool IsEqualTo(Object *other) = 0;
        virtual std::vector<uint8_t> Serialize() const = 0;

        const ObjectKind kind;
        bool marked{false};
        Object *next{nullptr};
    };

    struct REALSIX_API StrObject : public Object
    {
        StrObject(StringView value);
        ~StrObject() override = default;

        String ToString() const override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        String value{};
    };

    struct REALSIX_API ArrayObject : public Object
    {
        ArrayObject();
        ArrayObject(const std::vector<struct Value> &elements);
        ~ArrayObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        std::vector<struct Value> elements{};
    };

    struct REALSIX_API DictObject : public Object
    {
        DictObject();
        DictObject(const ValueUnorderedMap &elements);
        ~DictObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        ValueUnorderedMap elements{};
    };

    struct REALSIX_API StructObject : public Object
    {
        StructObject();
        StructObject(const std::unordered_map<String, Value> &elements);
        ~StructObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        std::unordered_map<String, Value> elements{};
    };

    struct REALSIX_API FunctionObject : public Object
    {
        FunctionObject();
        FunctionObject(StringView name);
        ~FunctionObject() override = default;

        String ToString() const override;
#ifndef NDEBUG
        String ToStringWithChunk() const;
#endif
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        // ++ Function cache relative
        void SetCache(size_t hash, const std::vector<Value> &result);
        bool GetCache(size_t hash, std::vector<Value> &result) const;
        std::unordered_map<size_t, std::vector<Value>> caches;
        // -- Function cache relative

        uint8_t arity{0};
        VarArg varArg{VarArg::NONE};
        int8_t upValueCount{0};
        Chunk chunk{};
        String name{};
    };

    struct REALSIX_API UpValueObject : public Object
    {
        UpValueObject();
        UpValueObject(Value *location);
        ~UpValueObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        Value *location{nullptr};
        Value closed{};
        UpValueObject *nextUpValue{nullptr};
    };

    struct REALSIX_API ClosureObject : public Object
    {
        ClosureObject();
        ClosureObject(FunctionObject *function);
        ~ClosureObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        FunctionObject *function{nullptr};
        std::vector<UpValueObject *> upvalues{};
    };

    using NativeFunction = std::function<bool(Value *, uint32_t, const Token *, Value &)>;

    struct REALSIX_API NativeFunctionObject : public Object
    {
        NativeFunctionObject();
        NativeFunctionObject(NativeFunction f);
        ~NativeFunctionObject() override = default;

        String ToString() const override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        NativeFunction fn{};
    };

    struct REALSIX_API RefObject : public Object
    {
        RefObject(Value *pointer);
        ~RefObject() override = default;

        String ToString() const override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        Value *pointer{nullptr};
    };

    struct REALSIX_API ClassObject : public Object
    {
        ClassObject();
        ClassObject(StringView name);
        ~ClassObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        bool GetDeclMember(const String &name, Value &retV);
        bool GetParentDeclMember(const String &name, Value &retV);

        String name{};
        std::map<int32_t, ClosureObject *> constructors{}; // argument count as key for now
        std::unordered_map<String, Value> defaultMembers{};
        std::unordered_map<String, Value> functions{};
        std::unordered_map<String, Value> enums{};
        std::map<String, ClassObject *> parents{};
    };

    struct REALSIX_API ClassInstanceObject : public Object
    {
        ClassInstanceObject();
        ClassInstanceObject(ClassObject *klass);
        ~ClassInstanceObject() override = default;
        
        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        bool GetMember(const String &name, Value &retV,bool parentMemberOnly = false);
        bool GetParentMember(const String &name, Value &retV);

        ClassObject *klass;
        std::unordered_map<String, Value> members{};
        std::map<String, ClassInstanceObject *> parentInstances{};
    };

    struct REALSIX_API ClassClosureBindObject : public Object
    {
        ClassClosureBindObject();
        ClassClosureBindObject(const Value &receiver, ClosureObject *cl);
        ~ClassClosureBindObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        Value receiver{};//ClassObject or ClassInstanceObject
        ClosureObject *closure{nullptr};
    };

    struct REALSIX_API EnumObject : public Object
    {
        EnumObject();
        EnumObject(const String &name, const std::unordered_map<String, Value> &pairs);
        ~EnumObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        bool GetMember(const String &name, Value &retV);

        String name{};
        std::unordered_map<String, Value> pairs{};
    };

    struct REALSIX_API ModuleObject : public Object
    {
        ModuleObject();
        ModuleObject(const String &name, const std::unordered_map<String, Value> &members={});
        ~ModuleObject() override = default;

        String ToString() const override;
        void Blacken() override;
        bool IsEqualTo(Object *other) override;
        std::vector<uint8_t> Serialize() const override;

        bool GetMember(const String &name, Value &retV);

        String name{};
        std::unordered_map<String, Value> members{};
    };
}