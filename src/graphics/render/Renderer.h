#pragma once

#include "GltfModel.h"
#include <GL/glew.h>


// #define ASSERT(x) if(!(x)) asm("int3");
//
// #define GLCall(x) GlClearError();\
//     x;\
//     ASSERT(GlLogCall(#x, __FILE__, __LINE__))
//
// void GlClearError();
//
// bool GlLogCall(const char *function, const char *file, int line);
class VertexArray;
class IndexBuffer;
class Shader;

class Renderer {

private:

public:
    Renderer();
    ~Renderer() = default;
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer ib, const Shader &shader);
    void Draw(GltfModel &model, const Shader &shader);
};
