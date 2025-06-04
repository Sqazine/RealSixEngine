#include "TestApp.hpp"
namespace RealSix
{
    void MainEntry(int argc, char *argv[])
    {
        RealSix::TestApp::GetInstance().Run();
    }
}