#include "ParticleRenderer.hpp"

namespace Engine
{
    // ----- Private -----

    void ParticleRenderer::InitGpuStorage()
    {
        //Create vertice data
        static constexpr float vertices[] =
        {
            -0.5f,  0.5f,
            -0.5f, -0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f
        };

        //Create and bind vao
        _vao = MakeScope<VertexArray>();
        _vao->Bind();

        //Create vbo's, send it data and configure vao
        _vboVert = MakeScope<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
        _vao->DefineAttributes(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        _vboModel = MakeScope<VertexBuffer>(&_modelViewStorage[0], _modelViewStorage.size() * 16 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), nullptr);
        _vao->DefineAttributes(2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
        _vao->DefineAttributes(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
        _vao->DefineAttributes(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

        _vboTex = MakeScope<VertexBuffer>(&_texOffsetStorage[0], _texOffsetStorage.size() * 4 * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

        _vboBlend = MakeScope<VertexBuffer>(&_blendFactorStorage[0], _blendFactorStorage.size() * sizeof(float), GL_DYNAMIC_DRAW);
        _vao->DefineAttributes(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

        //Set attribute divisors for per instance data
        _vao->AttributeDivisor(1, 1);
        _vao->AttributeDivisor(2, 1);
        _vao->AttributeDivisor(3, 1);
        _vao->AttributeDivisor(4, 1);
        _vao->AttributeDivisor(5, 1);
        _vao->AttributeDivisor(6, 1);

        //Unbind everything
        _vao->Unbind();
        _vboVert->Unbind();
        _vboModel->Unbind();
        _vboTex->Unbind();
        _vboBlend->Unbind();
    }

    void ParticleRenderer::UpdateGpuStorage()
    {
        _vboModel->Bind();
        _vboModel->Update(&_modelViewStorage[0], _modelViewStorage.size() * 16 * sizeof(float));
        _vboModel->Unbind();

        _vboTex->Bind();
        _vboTex->Update(&_texOffsetStorage[0], _texOffsetStorage.size() * 4 * sizeof(float));
        _vboTex->Unbind();

        _vboBlend->Bind();
        _vboBlend->Update(&_blendFactorStorage[0], _blendFactorStorage.size() * sizeof(float));
        _vboBlend->Unbind();
    }

    void ParticleRenderer::GenerateParticles()
    {
        //Iterate over particle count
        for(uint32 i = 0; i < _count; i++)
        {
            //Create and randomize the parameters a bit to deviate the particles
            glm::vec3 pos = _position;
            glm::vec3 vel = glm::vec3( 2.0f * ((Random::GetFloat() - 0.5f) * 2.0f),
                                      25.0f * ((Random::GetFloat())        * 1.5f),
                                       2.0f * ((Random::GetFloat() - 0.5f) * 2.0f)) * _speed;
            float grav = _gravityCompliance;
            float life = _lifeLength + ((Random::GetFloat() - 0.5f) * 4.0f);
            float rot  = 0.0f;
            float size = _size * Random::GetFloat();

            //Create particle
            auto *particle = new Particle(pos, vel, grav, life, rot, size, _numberOfRows);

            //Save the different components
            _particleStorage.push_back(particle);
            _modelViewStorage.push_back(GetModelViewMatrix(particle));
            _texOffsetStorage.emplace_back(0.0f);
            _blendFactorStorage.push_back(0.0f);
        }
    }

    glm::mat4 ParticleRenderer::GetModelViewMatrix(const Particle* particle)
    {
        //Create model matrix
        glm::mat4 model(1.0f);

        //Get view matrix
        glm::mat4 view = Camera3D::GetViewMatrix();

        //Set upper 3x3 part of the model matrix to the transpose of the view matrix to negate any rotation. Then apply transformations
        model = glm::translate(model, particle->GetPosition());
        model[0][0] = view[0][0];
        model[0][1] = view[1][0];
        model[0][2] = view[2][0];
        model[1][0] = view[0][1];
        model[1][1] = view[1][1];
        model[1][2] = view[2][1];
        model[2][0] = view[0][2];
        model[2][1] = view[1][2];
        model[2][2] = view[2][2];
        model = glm::rotate(model, glm::radians(particle->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(particle->GetSize()));

        return view * model;
    }

    // ----- Public -----

    ParticleRenderer::ParticleRenderer
    (
        uint32 count, float size, float speed, float gravityCompliance, float lifeLength,
        float respawnThreshold, const std::string& glTextureAtlas, const std::string& shader, const glm::vec3& position
    )
        :   _glTextureAtlas(ResourceManager::GetGLTexture(glTextureAtlas)), _shader(ResourceManager::GetShader(shader)),
            _count(count), _numberOfRows(_glTextureAtlas->GetNumberOfRows()), _verticeCount(4), _size(size),
            _speed(speed), _gravityCompliance(gravityCompliance), _lifeLength(lifeLength),
            _respawnTreshold(respawnThreshold), _position(position)
    {
        Logger::Info("Created", "Renderer", __func__);
        GenerateParticles();
        InitGpuStorage();
    }

    ParticleRenderer::~ParticleRenderer()
    {
        for(auto const& p : _particleStorage)
        {
            delete p;
        }
    }

    void ParticleRenderer::Flush(Renderer* sceneRenderer)
    {
        GLRenderSettings::DisableDepthtest();

        //Bind shader
        _shader->Bind();

        //Bind texture
        _glTextureAtlas->Bind();

        //Bind vao and vbo's
        _vao->Bind();
        _vboVert->Bind();
        _vboModel->Bind();
        _vboTex->Bind();
        _vboBlend->Bind();

        //Set uniforms
        _shader->SetUniformMat4f("projection", ((SceneRenderer*)sceneRenderer)->GetProjMatrix());
        _shader->SetUniform1f("numberOfRows", (float)_numberOfRows);

        //Iterate over all particles
        for(uint32 i = 0; i < _count; i++)
        {
            //Get current particle
            Particle* particle = _particleStorage[i];

            if(!particle->Update((float) Window::GetDeltaTime_sec()) || (particle->GetPosition().y >= _respawnTreshold))
            {
                particle->Respawn();
            }

            //Update storage containers
            _modelViewStorage.at(i)   = GetModelViewMatrix(particle);
            _texOffsetStorage.at(i)   = glm::vec4(particle->GetTextureOffset0(), particle->GetTextureOffset1());
            _blendFactorStorage.at(i) = particle->GetBlendFactor();
        }

        //Upload updated vbo's to the gpu
        UpdateGpuStorage();

        //Render particles instanced
        GLCall(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, _verticeCount, _count))

        //Sort particles depending on the distance to the camera
        std::sort(_particleStorage.begin(), _particleStorage.end(),
                  [] (Particle const* a, Particle const* b)
                 { return a->GetDistanceToCam() < b->GetDistanceToCam(); });

        //Unbind vao and vbo's
        _vboBlend->Unbind();
        _vboTex->Unbind();
        _vboModel->Unbind();
        _vboVert->Unbind();
        _vao->Unbind();

        //Unbind texture
        _glTextureAtlas->Unbind();

        //Unbind shader
        _shader->Unbind();

        //Save stats
        RenderStatistics::drawnVertices += _verticeCount * _count;
        RenderStatistics::drawCalls++;
        RenderStatistics::particlePasses++;

        GLRenderSettings::EnableDepthtest();
    }
}
