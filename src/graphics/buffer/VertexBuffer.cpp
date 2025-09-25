#include "VertexBuffer.h"
#include "Macros.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, bool isIndex) : _index(isIndex) {
    GLCall(glGenBuffers(1, &m_RendererID));
    if (_index) {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    } else {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }
}

VertexBuffer::VertexBuffer(VertexBuffer &&Other) {
    this->m_RendererID = Other.m_RendererID;
    this->_index = Other._index;
    Other._moved = true;
}

VertexBuffer::~VertexBuffer() {
    if (!_moved) {
        GLCall(glDeleteBuffers(1, &m_RendererID));
    }
}

void VertexBuffer::Bind() const {
    if (_index) {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    } else {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }
}

void VertexBuffer::Unbind() const {
    if (_index) {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    } else {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}


