/**
 * @file UniformBufferGL.hpp
 * @brief Header file for the UniformBufferGL class, which manages OpenGL uniform buffers.
 */
#ifndef GPU_OPENGL_UNIFORMBUFFERGL_HPP
#define GPU_OPENGL_UNIFORMBUFFERGL_HPP

// GCOVR_EXCL_START
#include <cstddef>
#include <qopenglfunctions_3_3_core.h>

/**
 * @class UniformBufferGL
 * @brief A class that manages OpenGL uniform buffers for storing uniform data.
 *
 * This class provides methods to create, update, and manage uniform buffers in OpenGL.
 * It allows for efficient data transfer to the GPU, particularly useful for storing camera matrices,
 * lighting information, and other uniform data that needs to be accessed by shaders.
 */
class UniformBufferGL : protected QOpenGLFunctions_3_3_Core {
private:
  unsigned int m_ubo  = 0;
  size_t       m_size = 0;

public:
  /**
   * @brief Constructor for UniformBufferGL.
   * @param size The size of the uniform buffer in bytes.
   * @param binding_point The binding point for the uniform buffer.
   */
  UniformBufferGL(size_t size, unsigned int binding_point);

  UniformBufferGL(const UniformBufferGL&)            = delete;
  UniformBufferGL& operator=(const UniformBufferGL&) = delete;
  UniformBufferGL(UniformBufferGL&&)                 = delete;
  UniformBufferGL& operator=(UniformBufferGL&&)      = delete;

  /**
   * @brief Updates the uniform buffer with new data.
   * @param data Pointer to the data to be uploaded.
   * @param size Size of the data in bytes.
   * @param offset Offset in the buffer where the data should be written.
   */
  void updateData(const void* data, size_t size, size_t offset);

  /**
   * @brief Gets the ID of the uniform buffer object.
   * @return The OpenGL ID of the uniform buffer.
   */
  unsigned int getID() const { return m_ubo; }

  /**
   * @brief Destructor for UniformBufferGL.
   * Cleans up the OpenGL resources associated with the uniform buffer.
   */
  ~UniformBufferGL() override;
};

// GCOVR_EXCL_STOP
#endif // GPU_OPENGL_UNIFORMBUFFERGL_HPP