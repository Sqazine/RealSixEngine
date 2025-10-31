#pragma once
#include "Core/Marco.h"
#include "Core/Common.h"
namespace RealSix::Script
{
    class REALSIX_API Context : public Singleton<Context>
    {
    public:
        void Init();
        void Destroy();
    };
}