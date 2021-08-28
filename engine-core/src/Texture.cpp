#include "Texture.hpp"

namespace Core
{
    // ----- Public -----

    Texture::Texture()
        :   _textureID(0)
    {

    }

    Texture::~Texture()
    {
        GLCall(glDeleteTextures(1, &_textureID));
    }

    void Texture::InitFromFile(const std::string &filepath)
    {
        int32 width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        stbi_set_flip_vertically_on_load(false);

        if(localBuffer)
        {
            GLenum format = 0;
            if(nrChannels == 1)
                format = GL_RED;
            else if(nrChannels == 3)
                format = GL_RGB;
            else if(nrChannels == 4)
                format = GL_RGBA;
            else
                LOG(ERROR) << "Failed:   Imageformat is not supported | " << filepath;

            if(format != 0)
            {
                GLCall(glGenTextures(1, &_textureID));
                Bind();

                //Texture parameters
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer));
                GLCall(glGenerateMipmap(GL_TEXTURE_2D));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

                //Activate anisotropic filtering
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0));
                GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f));

                LOG(INFO) << "Loaded:   Texture | " << filepath << " (X: " << width << " | Y: " << height << " | Channels: " << nrChannels << ")";
            }
        }
        else
        {
            LOG(ERROR) << "Failed:   Texture-Loading | " << filepath;
        }

        stbi_image_free(localBuffer);
    }

    void Texture::InitAsDepthTexture(const uint32 width, const uint32 height)
    {
        //Create depth texture
        GLCall(glGenTextures(1, &_textureID));
        Bind();
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

        //Configure depth texture
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }

    void Texture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    void Texture::BindToSlot(const uint32 slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    uint32 Texture::GetTextureID() const
    {
        return _textureID;
    }

    void Texture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}