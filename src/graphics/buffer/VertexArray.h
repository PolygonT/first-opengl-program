#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray {
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void AddBuffer(unsigned int rendererID, const VertexBufferLayout &layout);

    void Bind() const;
    void UnBind() const;
};
