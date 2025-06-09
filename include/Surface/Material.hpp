/**
 * @file Material.hpp
 * @brief Header file for the Material class.
 */
#ifndef SURFACE_MATERIAL_HPP
#define SURFACE_MATERIAL_HPP

#include <memory>

#include "Core/CommonTypes.hpp"
#include "Surface/Texture.hpp"

/**
 * @class Material
 * @brief Class representing a material with albedo and normal textures.
 *
 * This class encapsulates the properties of a material, including its albedo and normal textures.
 */
class Material {
private:
  std::shared_ptr<Texture> m_albedoTexture;
  std::shared_ptr<Texture> m_normalTexture;

public:
  Material();                                     ///< Default constructor.
  Material(const Material&)            = default; ///< Default copy constructor
  Material(Material&&)                 = default; ///< Default move constructor
  Material& operator=(const Material&) = default; ///< Default copy assignment
  Material& operator=(Material&&)      = default; ///< Default move assignment

  /**
   * @brief Sets the albedo texture for this material.
   * @param texture The texture to set as the albedo texture.
   */
  void setAlbedoTexture(const std::shared_ptr<Texture>& texture) { m_albedoTexture = texture; }

  const Texture& getAlbedoTexture() const; ///< Gets the albedo texture of this material.

  const Texture& getNormalTexture() const; ///< Gets the normal texture of this material.

  /**
   * @brief Sets the normal texture for this material.
   * @param texture The texture to set as the normal texture.
   */
  void setNormalTexture(const std::shared_ptr<Texture>& texture) { m_normalTexture = texture; }

  /**
   * @brief Gets the albedo texture of this material.
   * @return The albedo texture.
   */
  ColorRGBA getAlbedo(TextureUV uv_coord) const;
  /**
   * @brief Gets the normal texture of this material.
   * @return The normal texture.
   */
  ColorRGB getNormal(TextureUV uv_coord) const;

  ~Material() = default; ///< Default destructor.
};

#endif // SURFACE_MATERIAL_HPP