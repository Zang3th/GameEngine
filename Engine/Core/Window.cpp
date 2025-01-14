#include "Window.hpp"
#include "GlobalParams.hpp"

namespace Engine
{
    // ----- Public -----

    uint32 Window::Init(const std::string& title)
    {
        //Initialize GLFW
        if(!glfwInit())
        {
            Logger::Error("Failed", "GLFW-Library", std::to_string(glfwGetError(nullptr)));
            return EXIT_FAILURE;
        }

        Logger::Info("Loaded", "GLFW-Library");

        //Set window hints
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        //Create window title
        _windowName = title;

        //Create version specific string based on the provided definitions
        #ifdef SALINITYGL_ENGINE_VERSION
            std::string versionString = SALINITYGL_ENGINE_VERSION;
        #ifdef SALINITYGL_ENGINE_VERSION_SUFFIX
            versionString += "-";
            versionString += SALINITYGL_ENGINE_VERSION_SUFFIX;
        #endif
            _windowName += " (" + versionString + ")";
        #endif

        //Create window
        _window = glfwCreateWindow(WindowParams::WIDTH, WindowParams::HEIGHT, _windowName.c_str(), nullptr, nullptr);

        if(!_window)
        {
            Logger::Error("Failed", "GLFW-Window", std::to_string(glfwGetError(nullptr)));
            return EXIT_FAILURE;
        }

        Logger::Info("Created", "GLFW-Window", _windowName);

        glfwMakeContextCurrent(_window);
        glfwSwapInterval(0);

        //Load OpenGL-Functions via glad
        if(!gladLoadGL(glfwGetProcAddress))
        {
            Logger::Error("Failed", "OpenGL-Load.");
            return EXIT_FAILURE;
        }

        //Log version
        std::string gpuString(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        std::string driverString(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        Logger::Info("Loaded", "OpenGL", std::string(gpuString + ", " + driverString));

        GLRenderSettings::SetViewport(WindowParams::WIDTH, WindowParams::HEIGHT);
        GLRenderSettings::EnableDebugging();

        //Start application
        _isRunning = true;

        return EXIT_SUCCESS;
    }

    void Window::Close()
    {
        glfwTerminate();
    }

    void Window::CalcFrametime()
    {
        //Get current time
        const double currTime_sec = glfwGetTime();

        //Calculate delta
        _dt_sec       = currTime_sec - _lastTime_sec;
        _lastTime_sec = currTime_sec;

        //Accumulate for averaging
        _frameCounter++;
        _dtAccumulated += _dt_sec;

        if(_frameCounter > 180)
        {
            _fpsAvg = 1 / (_dtAccumulated / _frameCounter);

            //Reset
            _frameCounter  = 0;
            _dtAccumulated = 0.0f;
        }

        //Check and run benchmark for 10 seconds
        if(UIParams::runBenchmark == true)
        {
            if(Benchmark::IsRunning() == false)
            {
                Benchmark::Start(_windowName, 10000);
            }

            Benchmark::AddFrame(_dt_sec * 1000);
        }
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::ProcessEvents()
    {
        if(glfwWindowShouldClose(_window))
        {
            _isRunning = false;
            Logger::Info("Closed", "GLFW-Window", _windowName);
        }

        if(glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(_window, true);
        }
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }

    bool Window::IsRunning()
    {
        return _isRunning;
    }

    double Window::GetDeltaTime_sec()
    {
        return _dt_sec;
    }

    double Window::GetDeltaTime_msec()
    {
        return _dt_sec * 1000.0f;
    }

    double Window::GetFps()
    {
        return _fpsAvg;
    }

    uint32 Window::GetFrameCounter()
    {
        return _frameCounter;
    }

    GLFWwindow* Window::GetWindow()
    {
        return _window;
    }

    float Window::GetAspectRatio()
    {
        return (float)WindowParams::WIDTH/(float)WindowParams::HEIGHT;
    }
}
