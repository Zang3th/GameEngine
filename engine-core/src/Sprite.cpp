#include "Sprite.hpp"

namespace Core
{
    // ----- Private -----   

    Ref<VertexArray> Sprite::CreateSpriteVao()
    {
        //Create and bind vao
        Ref<VertexArray> vao = MakeRef<VertexArray>();
        vao->Bind();

        //Create data
        float vertices[] =
        {
            //Position  //Texture
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        //Set vertice count accordingly
        _verticeCount = 6;

        //Create vbo, send it data and configure vao
        VertexBuffer vbo(vertices, sizeof(vertices));
        vao->DefineAttributes(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        //Unbind vao
        vao->Unbind();

        return vao;
    }
    
    glm::mat4 Sprite::CreateModelMatrix(glm::vec2 position, glm::vec2 size, float rotation)
    {
        //Create model matrix and apply transformations
        glm::mat4 model = glm::mat4(1.0f);

        //Translate
        model = glm::translate(model, glm::vec3(position, 0.0f));

        //Rotate
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

        //Scale
        model = glm::scale(model, glm::vec3(size, 1.0f));

        return model;
    }

    // ----- Public -----

    Sprite::Sprite(Texture* texture, Shader* shader, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
        :   _color(color),
            _texture(texture),
            _shader(shader),
            _position(position),
            _size(size),
            _rotation(rotation),
            _verticeCount(0)
    {
        _vao = CreateSpriteVao();
        _model = CreateModelMatrix(_position, _size, _rotation);
    }

    uint32 Sprite::Draw(const glm::mat4& projection) const
    {
        _shader->Bind();
        
        //Set uniforms
        _shader->SetUniformVec3f("color", _color);
        _shader->SetUniformMat4f("model", _model);
        _shader->SetUniformMat4f("projection", projection);

        _texture->Bind();
        _vao->Bind();

        //Render quad
        GLCall(glDrawArrays(GL_TRIANGLES, 0, _verticeCount));

        _vao->Unbind();
        _texture->Unbind();
        _shader->Unbind();

        //Return rendered vertices
        return _verticeCount;
    }

    void Sprite::Translate(const glm::vec2& position)
    {
        glm::vec2 newPosition = glm::vec2(_position.x + position.x, _position.y + position.y);
        _model = CreateModelMatrix(newPosition, _size, _rotation);
        _position = newPosition;
    }

    void Sprite::SetTexture(Texture* texture)
    {
        _texture = texture;
    }
}