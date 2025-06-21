/**
 * @file Material.hpp
 * @brief Header file for the Material class.
 */
#ifndef SURFACE_MATERIAL_HPP
#define SURFACE_MATERIAL_HPP

#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Observer.hpp"
#include "Surface/Texture.hpp"

/**
 * @class Material
 * @brief Class representing a material with albedo and normal textures.
 *
 * This class encapsulates the properties of a material, including its albedo and normal textures.
 */
class Material {
private:
  Texture* m_diffuse_texture = nullptr;
  Texture* m_normal_texture  = nullptr;

  Observer<const Material*> m_material_changed_observer;

public:
  Material(); ///< Default constructor.

  Material(const Material&)            = delete;
  Material(Material&&)                 = delete;
  Material& operator=(const Material&) = delete;
  Material& operator=(Material&&)      = delete;

  /**
   * @brief Gets the observer that notifies when the material changes.
   * @return A reference to the observer that notifies about material changes.
   */
  Observer<const Material*>& getMaterialChangedObserver() { return m_material_changed_observer; }

  /**
   * @brief Sets the albedo texture for this material.
   * @param texture The texture to set as the albedo texture.
   */
  void setDiffuseTexture(Texture* texture);

  /**
   * @brief Gets the diffuse texture of this material.
   * @return The pointer to the diffuse texture.
   */
  Texture* getDiffuseTexture() const;

  /**
   * @brief Gets the normal texture of this material.
   * @return The pointer to the normal texture.
   */
  Texture* getNormalTexture() const;

  /**
   * @brief Sets the normal texture for this material.
   * @param texture The texture to set as the normal texture.
   */
  void setNormalTexture(Texture* texture);

  /**
   * @brief Gets the albedo texture of this material.
   * @return The albedo texture.
   */
  ColorRGBA getDiffuse(TextureUV uv_coord) const;
  /**
   * @brief Gets the normal texture of this material.
   * @return The normal texture.
   */
  ColorRGB getNormal(TextureUV uv_coord) const;

  ~Material() = default; ///< Default destructor.
};

#endif // SURFACE_MATERIAL_HPP