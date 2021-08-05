#include "GW_Interface.hpp"

bool Core::WireframeRendering = false;

namespace GW
{
    // ----- Private -----

    void Interface::CalcOverlayPosition()
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;
        ImVec2 work_size = viewport->WorkSize;
        _overlayPos.x = work_pos.x + work_size.x - PAD;
        _overlayPos.y = work_pos.y + PAD;
        _overlayPosPivot.x = 1.0f;
        _overlayPosPivot.y = 0.0f;
    }

    // ----- Public -----

    Interface::Interface(Core::WindowManager* window, Core::Renderer* renderer, Core::Camera* camera)
    : Core::UserInterface(window), _renderer(renderer), _camera(camera), _showOverlay(true)
    {
        //Flags
        _windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
  
        //Load custom font
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../res/fonts/JetBrainsMono-Medium.ttf", 18);
    }

    void Interface::AddElements()
    {
        //Discard old plotting data every 120 frames
        if(_window->GetFrameCounter() > 120)
            ImGui::PlotVarFlushOldEntries();

        //Menu bar
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Settings"))
            {
                ImGui::MenuItem("Show overlay", "", &_showOverlay);
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Rendering"))
            {
                ImGui::MenuItem("Wireframe-Mode", "", &Core::WireframeRendering);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        //Overlay
        if(_showOverlay)
        {
            //Settings
            CalcOverlayPosition();
            ImGui::SetNextWindowBgAlpha(0.35f);
            ImGui::SetNextWindowPos(_overlayPos, ImGuiCond_Always, _overlayPosPivot);
        
            if(ImGui::Begin("Example: Simple overlay", &_showOverlay, _windowFlags))
            {
                //Application stats
                ImGui::Text("Application average %.2f ms/frame (%.1f FPS)", _window->GetDeltaTime() * 1000.0f, _window->GetFps());
                ImGui::Separator();
                ImGui::PlotVar("", (float)_window->GetDeltaTime() * 1000.0f, 0.0f, 30.0f);
                ImGui::Separator();
                ImGui::Text("Drawcalls: %d", _renderer->GetDrawcalls());
                ImGui::Text("Vertices:  %d", _renderer->GetDrawnVertices());
                ImGui::Separator();

                //Camera stats
                ImGui::Text("Camera-Position (X: %.1f, Y: %.1f, Z: %.1f)", _camera->GetPosition().x, _camera->GetPosition().y, _camera->GetPosition().z);
                ImGui::Text("Camera-Yaw: %.1f, Camera-Pitch: %.1f", _camera->GetYaw(), _camera->GetPitch());
                ImGui::Text("Camera-Front  (X: %.1f, Y: %.1f, Z: %.1f)", _camera->GetFront().x, _camera->GetFront().y, _camera->GetFront().z);
                ImGui::Separator();

                //Profiling/Timing-Results
                for(auto const& entry : Core::ProfileResults::_results)
                    ImGui::Text("%.3fms - %s", entry.second, entry.first);

                ImGui::End();
            }
        }
    }
}