/**
 * @file MaterialGL.hpp
 * @brief Header file for the MaterialGL class, which represents a material in OpenGL.
 */
#ifndef GPU_OPENGL_MATERIALGL_HPP
#define GPU_OPENGL_MATERIALGL_HPP

// GCOVR_EXCL_START

#include "GPU/OpenGL/TextureGL.hpp"
#include "Surface/Material.hpp"

/**
 * @class MaterialGL
 * @brief A class that represents a material in OpenGL, encapsulating its properties and textures.
 *
 * This class is designed to work with OpenGL and provides methods to bind and unbind the material's textures.
 * It holds references to the source Material object and its associated textures, such as albedo and normal textures.
 */
class MaterialGL {
private:
  const Material* m_material;

  const TextureGL* m_albedoTexture;
  const TextureGL* m_normalTexture;

public:
  /**
   * @brief Constructor for MaterialGL.
   * Initializes the material with the given Material object and textures.
   * @param material The Material object to initialize this MaterialGL with.
   * @param albedoTexture Pointer to the albedo texture (diffuse texture).
   * @param normalTexture Pointer to the normal texture.
   */
  MaterialGL(const Material& material, const TextureGL* albedoTexture, const TextureGL* normalTexture)
      : m_material(&material), m_albedoTexture(albedoTexture), m_normalTexture(normalTexture) {}

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
   * @brief Binds the material textures to the appropriate texture units.
   */
  void bind() const;

  /**
   * @brief Unbinds the material textures from the texture units.
   */
  static void unbind();

  ~MaterialGL() = default; ///< Default destructor.
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_MATERIALGL_HPP