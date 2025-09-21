#pragma once

class VertexBuffer {
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();

    inline unsigned int GetRendererID() const { return m_RendererID; }

    void Bind() const;
    void Unbind() const;
};
