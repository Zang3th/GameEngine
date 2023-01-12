#include "SpriteRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    SpriteRenderer::SpriteRenderer()
        : _orthoProj(glm::ortho(0.0f, (float)APP_SETTINGS.WINDOW_WIDTH, 0.0f, (float)APP_SETTINGS.WINDOW_HEIGHT, -1.0f, 1.0f))
    {
        Logger::Info("Created", __func__);
    }

    // ----- Public -----

    void SpriteRenderer::Flush(Renderer* renderer)
    {
        GLRenderSettings::DisableWireframe();

        //Render sprites
        for(const auto& sprite : _spriteStorage)
        {
            APP_SETTINGS.renderStats.drawnVertices += sprite->Draw(_orthoProj);
            APP_SETTINGS.renderStats.drawCalls++;
        }

        //Increase render pass counter
        APP_SETTINGS.renderStats.spritePasses++;
    }

    void SpriteRenderer::AddSprite(const glm::vec2& size, const glm::vec2& pos, Texture* texture, Shader* shader)
    {
        auto sprite = new Sprite(texture, shader, glm::vec3(1.0f));
        sprite->ChangeSize(size);
        sprite->ChangePosition(pos);
        _spriteStorage.push_back(sprite);
    }
}