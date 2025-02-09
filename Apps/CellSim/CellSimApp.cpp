#include "CellSimApp.hpp"

namespace CS
{
    // ----- Private -----

    void CellSimApp::LoadResources()
    {
        //Shader
        Engine::ResourceManager::LoadShader("ShadowCreateShader", "../Res/Shader/GreenWorld/ShadowCreate_VS.glsl", "../Res/Shader/GreenWorld/ShadowCreate_FS.glsl");
        Engine::ResourceManager::LoadShader("ModelShader", "../Res/Shader/GreenWorld/Model_VS.glsl", "../Res/Shader/GreenWorld/Model_FS.glsl");
        Engine::ResourceManager::LoadShader("SpriteShaderGreyscale", "../Res/Shader/GreenWorld/Sprite_VS.glsl", "../Res/Shader/GreenWorld/SpriteGreyscale_FS.glsl");
        Engine::ResourceManager::LoadShader("CellShader", "../Res/Shader/CellSim/Cell_VS.glsl", "../Res/Shader/CellSim/Cell_FS.glsl");
    }

    bool CellSimApp::Init()
    {
        //Configure some global application parameters
        Engine::RenderParams::farPlane      = 1024.0f;
        Engine::RenderParams::planeSize     = 1;
        Engine::CameraParams::movementSpeed = 75.0f;
        Engine::CameraParams::startPitch    = -33.0f;
        Engine::CameraParams::startPos      = glm::vec3(-115.0f, 105.0f, 5.0f);
        Engine::LightParams::position       = glm::vec3(250.0f, 250.0f, -250.0f);
        Engine::LightParams::target         = glm::vec3(0.0f, 0.0f, 0.0f);

        //Initialize engine components
        Engine::Logger::Init();
        if(Engine::Window::Init("CellSim") != EXIT_SUCCESS)
        {
            return false;
        }
        Engine::Camera3D::Init();
        Engine::CameraController3D::Init();
        Engine::RenderManager::Init3D();

        //Load shaders and textures
        LoadResources();

        //Create application specific renderers
        _sceneRenderer = new Engine::SceneRenderer();
        _sceneRenderer->SetModelShader("ModelShader");
        Engine::RenderManager::Submit(_sceneRenderer);

        _cellRenderer = new Engine::CellRenderer("CellShader", glm::vec3(-30.0f, 0.5f, -30.0f));
        Engine::RenderManager::Submit(_cellRenderer);

        //Create cell manager and add cell renderer
        _cellManager = Engine::MakeScope<Engine::CellManager>(_cellRenderer);

        //Create UI
        _interface = Engine::MakeScope<CellSimInterface>();
        _interface->Init();

        //Create timer
        _physicsTimer = Engine::MakeScope<Engine::Timer>(10); //10ms

        AddObjects();
        AddCellWorld();

        return true;
    }

