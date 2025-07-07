/**
 * @file TextureGL.hpp
 * @brief Header file for the TextureGL class, which represents a texture in OpenGL.
 */
#ifndef GPU_OPENGL_TEXTUREGL_HPP
#define GPU_OPENGL_TEXTUREGL_HPP

// GCOVR_EXCL_START

#include <qopenglfunctions_3_3_core.h>

#include "Core/CommonTypes.hpp"
#include "Core/Observer.hpp"
#include "GPU/ITextureGPU.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureWrapping.hpp"

/**
 * @class TextureGL
 * @brief Class representing a texture in OpenGL.
 *
 * This class encapsulates the functionality of an OpenGL texture, including uploading data to the GPU,
 * binding and unbinding the texture, and configuring its parameters.
 */
class TextureGL : public ITextureGPU, protected QOpenGLFunctions_3_3_Core {
private:
  GLuint m_texture_id = 0U;

  Observer<>::CallbackID m_texture_data_observer_id;
  Observer<>::CallbackID m_texture_parameters_observer_id;

  void configureParameters(const Texture* texture);

  void applyWrappingMode(TextureSampling::TextureWrapping wrapping_mode, const ColorRGBA& border_color);
  void applyFilteringMode(TextureSampling::TextureFiltering filtering_mode);

  static GLint GetGlWrappingMode(TextureSampling::TextureWrapping wrapping_mode);
  static GLint GetGlFilteringMode(TextureSampling::TextureFiltering filtering_mode);
  static GLint GetGlFormat(int channel_count);

public:
  /**
   * @brief Constructor for TextureGL.
   * @param texture The Texture to be represented in OpenGL.
   */
  explicit TextureGL(Texture* texture);

  TextureGL(const TextureGL&)            = delete;
  TextureGL& operator=(const TextureGL&) = delete;
  TextureGL(TextureGL&&)                 = delete;
  TextureGL& operator=(TextureGL&&)      = delete;

  /**
   * @brief Uploads the texture data to the GPU.
   */
  void uploadToGPU() override;

  /**
   * @brief Releases the OpenGL texture resources.
   */
  void release() override;

  /**
   * @brief Binds the texture to a specified texture unit.
   * @param texture_unit The texture unit to bind the texture to.
   */
  void bind(int texture_unit);

  /**
   * @brief Unbinds the texture from the current texture unit.
   * @param texture_unit The texture unit to unbind the texture from.
   */
  void unbind(int texture_unit);

  /**
   * @brief Destructor for TextureGL.
   */
  ~TextureGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_TEXTUREGL_HPP
