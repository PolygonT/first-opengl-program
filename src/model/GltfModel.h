#pragma once

#include "tinygltf/tiny_gltf.h"

#include <GL/glew.h>
#include <string>


#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"

struct ModelView {
    std::map<int, VertexArray>* vaMap {nullptr};
    std::map<int, VertexBuffer>* vbMap {nullptr};
    std::map<int, Texture>* texMap {nullptr};
};

class GltfModel {
private:
    std::map<int, VertexArray> m_Vas;
    std::map<int, VertexBuffer> m_Vbs;
    std::map<int, Texture> m_Texs;

public:
    tinygltf::Model m_Model;
    GltfModel(const std::string& filename);
    ~GltfModel() = default;

    ModelView BindModel();
    void Bind();
    void UnBind();

private:
    void BindMesh(tinygltf::Mesh& mesh);
    void BindModelNodes(tinygltf::Node &node);
};
