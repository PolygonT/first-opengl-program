#pragma once

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const void* data, unsigned int count);
    IndexBuffer(const void* data, unsigned int count, unsigned int size);
    ~IndexBuffer();

    inline unsigned int GetRendererID() const { return m_RendererID; }

    inline unsigned int GetCount() const { return m_Count; }



    void Bind() const;
    void Unbind() const;
};
