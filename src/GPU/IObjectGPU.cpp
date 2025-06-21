#include <cstddef>

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "GPU/IObjectGPU.hpp"
#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
IObjectGPU::IObjectGPU(Object3D* object)
    : m_object(object), m_indicesCount(static_cast<unsigned int>(object->getMesh().getFaces().size() * 3)),
      m_indicesSize(m_indicesCount * sizeof(unsigned int)) {

  updateMatrices();
  m_object->getTransformationChangedObserver().add([this]() { updateMatrices(); });

  const Mesh&  mesh        = object->getMesh();
  const size_t vertexCount = mesh.getVertices().size();
  const size_t faceCount   = mesh.getFaces().size();

  m_dataSize = vertexCount * Vertex::valuePerVertex() * sizeof(float);

  const size_t floatsPerVertex = Vertex::valuePerVertex();

  m_vertices.resize(vertexCount * floatsPerVertex);
  m_indices.resize(m_indicesCount);

  for(size_t i = 0; i < vertexCount; ++i) {
    const Vertex& v      = mesh.getVertices()[i];
    const size_t  base   = i * floatsPerVertex;
    m_vertices[base + 0] = static_cast<float>(v.position.x);
    m_vertices[base + 1] = static_cast<float>(v.position.y);
    m_vertices[base + 2] = static_cast<float>(v.position.z);

    m_vertices[base + 3] = static_cast<float>(v.normal.x);
    m_vertices[base + 4] = static_cast<float>(v.normal.y);
    m_vertices[base + 5] = static_cast<float>(v.normal.z);

    m_vertices[base + 6] = static_cast<float>(v.uv_coord.u);
    m_vertices[base + 7] = static_cast<float>(v.uv_coord.v);

    m_vertices[base + 8]  = static_cast<float>(v.tangent.x);
    m_vertices[base + 9]  = static_cast<float>(v.tangent.y);
    m_vertices[base + 10] = static_cast<float>(v.tangent.z);

    m_vertices[base + 11] = static_cast<float>(v.bitangent.x);
    m_vertices[base + 12] = static_cast<float>(v.bitangent.y);
    m_vertices[base + 13] = static_cast<float>(v.bitangent.z);
  }

  for(size_t i = 0; i < faceCount; ++i) {
    const Face& f        = mesh.getFaces()[i];
    m_indices[i * 3 + 0] = static_cast<unsigned int>(f.vertexIndices[0]);
    m_indices[i * 3 + 1] = static_cast<unsigned int>(f.vertexIndices[1]);
    m_indices[i * 3 + 2] = static_cast<unsigned int>(f.vertexIndices[2]);
  }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

void IObjectGPU::updateMatrices() {
  if(m_object != nullptr) {
    m_model_matrix  = lin::Mat4f(m_object->getTransformationMatrix());
    m_normal_matrix = lin::Mat3f(m_object->getNormalMatrix());
  }
}