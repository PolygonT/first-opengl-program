#include "VertexBufferLayout.h"

template<typename T>
void VertexBufferLayout::Push(unsigned int count) { 
    static_assert(false); 
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count) {
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE, 0, 0});
    m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
}

template<> 
void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE, 0, 0});
    m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<> 
void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE, 0, 0});
    m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

void VertexBufferLayout::PushElement(unsigned int type, unsigned int count, bool normalized,
                                     int vaa, long offset, unsigned int stride) {
    m_Elements.push_back({type, count, normalized, vaa, offset});
    m_Stride += stride;
}

