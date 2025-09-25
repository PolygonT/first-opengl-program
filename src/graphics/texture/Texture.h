#pragma once

#include <string>

class Texture {
private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

    bool _moved {false};

public:
  Texture(const std::string &path);
  Texture(unsigned char *buffer, int width, int height, int bpp);

  Texture(const Texture &) = delete;
  Texture(Texture &&);

  Texture &operator=(const Texture &) = delete;
  Texture &operator=(Texture &&) = default;

  ~Texture();

  void Bind(unsigned int slot = 0);
  void UnBind();

  inline int GetWidth() const { return m_Width; }
  inline int GetHeight() const { return m_Height; }

private:
    void CreateTexture();
};
