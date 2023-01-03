#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_plot_var.h"
#include "Types.hpp"
#include "Window.hpp"

namespace Engine
{
    class Interface
    {
        protected:
            static void SetDarkThemeColors();
            static void TextCentered(const char* text);

        public:
            Interface();
            virtual ~Interface();
            virtual void AddElements() = 0;
            static  void PrepareFrame();
            static  void Render();
    };
}