#pragma once

#include <vector>
#include <GL/glew.h>

#include "Macros.h"

struct VertexBufferLayoutElement {
    unsigned int type; // 数据类型
    unsigned int count; // 一个vertex有多少个元素
    unsigned char normalized; // ?
    int vaa;
    long offset;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferLayoutElement> m_Elements;
    unsigned int m_Stride; // 一个vertex有多少个字节

public:
    VertexBufferLayout() : m_Stride(0) {}

    ~VertexBufferLayout() = default;

    inline unsigned int GetStride() const { return m_Stride; }
    inline std::vector<VertexBufferLayoutElement> GetElements() const { return m_Elements; }

    template<typename T>
    void Push(unsigned int count);

    void PushElement(unsigned int type, unsigned int count, bool normalized,
                     int vaa, long offset, unsigned int stride);
};

