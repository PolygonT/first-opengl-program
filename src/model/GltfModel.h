#pragma once

#include "tinygltf/tiny_gltf.h"

#include <GL/glew.h>
#include <string>


#include "VertexArray.h"
#include "GltfMesh.h"

class GltfModel {
private:
    VertexArray m_Va;
    std::vector<GltfMesh> m_Meshs;
    unsigned int m_RendererID;

public:
    tinygltf::Model m_Model;
    GltfModel(const std::string& filename);
    ~GltfModel() = default;

    std::pair<GLuint, std::map<int, GLuint>> BindModel();
    void Bind();
    void UnBind();

    inline VertexArray& GetVertexArray() { return m_Va; }
    inline std::vector<GltfMesh> GetMeshs() { return m_Meshs; }

private:
    void BindMesh(std::map<int, GLuint>& vbos, VertexArray& va, tinygltf::Mesh& mesh);
    void BindModelNodes(std::map<int, GLuint>& vbos, VertexArray& va, tinygltf::Node &node);
};
