#pragma once

#include "ErrorManager.hpp"
#include "glfw3.h"
#include "CoreDefines.hpp"
#include "GLRenderSettings.hpp"

namespace Engine
{
    class Window
    {
        private:
            inline static GLFWwindow* _window        = nullptr;
            inline static std::string _windowName    = std::string();
            inline static double      _deltaTime     = double();
            inline static double      _lastFrame     = double();
            inline static double      _dtAccumulated = double();
            inline static double      _fpsAvg        = double();
            inline static uint32      _frameCounter  = uint32();
            inline static bool        _isRunning     = bool();

        public:
            Window() = delete;
            static void Init(const std::string& title);
            static void CalcFrametime();
            static void PollEvents();
            static void ProcessEvents();
            static void SwapBuffers();

            [[nodiscard]] static bool        IsRunning();
            [[nodiscard]] static double      GetDeltaTime();
            [[nodiscard]] static double      GetFps();
            [[nodiscard]] static uint32      GetFrameCounter();
            [[nodiscard]] static GLFWwindow* GetWindow();
    };
}