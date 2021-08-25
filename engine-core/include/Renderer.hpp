#pragma once

#include "Camera.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Model.hpp"
#include "CoreDefines.hpp"
#include "ErrorManager.hpp"
#include "Cubemap.hpp"

#include <vector>

namespace Core
{
    class Renderer
    {
        private:
            std::vector<const Sprite*>  _spriteBuffer;
            std::vector<const Model*>   _modelBuffer;
            glm::mat4                   _orthoProjection;
            glm::mat4                   _perspProjection;
            Camera*                     _camera;
            const Cubemap*              _cubemap;
            uint32                      _drawcalls;
            uint32                      _drawnVertices;

        public:
            explicit Renderer(Camera* camera);
            void PrepareFrame();
            void Submit(const Sprite* sprite);
            void Submit(const Model* model);
            void Submit(const Cubemap* cubemap);
            void Flush();
            [[nodiscard]] uint32 GetDrawcalls() const;
            [[nodiscard]] uint32 GetDrawnVertices() const;
    };
}