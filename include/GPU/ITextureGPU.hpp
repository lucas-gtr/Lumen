/**
 * @file ITextureGPU.hpp
 * @brief Header file for the ITextureGPU interface, which defines the GPU texture interface.
 */
#ifndef GPU_ITEXTUREGPU_HPP
#define GPU_ITEXTUREGPU_HPP

#include "Surface/Texture.hpp"

/**
 * @class ITextureGPU
 * @brief Interface for GPU textures, defining methods for uploading and releasing texture resources.
 *
 * This interface provides methods to upload texture data to the GPU and release resources.
 * It is designed to be implemented by specific GPU texture classes, such as OpenGL, Vulkan or CUDA.
 */
class ITextureGPU {
private:
  Texture* m_texture;

public:
  /**
   * @brief Constructor for ITextureGPU.
   * @param texture The Texture to be represented in GPU memory.
   */
  explicit ITextureGPU(Texture* texture);

  ITextureGPU(const ITextureGPU&)            = delete;
  ITextureGPU& operator=(const ITextureGPU&) = delete;
  ITextureGPU(ITextureGPU&&)                 = delete;
  ITextureGPU& operator=(ITextureGPU&&)      = delete;

  /**
   * @brief Gets the texture source.
   * @return A pointer to the Texture object.
   */
  Texture* getSource() const { return m_texture; }

  virtual void uploadToGPU() = 0; ///< Pure virtual function to upload texture data to the GPU.

  virtual void release() = 0; ///< Pure virtual function to release GPU resources.

  virtual ~ITextureGPU() = default; ///< Virtual destructor.
};

#endif // GPU_ITEXTUREGPU_HPP