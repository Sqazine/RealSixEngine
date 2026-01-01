#pragma once
#include <vector>
#include "Object.hpp"
#include "Value.hpp"
#include "Common.hpp"
#include "Logger.hpp"
#include "Config/Config.hpp"

namespace RealSix::Script
{
    struct CallFrame
    {
        CallFrame() = default;
        CallFrame(ClosureObject *closure, Value *slots) : closure(closure), slots(slots), ip(closure->function->chunk.opCodes.data()), argumentsHash(0) {}
        ~CallFrame() = default;

        ClosureObject *closure = nullptr;
        uint8_t *ip = nullptr;
        Value *slots = nullptr;

        // ++ Function cache relative
        size_t argumentsHash;
        // -- Function cache relative
    };

    class REALSIX_API Allocator : public Singleton<Allocator>
    {
    public:
        void Init();
        void Destroy();

        void ResetStackPointer();
        void ResetCallFramePointer();

        template <class T, typename... Args>
        T *CreateObject(Args &&...params)
        {
            T *object = new T(std::forward<Args>(params)...);
            size_t objBytes = sizeof(*object);
            mBytesAllocated += objBytes;
#ifndef NDEBUG
            if (ScriptConfig::GetInstance().IsStressGC())
                GC();
#endif
            if (mBytesAllocated > mNextGCByteSize)
                GC();

            object->next = mObjectChain;
            object->marked = false;
            mObjectChain = object;
#ifndef NDEBUG
            if (ScriptConfig::GetInstance().IsDebugGC())
                REALSIX_LOG_INFO("{} has been add to gc record chain {} for {}", (void *)object, objBytes, object->kind);
#endif

            return object;
        }

        void PushStack(const Value &value);
        Value PopStack();
        Value PeekStack(int32_t distance);

        void SetStackTop(Value *top);
        Value *StackTop() const;
        Value *Stack();
        void MoveStackTop(int32_t offset);

        void PushCallFrame(const CallFrame &callFrame);
        CallFrame *PopCallFrame();
        CallFrame *PeekCallFrame(int32_t distance);

        bool IsCallFrameStackEmpty();
        size_t CallFrameCount();

        UpValueObject *CaptureUpValue(Value *location);
        void ClosedUpValues(Value *end);

        void SetValueFromStackTopOffset(int32_t offset, const Value &value);

        Value *GetGlobalVariable(size_t idx);
        void SetGlobalVariable(size_t idx, const Value &v);

        Value *GetStaticVariable(size_t idx);
		bool& IsStaticVariableInitialized(size_t idx) { return mStaticVariableInitializedList[idx]; }
    private:
        friend class VM;
        friend class Compiler;
        friend class LibraryManager;
        void StopGC();
        void RecoverGC();
        bool m_IsStopGC{false};

    private:
        template <class T>
        void FreeObject(T *object)
        {
#ifndef NDEBUG
            if (ScriptConfig::GetInstance().IsDebugGC())
                REALSIX_LOG_INFO("delete object(0x{})", (void *)object);
#endif
            mBytesAllocated -= sizeof(object);
            SAFE_DELETE(object);
        }
        void FreeObjects();
        void GC();

        void MarkRootObjects();
        void MarkGrayObjects();
        void Sweep();

        Value mGlobalVariableList[VARIABLE_MAX];

        Value mStaticVariableList[VARIABLE_MAX];
		bool mStaticVariableInitializedList[VARIABLE_MAX];

        Value *mStackTop;
        Value mValueStack[STACK_MAX];

        CallFrame *mCallFrameTop;
        CallFrame mCallFrameStack[STACK_MAX];

        UpValueObject *mOpenUpValues;

        friend struct Object;

        Object *mObjectChain;
        std::vector<Object *> mGrayObjects;
        size_t mBytesAllocated;
        size_t mNextGCByteSize;
    };

#define GET_GLOBAL_VARIABLE(idx) (Allocator::GetInstance().GetGlobalVariable(idx))

#define GET_STATIC_VARIABLE(idx) (Allocator::GetInstance().GetStaticVariable(idx))
#define GET_STATIC_VARIABLE_INITIALIZED(idx) (Allocator::GetInstance().IsStaticVariableInitialized(idx))

#define PUSH_STACK(v) (Allocator::GetInstance().PushStack(v))
#define POP_STACK() (Allocator::GetInstance().PopStack())
#define PEEK_STACK(dist) (Allocator::GetInstance().PeekStack(dist))
#define STACK_TOP() (Allocator::GetInstance().StackTop())
#define STACK() (Allocator::GetInstance().Stack())
#define SET_STACK_TOP(v) (Allocator::GetInstance().SetStackTop(v))
#define MOVE_STACK_TOP(idx) (Allocator::GetInstance().MoveStackTop(idx))
#define SET_VALUE_FROM_STACK_TOP_OFFSET(offset, v) (Allocator::GetInstance().SetValueFromStackTopOffset(offset, v))

#define PUSH_CALL_FRAME(v) (Allocator::GetInstance().PushCallFrame(v))
#define POP_CALL_FRAME() (Allocator::GetInstance().PopCallFrame())
#define PEEK_CALL_FRAME(dist) (Allocator::GetInstance().PeekCallFrame(dist))
#define IS_CALL_FRAME_STACK_EMPTY() (Allocator::GetInstance().IsCallFrameStackEmpty())
#define CALL_FRAME_COUNT() (Allocator::GetInstance().CallFrameCount())

#define CAPTURE_UPVALUE(location) (Allocator::GetInstance().CaptureUpValue(location))
#define CLOSED_UPVALUES(end) (Allocator::GetInstance().ClosedUpValues(end))
}