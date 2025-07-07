#include <cstddef>
#include <linalg/Mat3.hpp>
#include <linalg/Mat4.hpp>

#include "GPU/IObjectGPU.hpp"
#include "Geometry/Mesh.hpp"
#include "SceneObjects/Object3D.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
IObjectGPU::IObjectGPU(Object3D* object)
    : m_object(object), m_indices_count(static_cast<unsigned int>(object->getMesh().getFaces().size() * 3)),
      m_indices_size(m_indices_count * sizeof(unsigned int)) {

  updateMatrices();
  m_object->getTransformationChangedObserver().add([this]() { updateMatrices(); });

  const Mesh&  mesh        = object->getMesh();
  const size_t vertex_count = mesh.getVertices().size();
  const size_t face_count   = mesh.getFaces().size();

  m_data_size = vertex_count * Vertex::ValuePerVertex() * sizeof(float);

  const size_t floats_per_vertex = Vertex::ValuePerVertex();

  m_vertices.resize(vertex_count * floats_per_vertex);
  m_indices.resize(m_indices_count);

  for(size_t i = 0; i < vertex_count; ++i) {
    const Vertex& v      = mesh.getVertices()[i];
    const size_t  base   = i * floats_per_vertex;
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

  for(size_t i = 0; i < face_count; ++i) {
    const Face& f        = mesh.getFaces()[i];
    m_indices[i * 3 + 0] = static_cast<unsigned int>(f.vertex_indices[0]);
    m_indices[i * 3 + 1] = static_cast<unsigned int>(f.vertex_indices[1]);
    m_indices[i * 3 + 2] = static_cast<unsigned int>(f.vertex_indices[2]);
  }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

void IObjectGPU::updateMatrices() {
  if(m_object != nullptr) {
    m_model_matrix  = linalg::Mat4f(m_object->getTransformationMatrix());
    m_normal_matrix = linalg::Mat3f(m_object->getNormalMatrix());
  }
}