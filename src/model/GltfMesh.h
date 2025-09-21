#pragma once

#include "IndexBuffer.h"

class GltfMesh {
private:
    IndexBuffer m_Ib;
public:
    GltfMesh(IndexBuffer& in_ib);
    ~GltfMesh() = default;

    inline IndexBuffer& GetIndexBuffer() { return m_Ib; }
};
