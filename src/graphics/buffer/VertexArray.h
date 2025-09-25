#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray {
private:
    unsigned int m_RendererID;
    bool _moved { false };
public:
    VertexArray();
    VertexArray(const VertexArray &) = delete;
    VertexArray(VertexArray &&);
    VertexArray &operator=(const VertexArray &) = delete;
    VertexArray &operator=(VertexArray &&) = default;
    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void AddBuffer(const VertexBufferLayout &layout);

    void Bind() const;
    void UnBind() const;
};
