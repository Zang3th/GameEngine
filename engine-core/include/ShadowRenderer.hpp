#pragma once

#include "FrameBuffer.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"

namespace Core
{
    class ShadowRenderer
    {
        private:
            glm::mat4           _lightProjection;
            Scope<FrameBuffer>  _fbo;
            Shader*             _shader;
            uint32              _shadowWidth;
            uint32              _shadowHeight;

        public:
            ShadowRenderer(Shader* shader, uint32 width, uint32 height, glm::vec3 lightPos);
            void StartFrame();
            void EndFrame();
            Shader* GetShader();
            [[nodiscard]] Texture* GetDepthTexture() const;
    };
}