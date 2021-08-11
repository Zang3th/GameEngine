#include "Renderer.hpp"

namespace Core
{
    // ----- Public -----

    Renderer::Renderer(Camera* camera)
    : _camera(camera), _drawcalls(0), _drawnVertices(0), _orthoProjection(glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f)), _perspProjection(glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 1000.0f))
    {

    }

    void Renderer::Prepare()
    {
        //Reset render stats for current frame
        _drawcalls = 0;
        _drawnVertices = 0;

        //Clear buffers
        _spriteBuffer.clear();
        _modelBuffer.clear();
    }

    void Renderer::Submit(const Sprite* sprite)
    {
        _spriteBuffer.push_back(sprite);
    }

    void Renderer::Submit(const Model* model)
    {
        _modelBuffer.push_back(model);
    }

    void Renderer::Flush()
    {
        //Check for Wireframe-Mode
        if(WireframeRendering){
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));}
        else{
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));}

        //Render models
        for(const auto& model : _modelBuffer)
        {
            _drawnVertices += model->Draw(_perspProjection, _camera->GetViewMatrix(), _camera->GetPosition());
            _drawcalls++;
        }

        //Render sprites
        for(const auto& sprite : _spriteBuffer)
        {
            _drawnVertices += sprite->Draw(_orthoProjection);
            _drawcalls++;
        }
    }

    unsigned int Renderer::GetDrawcalls() const
    {
        return _drawcalls;
    }

    unsigned int Renderer::GetDrawnVertices() const
    {
        return _drawnVertices;
    }
}