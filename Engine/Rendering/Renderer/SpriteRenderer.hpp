#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "Sprite.hpp"
#include "GlobalParams.hpp"

#include <vector>

namespace Engine
{
    class SpriteRenderer final : public Renderer
    {
        friend class RenderManager;

        private:
            std::vector<Sprite*> _spriteStorage;

            SpriteRenderer();

        public:
            void Flush(Renderer* renderer) final;
            void AddSprite(const glm::vec2& size, const glm::vec2& pos, Texture* texture, Shader* shader);
    };
}