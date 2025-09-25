#include "Texture.h"
#define TINYGLTF_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "GltfModel.h"
#include <string>
#include <iostream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GltfModel::GltfModel(const std::string& filename) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromFile(&m_Model, &err, &warn, filename);
    // bool ret = loader.LoadASCIIFromFile(&m_Model, &err, &warn, filename);

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }
}

ModelView GltfModel::BindModel() {
    const tinygltf::Scene& scene = m_Model.scenes[m_Model.defaultScene];

    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < m_Model.nodes.size()));
        VertexArray va;
        va.Bind();
        BindModelNodes(m_Model.nodes[scene.nodes[i]]);
        va.UnBind();
        m_Vas.insert({i, std::move(va)});
    }

    // cleanup vbos but do not delete index buffers yet
    for (auto it = m_Vbs.cbegin(); it != m_Vbs.cend(); ) {
        tinygltf::BufferView bufferView = m_Model.bufferViews[it->first];
        if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
            m_Vbs.erase(it++);
        } else {
            ++it;
        }
    }

  return {&m_Vas, &m_Vbs, &m_Texs};
}

void GltfModel::BindMesh(tinygltf::Mesh& mesh) {
    for (size_t i = 0; i < m_Model.bufferViews.size(); ++i) {
        const tinygltf::BufferView &bufferView = m_Model.bufferViews[i];
        if (bufferView.target == 0) {  // TODO impl drawarrays
            std::cout << "WARN: bufferView.target is zero" << std::endl;
            continue;  // Unsupported bufferView.
            /*
                   From spec2.0 readme:
                   https://github.com/KhronosGroup/glTF/tree/master/specification/2.0
                            ... drawArrays function should be used with a count equal to
                   the count            property of any of the accessors referenced by the
                   attributes            property            (they are all equal for a given
                   primitive).
                 */
        }

        const tinygltf::Buffer &buffer = m_Model.buffers[bufferView.buffer];
        std::cout << "bufferview.target " << bufferView.target << std::endl;


        bool isIndexBuffer = bufferView.target == GL_ELEMENT_ARRAY_BUFFER;
        VertexBuffer vb {&buffer.data.at(0) + bufferView.byteOffset, static_cast<unsigned int>(bufferView.byteLength), isIndexBuffer};

        vb.Bind();

        std::cout << "buffer.data.size = " << buffer.data.size()
            << ", bufferview.byteOffset = " << bufferView.byteOffset
            << std::endl;

        m_Vbs.insert({i, std::move(vb)});
    }

    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = m_Model.accessors[primitive.indices];

        for (auto &attrib : primitive.attributes) {
            tinygltf::Accessor accessor = m_Model.accessors[attrib.second];
            int byteStride =
                accessor.ByteStride(m_Model.bufferViews[accessor.bufferView]);
            VertexBuffer& vertexBuffer = m_Vbs.at(accessor.bufferView);
            vertexBuffer.Bind();

            int size = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                size = accessor.type;
            }

            int vaa = -1;
            if (attrib.first.compare("POSITION") == 0) vaa = 0;
            if (attrib.first.compare("NORMAL") == 0) vaa = 1;
            if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
            if (vaa > -1) {
                glEnableVertexAttribArray(vaa);
                glVertexAttribPointer(vaa, size, accessor.componentType,
                                      accessor.normalized ? GL_TRUE : GL_FALSE,
                                      byteStride, BUFFER_OFFSET(accessor.byteOffset));
            } else {
                std::cout << "vaa missing: " << attrib.first << std::endl;
            }
        }

        if (primitive.material > -1) {
            tinygltf::Material& material = m_Model.materials[primitive.material];
            if (material.pbrMetallicRoughness.baseColorFactor.size() > 0) {
                // base color texture
                tinygltf::Texture& tex = m_Model.textures[primitive.material];
                tinygltf::Image image = m_Model.images[tex.source];
                Texture texture {&image.image.at(0), image.width, image.height, image.component};
                m_Texs.insert({primitive.material, std::move(texture)});
            } else {
                // pbr texture
                tinygltf::Texture& tex = m_Model.textures[primitive.material];
                tinygltf::Image image = m_Model.images[tex.source];
                Texture texture {&image.image.at(0), image.width, image.height, image.component};
                m_Texs.insert({primitive.material, std::move(texture)});
            }
        }

        if (m_Model.textures.size() > 0) {
            // fixme: Use material's baseColor
            // tinygltf::Texture &tex = m_Model.textures[0];

            // if (tex.source > -1) {
                // // tinygltf::Image& image = m_Model.images[tex.source];
                //
                // // tinygltf::Image &image = m_Model.images[tex.source];
                // // Texture texture {&image.image.at(0)};
                // // texture.Bind();
                //
                // GLuint texid;
                // glGenTextures(1, &texid);
                //
                // tinygltf::Image &image = m_Model.images[tex.source];
                //
                // glBindTexture(GL_TEXTURE_2D, texid);
                // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                //
                // GLenum format = GL_RGBA;
                //
                // if (image.component == 1) {
                //   format = GL_RED;
                // } else if (image.component == 2) {
                //   format = GL_RG;
                // } else if (image.component == 3) {
                //   format = GL_RGB;
                // } else {
                //   // ???
                // }
                //
                // GLenum type = GL_UNSIGNED_BYTE;
                // if (image.bits == 8) {
                //   // ok
                // } else if (image.bits == 16) {
                //   type = GL_UNSIGNED_SHORT;
                // } else {
                //   // ???
                // }
                //
                // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                //              format, type, &image.image.at(0));
            // }
        }
    }
}

void GltfModel::BindModelNodes(tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < m_Model.meshes.size())) {
        BindMesh(m_Model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < m_Model.nodes.size()));
        BindModelNodes(m_Model.nodes[node.children[i]]);
    }
}

void GltfModel::Bind() {
}

void GltfModel::UnBind() {
}

