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

    // bool ret = loader.LoadBinaryFromFile(&m_Model, &err, &warn, filename);
    bool ret = loader.LoadASCIIFromFile(&m_Model, &err, &warn, filename);

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

std::pair<GLuint, std::map<int, GLuint>> GltfModel::BindModel() {
    std::map<int, GLuint> vbos;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    m_RendererID = vao;
    const tinygltf::Scene& scene = m_Model.scenes[m_Model.defaultScene];

    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < m_Model.nodes.size()));
        BindModelNodes(vbos, m_Va, m_Model.nodes[scene.nodes[i]]);
    }

    // TODO
    glBindVertexArray(0);
    // cleanup vbos but do not delete index buffers yet
    for (auto it = vbos.cbegin(); it != vbos.cend();) {
        tinygltf::BufferView bufferView = m_Model.bufferViews[it->first];
        if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
            glDeleteBuffers(1, &vbos[it->first]);
            vbos.erase(it++);
        }
        else {
            ++it;
        }
    }

  return {vao, vbos};


  //     std::map<int, GLuint> vbos;
  // GLuint vao;
  // glGenVertexArrays(1, &vao);
  // glBindVertexArray(vao);
  //
  // const tinygltf::Scene &scene = model.scenes[model.defaultScene];
  // for (size_t i = 0; i < scene.nodes.size(); ++i) {
  //   assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
  //   bindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
  // }
  //
  // glBindVertexArray(0);
  // // cleanup vbos but do not delete index buffers yet
  // for (auto it = vbos.cbegin(); it != vbos.cend();) {
  //   tinygltf::BufferView bufferView = model.bufferViews[it->first];
  //   if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER) {
  //     glDeleteBuffers(1, &vbos[it->first]);
  //     vbos.erase(it++);
  //   }
  //   else {
  //     ++it;
  //   }
  // }
  //

}

void GltfModel::BindMesh(std::map<int, GLuint>& vbos, VertexArray& va, tinygltf::Mesh& mesh) {
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

    GLuint vbo;
    glGenBuffers(1, &vbo);
    vbos[i] = vbo;
    glBindBuffer(bufferView.target, vbo);

    std::cout << "buffer.data.size = " << buffer.data.size()
              << ", bufferview.byteOffset = " << bufferView.byteOffset
              << std::endl;

    glBufferData(bufferView.target, bufferView.byteLength,
                 &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
  }

  for (size_t i = 0; i < mesh.primitives.size(); ++i) {
    tinygltf::Primitive primitive = mesh.primitives[i];
    tinygltf::Accessor indexAccessor = m_Model.accessors[primitive.indices];

    for (auto &attrib : primitive.attributes) {
      tinygltf::Accessor accessor = m_Model.accessors[attrib.second];
      int byteStride =
          accessor.ByteStride(m_Model.bufferViews[accessor.bufferView]);
      glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

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
      } else
        std::cout << "vaa missing: " << attrib.first << std::endl;
    }

    if (m_Model.textures.size() > 0) {
      // fixme: Use material's baseColor
      tinygltf::Texture &tex = m_Model.textures[0];

      if (tex.source > -1) {

        GLuint texid;
        glGenTextures(1, &texid);

        tinygltf::Image &image = m_Model.images[tex.source];

        glBindTexture(GL_TEXTURE_2D, texid);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format = GL_RGBA;

        if (image.component == 1) {
          format = GL_RED;
        } else if (image.component == 2) {
          format = GL_RG;
        } else if (image.component == 3) {
          format = GL_RGB;
        } else {
          // ???
        }

        GLenum type = GL_UNSIGNED_BYTE;
        if (image.bits == 8) {
          // ok
        } else if (image.bits == 16) {
          type = GL_UNSIGNED_SHORT;
        } else {
          // ???
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                     format, type, &image.image.at(0));
      }
    }
  }
}

void GltfModel::BindModelNodes(std::map<int, GLuint>& vbos, VertexArray &va, tinygltf::Node &node) {
    if ((node.mesh >= 0) && (node.mesh < m_Model.meshes.size())) {
        BindMesh(vbos, va, m_Model.meshes[node.mesh]);
    }

    for (size_t i = 0; i < node.children.size(); i++) {
        assert((node.children[i] >= 0) && (node.children[i] < m_Model.nodes.size()));
        BindModelNodes(vbos, va, m_Model.nodes[node.children[i]]);
    }
}

void GltfModel::Bind() {
    m_Va.Bind();
}

void GltfModel::UnBind() {
    m_Va.UnBind();
}

