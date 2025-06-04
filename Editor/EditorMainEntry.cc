#include "EditorApp.hpp"

namespace RealSix
{
    class EditorAppInstance : public EditorApp, public Singleton<EditorAppInstance>
    {
    };
    void MainEntry(int argc, char *argv[])
    {
        EditorAppInstance::GetInstance().Run();
    }
}