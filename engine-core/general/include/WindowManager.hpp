#pragma once

#include "ErrorManager.hpp"
#include <GLFW/glfw3.h>
#include "CoreDefines.hpp"
#include <string>

namespace Core
{
    class WindowManager
    {
        private:
            std::string _windowName;
            GLFWwindow* _window;
            bool _isRunning;
            float _deltaTime;
            float _lastFrame;
            unsigned int _frameCounter;
            float _dtAccumulated;
            float _fpsAvg;

            void CreateWindow();
            void PollEvents();
            void ProcessEvents();
            void Prepare();
            void CalcFrametime();    

        public:
            WindowManager();
            ~WindowManager();
            void SetWindowTitle(const std::string& title);
            bool WindowIsRunning();
            void PrepareFrame();   
            void SwapBuffers();
            float GetDeltaTime();
            float GetFps();
            unsigned int GetFrameCounter();
            GLFWwindow* GetWindow();
    };
}