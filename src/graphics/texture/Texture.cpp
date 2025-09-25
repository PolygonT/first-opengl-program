#include "Texture.h"
#include "Macros.h"
#include "stb_image.h"

Texture::Texture(const std::string &path) : 
    m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0){

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    CreateTexture();

    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::Texture(unsigned char *buffer, int width, int height, int bpp) :
    m_RendererID(0), m_Filepath(""), m_LocalBuffer(buffer), m_Width(width), m_Height(height), m_BPP(bpp){

    CreateTexture();
}

Texture::Texture(Texture &&Other) {
    m_RendererID = Other.m_RendererID;
    m_Filepath = Other.m_Filepath;
    m_LocalBuffer = Other.m_LocalBuffer;
    m_Width = Other.m_Width;
    m_Height = Other.m_Height;
    m_BPP = Other.m_BPP;

    Other._moved = true;
}

void Texture::CreateTexture() {
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


Texture::~Texture() {
    if (!_moved) {
        GLCall(glDeleteTextures(1, &m_RendererID));
    }
}

void Texture::Bind(unsigned int slot) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

