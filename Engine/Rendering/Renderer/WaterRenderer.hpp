#pragma once

#include "FrameBuffer.hpp"
#include "Renderer.hpp"
#include "Camera3D.hpp"
#include "GLRenderSettings.hpp"
#include "SceneRenderer.hpp"

namespace Engine
{
    class WaterRenderer final : public Renderer
    {
        private:
            Scope<FrameBuffer>  _reflectFBO, _refractFBO;
            const uint32        _reflectionWidth  = 1024;
            const uint32        _reflectionHeight = 1024;
            const uint32        _refractionWidth  = 1024;
            const uint32        _refractionHeight = 1024;
            const float         _waterHeight      = 0.5f;
            const glm::vec4     _reflectionClipPlane = glm::vec4(0.0f, 1.0f, 0.0f, -_waterHeight);
            const glm::vec4     _refractionClipPlane = glm::vec4(0.0f, -1.0f, 0.0f, _waterHeight);

            void InitReflectionFBO();
            void InitRefractionFBO();
            void StartReflectionFrame();
            void EndReflectionFrame();
            void StartRefractionFrame();
            void EndRefractionFrame();
            void RenderReflectionFrame(SceneRenderer* sceneRenderer);
            void RenderRefractionFrame(SceneRenderer* sceneRenderer);

        public:
            WaterRenderer();

            void Flush(Renderer* sceneRenderer) override;

            [[nodiscard]] GLTexture* GetReflectTexture()      const;
            [[nodiscard]] GLTexture* GetRefractTexture()      const;
            [[nodiscard]] GLTexture* GetRefractDepthTexture() const;
    };
}
