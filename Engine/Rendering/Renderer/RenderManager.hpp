#pragma once

#include "Types.hpp"
#include "ErrorManager.hpp"
#include "glm.hpp"
#include "Renderer.hpp"
#include "SceneRenderer.hpp"
#include "SpriteRenderer.hpp"
#include "WaterRenderer.hpp"
#include "ParticleRenderer.hpp"
#include "CellRenderer.hpp"
#include "PixelRenderer.hpp"
#include "GridRenderer.hpp"

#include <vector>

namespace Engine
{
    class RenderManager
    {
        private:
            inline static std::vector<Renderer*> _rendererStorage  = std::vector<Renderer*>();
            inline static SceneRenderer*         _sceneRenderer    = nullptr;
            inline static ShadowRenderer*        _shadowRenderer   = nullptr;
            inline static SpriteRenderer*        _spriteRenderer   = nullptr;
            inline static WaterRenderer*         _waterRenderer    = nullptr;
            inline static ParticleRenderer*      _particleRenderer = nullptr;
            inline static CellRenderer*          _cellRenderer     = nullptr;
            inline static PixelRenderer*         _pixelRenderer    = nullptr;
            inline static GridRenderer*          _gridRenderer     = nullptr;
            inline static bool                   _initialized      = false;

        public:
            RenderManager() = delete;

            static void Init2D();
            static void Init3D();
            static void CleanUp();
            static void PrepareFrame();
            static bool GetInitStatus();

            static SceneRenderer*    AddSceneRenderer();
            static ShadowRenderer*   AddShadowRenderer(uint32 resolution, const glm::mat4& orthoProj, const std::string& shader);
            static SpriteRenderer*   AddSpriteRenderer();
            static WaterRenderer*    AddWaterRenderer();
            static ParticleRenderer* AddParticleRenderer
            (
                uint32 count, float size, float speed, float gravityCompliance, float lifeLength,
                float respawnThreshold, const std::string& textureAtlas, const std::string& shader, const glm::vec3& position
            );
            static CellRenderer* AddCellRenderer(const std::string& shader, const glm::vec3& worldSpawnPos);
            static PixelRenderer* AddPixelRenderer
            (
                uint32 width, uint32 height, uint32 pxSize,
                const std::string& bgTexture, const std::string& shader
            );
            static GridRenderer* AddGridRenderer(uint32 width, uint32 height, uint32 quadSize, const std::string& shader);

            static void RenderScene();
            static void RenderShadows();
            static void RenderSprites();
            static void RenderWater();
            static void RenderParticles();
            static void RenderCells();
            static void RenderPixels();
            static void RenderGrid();
    };
}