    void CellSimApp::AddObjects()
    {
        //Ground plane
        _sceneRenderer->AddPlane
        (
            Engine::RenderParams::planeSize,
            Engine::RenderParams::planeSize,
            64.0f,
            glm::vec3(-32.0f, 0.0f, -32.0f),
            nullptr,
            {}
        );

        //Frame
        _sceneRenderer->AddObject
        (
            1.0f,
            glm::vec3(0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            nullptr,
            "Frame",
            "../Res/Assets/Models/CellSim/Frame"
        );
    }

    void CellSimApp::AddCellWorld()
    {
        _cellManager->CreateCellWorld();
    }

    void CellSimApp::HandleCellSpawn()
    {
        Engine::CellParams cellParams
        {
            {
                Engine::CellTypeSpreadFactor[Engine::CellSimParams::selectedCellType],
                Engine::CellSimParams::selectedCellType
            },
            glm::u32vec3(Engine::CellSimParams::selectedCellCoords[0],
                         Engine::CellSimParams::selectedCellCoords[1],
                         Engine::CellSimParams::selectedCellCoords[2])
        };

        if(Engine::CellSimParams::selectedCellAmount == 1)
        {
            _cellManager->AddCell(cellParams);
        }
        else if(Engine::CellSimParams::createSpawner)
        {
            _cellManager->AddCellSpawner(cellParams);
        }
    }

    void CellSimApp::HandleCellKill()
    {
        _cellManager->DeleteCell
        (
            glm::u32vec3(Engine::CellSimParams::selectedCellCoords[0],
                         Engine::CellSimParams::selectedCellCoords[1],
                         Engine::CellSimParams::selectedCellCoords[2])
        );
    }

    // ----- Public -----

    CellSimApp::CellSimApp() = default;

    CellSimApp::~CellSimApp()
    {
        Engine::ResourceManager::CleanUp();
        Engine::RenderManager::CleanUp();
        Engine::Window::Close();
    }

    void CellSimApp::Update()
    {
        {
            Engine::PROFILE_SCOPE("Process events");

            Engine::Window::PollEvents();
            Engine::Window::ProcessEvents();
            Engine::CameraController3D::ProcessInput();
            _physicsTimer->Update(Engine::Window::GetDeltaTime_msec());

            if(Engine::UIParams::resetCamera)
            {
                Engine::Camera3D::ResetPosition();
                Engine::UIParams::resetCamera = false;
            }
        }

        {
            Engine::PROFILE_SCOPE("Manage cells");

            //Check for cell spawn
            if(Engine::CellSimParams::spawnCell)
            {
                HandleCellSpawn();
                Engine::CellSimParams::spawnCell = false;
            }

            //Check for cell kill
            if(Engine::CellSimParams::killCell)
            {
                HandleCellKill();
                Engine::CellSimParams::killCell = false;
            }

            //Check for cell delete
            if(Engine::CellSimParams::deleteCells)
            {
                _cellManager->DeleteCells();
                Engine::CellSimParams::deleteCells = false;
            }

            //Check for cell spawner delete
            if(Engine::CellSimParams::deleteSpawners)
            {
                _cellManager->DeleteSpawners();
                Engine::CellSimParams::deleteSpawners = false;
            }

            //Check for debug print
            if(Engine::CellSimParams::printDebug)
            {
                _cellManager->PrintDebug();
                Engine::CellSimParams::printDebug = false;
            }

            //Check if physics should be calculated this turn
            _calcPhysicsThisTurn = true;
            if(Engine::CellSimParams::enableSingleStepping)
            {
                _calcPhysicsThisTurn = false;

                if(Engine::CellSimParams::performSingleStep)
                {
                    _calcPhysicsThisTurn = true;
                    Engine::CellSimParams::performSingleStep = false;
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Calculate physics");

            if(_calcPhysicsThisTurn)
            {
                //Check if 10ms have elapsed
                if(_physicsTimer->CheckElapsedAndReset())
                {
                    //Increase tick counter
                    _tickCounter++;

                    //Do physics
                    if(Engine::CellSimParams::cellsAlive > 0)
                    {
                        _cellManager->CalculateCellPhysics();
                    }

                    //Add new cells for every existing spawner
                    if(_tickCounter > 5)
                    {
                        _cellManager->ResolveCellSpawners();
                        _tickCounter = 0;
                    }
                }
            }
        }

        {
            Engine::PROFILE_SCOPE("Prepare frame");

            Engine::Window::CalcFrametime();
            Engine::RenderManager::PrepareFrame();
            _interface->PrepareFrame();
        }

        {
            Engine::PROFILE_SCOPE("Render scene");

            _sceneRenderer->Flush(nullptr);
        }

        {
            Engine::PROFILE_SCOPE("Render cells");

            _cellRenderer->Flush((Engine::Renderer*)_sceneRenderer);
        }

        {
            Engine::PROFILE_SCOPE("Render UI");

            _interface->AddElements();
            _interface->Render();
        }

        {
            Engine::PROFILE_SCOPE("End frame");

            Engine::Window::SwapBuffers();
        }
    }
}
