#pragma once
#include "Core/Marco.hpp"
#include "Core/Common.hpp"
namespace RealSix::Script
{
    class REALSIX_API Context : public Singleton<Context>
    {
    public:
        void Init();
        void CleanUp();
    };
}