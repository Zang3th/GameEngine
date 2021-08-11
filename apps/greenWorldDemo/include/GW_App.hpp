#pragma once

#include "../../../engine-core/general/include/WindowManager.hpp"
#include "../../../engine-core/general/include/ResourceManager.hpp"
#include "../../../engine-core/general/include/Renderer.hpp"
#include "../../../engine-core/general/include/CoreDefines.hpp"
#include "../../../engine-core/general/include/Sprite.hpp"
#include "../../../engine-core/general/include/Profiler.hpp"
#include "../../../engine-core/general/include/Camera.hpp"
#include "../../../engine-core/general/include/Mesh.hpp"
#include "../../../engine-core/general/include/MeshCreator.hpp"
#include "../../../engine-core/general/include/Model.hpp"

#include "GW_Interface.hpp"
#include "GW_InputManager.hpp"

#include <vector>

namespace GW
{
    #define INIT_GREENWORLD_APP INITIALIZE_EASYLOGGINGPP

    class App
    {
        private:
            Core::Scope<Core::WindowManager> _windowManager;
            Core::Scope<Core::Renderer> _renderer;
            Core::Scope<Core::Camera> _camera;
            Core::Scope<Interface> _userInterface;
            std::vector<Core::Model> _models;

            static void ConfigureLogger();
            static void ConfigureProfiler();
            static void LoadResources();
            void InitModules();
            void CreateModels();

        public:
            App();
            ~App();
            bool IsRunning();
            void Update();
    };
}