#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Macros.h"

Renderer::Renderer() {
    // enable depth buffer
    GLCall(glEnable(GL_DEPTH_TEST));
    // enable face culling 
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_FRONT));
    GLCall(glFrontFace(GL_CCW));
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer ib,
                    const Shader &shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(GltfModel &model, const Shader &shader) {
    // shader.Bind();
    //
    // VertexArray &va = model.GetVertexArray();
    // va.Bind();
    //
    // for(auto& mesh : model.GetMeshs()) {
    //     IndexBuffer& ib = mesh.GetIndexBuffer();
    //     ib.Bind();
    //
    //     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));
    // }

}

