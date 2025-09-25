#pragma once

class VertexBuffer {
private:
    unsigned int m_RendererID;
    bool _index { false };
    bool _moved { false };
public:
    VertexBuffer(const void *data, unsigned int size, bool isIndex = false);
    VertexBuffer(const VertexBuffer &Other) = delete;
    VertexBuffer(VertexBuffer &&Other);
    VertexBuffer &operator=(const VertexBuffer &) = delete;
    VertexBuffer &operator=(VertexBuffer &&) = default;
    ~VertexBuffer();

  inline unsigned int GetRendererID() const { return m_RendererID; }

  void Bind() const;
  void Unbind() const;
};
