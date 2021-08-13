#include "Texture.hpp"

namespace Core
{
    // ----- Public -----

    Texture::Texture(const std::string& filepath)
        : _textureID(0)
    {
        int width, height, nrChannels;
        unsigned char* localBuffer = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

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
                LOG(ERROR) << "Imageformat is not supported: " << filepath;      

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

                LOG(INFO) << "Successfully load texture at: " << filepath << " (X: " << width << " | Y: " << height << " | Channel: " << nrChannels << ")";
            }        
        }
        else
        {
            LOG(ERROR) << "Failed to load texture at: " << filepath;
        }

        stbi_image_free(localBuffer);
    }

    Texture::~Texture()
    {
        GLCall(glDeleteTextures(1, &_textureID));
    }

    void Texture::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    void Texture::BindToSlot(const unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
    }

    void Texture::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}