#include "VertexArray.h"

#include "Macros.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

// template<typename T>
// static const std::vector<std::tuple<unsigned int, T*>> ipair(std::vector<T> vec) {
//     std::vector<std::tuple<unsigned int, T*>> res;
//     for (unsigned int i = 0; i < vec.size(); i++) {
//         res.push_back({i, &vec[i]});
//     }
//
//     return res;
// }

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    long offset = 0;

    const auto elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(
            glVertexAttribPointer(
                i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::AddBuffer(unsigned int rendererID,
                            const VertexBufferLayout &layout) {
    // TODO dup bind ?
    Bind();
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));

    const auto elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(
            glVertexAttribPointer(
                element.vaa, element.count, element.type, element.normalized, layout.GetStride(), (const void*)element.offset));
    }

}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const {
    GLCall(glBindVertexArray(0));
}


