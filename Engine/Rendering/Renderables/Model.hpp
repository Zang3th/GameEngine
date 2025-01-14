#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "GLTexture.hpp"
#include "Shader.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Types.hpp"
#include "ErrorManager.hpp"
#include "Mesh.hpp"

#include <vector>

namespace Engine
{
    class Model
    {
        private:
            Scope<VertexArray>      _vao;
            Scope<VertexBuffer>     _vboVert, _vboTex, _vboNorm, _vboTang;
            Scope<IndexBuffer>      _ibo;
            glm::mat4               _model;
            glm::vec3               _position;
            std::vector<GLTexture*> _glTextures;
            uint32                  _verticeCount;
            int32                   _gotDiffuseTex, _gotNormalMap;
            float                   _rotationX, _rotationY, _rotationZ;
            float                   _size;

            void                  InitGpuStorage(const Mesh* mesh);
            void                  SetModelMatrix();

        public:
            explicit                                     Model(const Mesh* mesh);
            [[nodiscard]] glm::mat4                      GetModelMatrix()  const;
            [[nodiscard]] uint32                         GetVerticeCount() const;
            [[nodiscard]] int32                          GotDiffuseTex()   const;
            [[nodiscard]] int32                          GotNormalMap()    const;
            [[nodiscard]] const std::vector<GLTexture*>* GetGLTextures()     const;

            void BindBuffers() const;
            void UnbindBuffers() const;
            void ChangePosition(const glm::vec3& position);
            void ChangeRotation(const glm::vec3& rotation);
            void ChangeSize(float size);
            void AddGLTexture(GLTexture* glTexture);
            void AddGLTextureToSlot(GLTexture* glTexture, uint32_t slot);
            void SetDiffuseTexture();
    };
}
