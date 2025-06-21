/**
 * @file MaterialGL.hpp
 * @brief Header file for the MaterialGL class, which represents a material in OpenGL.
 */
#ifndef GPU_OPENGL_MATERIALGL_HPP
#define GPU_OPENGL_MATERIALGL_HPP

// GCOVR_EXCL_START

#include <qopenglfunctions_3_3_core.h>

#include "GPU/OpenGL/TextureGL.hpp"
#include "Surface/Material.hpp"

/**
 * @class MaterialGL
 * @brief A class that represents a material in OpenGL, encapsulating its properties and textures.
 *
 * This class is designed to work with OpenGL and provides methods to bind and unbind the material's textures.
 * It holds references to the source Material object and its associated textures, such as albedo and normal textures.
 */
class MaterialGL : public QOpenGLFunctions_3_3_Core {
private:
  Material* m_material;

  TextureGL* m_diffuse_texture;
  TextureGL* m_normal_texture;

public:
  /**
   * @brief Constructor for MaterialGL.
   * Initializes the material with the given Material object and textures.
   * @param material The Material object to initialize this MaterialGL with.
   * @param diffuse_texture Pointer to the albedo texture (diffuse texture).
   * @param normal_texture Pointer to the normal texture.
   */
  MaterialGL(Material* material, TextureGL* diffuse_texture, TextureGL* normal_texture);

  MaterialGL(const MaterialGL&)            = delete;
  MaterialGL& operator=(const MaterialGL&) = delete;
  MaterialGL(MaterialGL&&)                 = delete;
  MaterialGL& operator=(MaterialGL&&)      = delete;

  /**
   * @brief Gets a pointer to the source Material object.
   * @return A pointer to the Material object.
   */
  const Material* getSource() const { return m_material; }

  /**
   * @brief Sets the diffuse texture for this material.
   * @param texture The texture to set as the diffuse texture.
   */
  void setDiffuseTexture(TextureGL* texture) { m_diffuse_texture = texture; }

  /**
   * @brief Sets the normal texture for this material.
   * @param texture The texture to set as the normal texture.
   */
  void setNormalTexture(TextureGL* texture) { m_normal_texture = texture; }

  /**
   * @brief Binds the material textures to the appropriate texture units.
   */
  void bind();

  /**
   * @brief Unbinds the material textures from the texture units.
   */
  void unbind();

  ~MaterialGL() override = default; ///< Default destructor.
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_MATERIALGL_HPP