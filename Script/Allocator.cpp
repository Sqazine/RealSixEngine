#include "Allocator.hpp"
#include "VM.hpp"
#include "Core/Logger.hpp"

namespace RealSix::Script
{
    void Allocator::Init()
    {
        if (mObjectChain)
            FreeObjects();

        mBytesAllocated = 0;
        mNextGCByteSize = 256;
        mObjectChain = nullptr;

        mCallFrameTop = mCallFrameStack;
        mStackTop = mValueStack;

        mOpenUpValues = nullptr;

        memset(mGlobalValueList, 0, sizeof(Value) * VARIABLE_MAX);
        memset(mStaticValueList, 0, sizeof(Value) * VARIABLE_MAX);
        memset(mStaticValueInitializedList, 0, sizeof(bool) * VARIABLE_MAX);
    }

    void Allocator::Destroy()
    {
        FreeObjects();
    }

    void Allocator::ResetStackPointer()
    {
        mStackTop = mValueStack;
    }
    void Allocator::ResetCallFramePointer()
    {
        mCallFrameTop = mCallFrameStack;
    }

    void Allocator::FreeObjects()
    {
        auto bytes = mBytesAllocated;
        Object *object = mObjectChain;
        while (object != nullptr)
        {
            Object *next = object->next;
            size_t objBytes = sizeof(*object);
            FreeObject(object);
            object = next;
        }

#ifndef NDEBUG
        if (ScriptConfig::GetInstance().IsDebugGC())
            REALSIX_LOG_INFO("collected {} bytes (from {} to {}) next gc bytes {}", bytes - mBytesAllocated, bytes, mNextGCByteSize);
#endif
    }

    void Allocator::PushStack(const Value &value)
    {
#ifndef NDEBUG
        if (mStackTop - mValueStack >= STACK_MAX)
            REALSIX_LOG_ERROR("Stack overflow.");
#endif
        *(mStackTop++) = value;
    }
    Value Allocator::PopStack()
    {
#ifndef NDEBUG
        if (mStackTop - mValueStack <= 0)
            REALSIX_LOG_ERROR("Stack underflow.");
#endif
        return *(--mStackTop);
    }
    Value Allocator::PeekStack(int32_t distance)
    {
        return *(mStackTop - distance - 1);
    }

    void Allocator::PushCallFrame(const CallFrame &callFrame)
    {
        *(mCallFrameTop++) = callFrame;
    }

    CallFrame *Allocator::PopCallFrame()
    {
        return --mCallFrameTop;
    }

    CallFrame *Allocator::PeekCallFrame(int32_t distance)
    {
        return mCallFrameTop - distance - 1;
    }

    bool Allocator::IsCallFrameStackEmpty()
    {
        return mCallFrameTop == mCallFrameStack;
    }

    size_t Allocator::CallFrameCount()
    {
        return mCallFrameTop - mCallFrameStack;
    }

    UpValueObject *Allocator::CaptureUpValue(Value *location)
    {
        UpValueObject *prevUpValue = nullptr;
        UpValueObject *upValue = mOpenUpValues;

        while (upValue != nullptr && upValue->location > location)
        {
            prevUpValue = upValue;
            upValue = upValue->nextUpValue;
        }

        if (upValue != nullptr && upValue->location == location)
            return upValue;

        auto createdUpValue = CreateObject<UpValueObject>(location);
        createdUpValue->nextUpValue = upValue;

        if (prevUpValue == nullptr)
            mOpenUpValues = createdUpValue;
        else
            prevUpValue->nextUpValue = createdUpValue;

        return createdUpValue;
    }
    void Allocator::ClosedUpValues(Value *end)
    {
        while (mOpenUpValues != nullptr && mOpenUpValues->location >= end)
        {
            UpValueObject *upvalue = mOpenUpValues;
            upvalue->closed = *upvalue->location;
            upvalue->location = &upvalue->closed;
            mOpenUpValues = upvalue->nextUpValue;
        }
    }

    void Allocator::SetStackTop(Value *top)
    {
        mStackTop = top;
    }

    Value *Allocator::StackTop() const
    {
        return mStackTop;
    }

    Value *Allocator::Stack()
    {
        return mValueStack;
    }

    void Allocator::MoveStackTop(int32_t offset)
    {
        mStackTop += offset;
    }

    void Allocator::SetValueFromStackTopOffset(int32_t offset, const Value &value)
    {
        mStackTop[offset] = value;
    }

    Value *Allocator::GetGlobalValueReference(size_t idx)
    {
        return &mGlobalValueList[idx];
    }

    void Allocator::SetGlobalValue(size_t idx, const Value &v)
    {
        mGlobalValueList[idx] = v;
    }

    Value *Allocator::GetStaticValueReference(size_t idx)
    {
        return &mStaticValueList[idx];
    }

    void Allocator::StopGC()
    {
        m_IsStopGC = true;
    }

    void Allocator::RecoverGC()
    {
        m_IsStopGC = false;
    }

    void Allocator::GC()
    {
#ifndef NDEBUG
        size_t bytes = 0;
        if (ScriptConfig::GetInstance().IsDebugGC())
        {
            REALSIX_LOG_INFO("begin gc");
            bytes = mBytesAllocated;
        }
#endif

        MarkRootObjects();
        MarkGrayObjects();
        Sweep();
        mNextGCByteSize = mBytesAllocated * GC_HEAP_GROW_FACTOR;

#ifndef NDEBUG
        if (ScriptConfig::GetInstance().IsDebugGC())
        {
            REALSIX_LOG_INFO("end gc");
            REALSIX_LOG_INFO("    collected {} bytes (from {} to {}) next gc bytes {}", bytes - mBytesAllocated, bytes, mNextGCByteSize);
        }
#endif
    }

    void Allocator::MarkRootObjects()
    {
        for (Value *slot = mValueStack; slot < mStackTop; ++slot)
            slot->Mark();
        for (CallFrame *slot = mCallFrameStack; slot < mCallFrameTop; ++slot)
            slot->closure->Mark();
        for (UpValueObject *upvalue = mOpenUpValues; upvalue != nullptr; upvalue = upvalue->nextUpValue)
            upvalue->Mark();

        for (int32_t i = 0; i < VARIABLE_MAX; ++i)
            if (mGlobalValueList[i] != Value())
                mGlobalValueList[i].Mark();
    }

    void Allocator::MarkGrayObjects()
    {
        while (mGrayObjects.size() > 0)
        {
            auto object = mGrayObjects.back();
            mGrayObjects.pop_back();
            object->Blacken();
        }
    }

    void Allocator::Sweep()
    {
        Object *previous = nullptr;
        Object *object = mObjectChain;
        while (object)
        {
            if (object->marked)
            {
                object->UnMark();
                previous = object;
                object = object->next;
            }
            else
            {
                Object *unreached = object;
                object = object->next;
                if (previous != nullptr)
                    previous->next = object;
                else
                    mObjectChain = object;

                FreeObject(unreached);
            }
        }
    }
}