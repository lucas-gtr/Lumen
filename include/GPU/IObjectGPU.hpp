/**
 * @file IObjectGPU.hpp
 * @brief Header file for the IObjectGPU interface, which defines the GPU object interface.
 */
#ifndef GPU_IOBJECTGPU_HPP
#define GPU_IOBJECTGPU_HPP

#include <cstddef>

#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "SceneObjects/Object3D.hpp"

/**
 * @class IObjectGPU
 * @brief Interface for GPU objects, defining methods for uploading and releasing GPU resources.
 *
 * This interface provides methods to upload object data to the GPU and release resources.
 * It is designed to be implemented by specific GPU object classes, such as OpenGL, Vulkan or CUDA.
 */
class IObjectGPU {
private:
  Object3D* m_object = nullptr;

  std::vector<float>        m_vertices;
  std::vector<unsigned int> m_indices;

  lin::Mat4f m_model_matrix  = lin::Mat4f::Identity();
  lin::Mat3f m_normal_matrix = lin::Mat3f::Identity();

  size_t       m_dataSize     = 0;
  unsigned int m_indicesCount = 0;
  size_t       m_indicesSize  = 0;

protected:
  size_t                    dataSize() const { return m_dataSize; }
  const std::vector<float>& vertices() const { return m_vertices; }

  size_t                           indicesSize() const { return m_indicesSize; }
  const std::vector<unsigned int>& indices() const { return m_indices; }

public:
  /**
   * @brief Constructor for IObjectGPU.
   * @param object The Object3D to be represented in GPU memory.
   */
  explicit IObjectGPU(Object3D* object);

  IObjectGPU(const IObjectGPU&)            = delete;
  IObjectGPU& operator=(const IObjectGPU&) = delete;
  IObjectGPU(IObjectGPU&&)                 = delete;
  IObjectGPU& operator=(IObjectGPU&&)      = delete;

  /**
   * @brief Gets the source Object3D associated with this GPU object.
   * @return A pointer to the Object3D source.
   */
  Object3D* getSource() const { return m_object; }

  /**
   * @brief Gets the number of indices in the index buffer.
   * @return The number of indices.
   */
  unsigned int getIndexCount() const { return m_indicesCount; }

  /**
   * @brief Gets the model matrix of the object.
   * @return A raw pointer to the model matrix data.
   */
  const float* getModelMatrix() { return m_model_matrix.data(); }

  /**
   * @brief Gets the normal matrix of the object.
   * @return A raw pointer to the normal matrix data.
   */
  const float* getNormalMatrix() { return m_normal_matrix.data(); }

  /**
   * @brief Updates the model and normal matrices based on the object's transformation.
   */
  void updateMatrices();

  virtual void uploadToGPU() = 0; ///< Pure virtual function to upload the object data to the GPU.

  virtual void release() = 0; ///< Pure virtual function to release GPU resources associated with the object.

  virtual ~IObjectGPU() = default; ///< Default destructor for the GPU object interface.
};

#endif // GPU_IOBJECTGPU_HPP