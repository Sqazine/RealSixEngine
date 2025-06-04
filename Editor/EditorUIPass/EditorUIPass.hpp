#pragma once

namespace RealSix
{
    class EditorApp;
    class EditorUIPass
    {
    public:
        EditorUIPass() = default;
        virtual ~EditorUIPass() = default;

        static EditorUIPass *Create(EditorApp* editorApp);

        virtual void Init() = 0;
        virtual void BeginRender() = 0;
        virtual void EndRender(bool onlyEditorPass) = 0;
    };
}