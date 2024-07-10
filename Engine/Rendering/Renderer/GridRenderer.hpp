#pragma once

#include "Renderer.hpp"
#include "glm.hpp"
#include "Sprite.hpp"
#include "Shader.hpp"
#include "GLRenderSettings.hpp"
#include "GlobalParams.hpp"
#include "ResourceManager.hpp"

namespace Engine
{
    class GridRenderer final : public Renderer
    {
        private:
            uint32    _gridWidth, _gridHeight, _quadSize, _quadAmountTotal;
            glm::vec3 _defaultColor;
            Shader*   _shader;

            Scope<VertexArray>  _vao;
            Scope<VertexBuffer> _vboVert, _vboColor;
            glm::mat4           _orthoProj, _model;

            std::vector<glm::vec3> _colorStorage;

            void InitGpuStorage();

        public:
            GridRenderer(uint32 width, uint32 height, uint32 quadSize, const std::string& shader);

            void Flush(Renderer* renderer) override;
            void UpdateGpuStorage() const;
            void Set(uint32 x, uint32 y, const glm::vec3& color);
            void Reset(uint32 x, uint32 y);
            void AddTextureSubsampled(const std::string& texture, const glm::uvec2& pos, uint32 size);
    };
}  // namespace Engine
