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
#include "Surface/TextureManager.hpp"

/**
 * @class Material
 * @brief Class representing a material with albedo and normal textures.
 *
 * This class encapsulates the properties of a material, including its albedo and normal textures.
 */
class Material {
private:
  Texture* m_diffuse_texture = TextureManager::DefaultDiffuseTexture();
  Texture* m_normal_texture  = TextureManager::DefaultNormalTexture();

  Texture* m_roughness_texture     = TextureManager::DefaultMidGrayTexture();
  double   m_roughness_value       = DEFAULT_ROUGHNESS_VALUE;
  bool     m_use_texture_roughness = false;

  Texture* m_metallic_texture     = TextureManager::DefaultBlackTexture();
  double   m_metallic_value       = 0.0;
  bool     m_use_texture_metallic = false;

  Texture* m_emissive_texture   = TextureManager::DefaultBlackTexture();
  double   m_emissive_intensity = 1.0;

  Observer<const Material*> m_material_changed_observer;

public:
  Material() = default; ///< Default constructor.

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
   * @brief Sets the roughness texture for this material.
   * @param texture The texture to set as the roughness texture.
   */
  void setRoughnessTexture(Texture* texture);

  /**
   * @brief Sets the roughness value for this material.
   * @param value The roughness value to set.
   */
  void setRoughnessValue(double value);

  /**
   * @brief Gets the roughness texture of this material.
   * @return The pointer to the roughness texture.
   */
  Texture* getRoughnessTexture() const;

  /**
   * @brief Checks if the material is using a texture for roughness.
   * @return True if a texture is used for roughness, false otherwise.
   */
  bool isUsingTextureRoughness() const { return m_use_texture_roughness; }

  /**
   * @brief Sets whether to use a texture for roughness.
   * @param use_texture True to use a texture, false to use a value.
   */
  void setUseTextureRoughness(bool use_texture);

  /**
   * @brief Sets the metallic texture for this material.
   * @param texture The texture to set as the metallic texture.
   */
  void setMetallicTexture(Texture* texture);

  /**
   * @brief Sets the metallic value for this material.
   * @param value The metallic value to set.
   */
  void setMetallicValue(double value);

  /**
   * @brief Gets the metallic texture of this material.
   * @return The pointer to the metallic texture.
   */
  Texture* getMetallicTexture() const;

  /**
   * @brief Checks if the material is using a texture for metallic.
   * @return True if a texture is used for metallic, false otherwise.
   */
  bool isUsingTextureMetallic() const { return m_use_texture_metallic; }

  /**
   * @brief Sets whether to use a texture for metallic.
   * @param use_texture True to use a texture, false to use a value.
   */
  void setUseTextureMetallic(bool use_texture);

  /**
   * @brief Sets the emissive texture for this material.
   * @param texture The texture to set as the emissive texture.
   */
  void setEmissiveTexture(Texture* texture);

  /**
   * @brief Gets the emissive texture of this material.
   * @return The pointer to the emissive texture.
   */
  Texture* getEmissiveTexture() const;

  /**
   * @brief Sets the emissive intensity for this material.
   * @param intensity The emissive intensity to set.
   */
  void setEmissiveIntensity(double intensity);

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

  /**
   * @brief Gets the roughness texture of this material.
   * @return The roughness texture.
   */
  double getRoughness(TextureUV uv_coord) const;

  /**
   * @brief Gets the metallic texture of this material.
   * @return The metallic texture.
   */
  double getMetallic(TextureUV uv_coord) const;

  /**
   * @brief Gets the emissive texture of this material.
   * @return The emissive texture.
   */
  ColorRGB getEmissive(TextureUV uv_coord) const;

  /**
   * @brief Gets the emissive intensity of this material.
   * @return The emissive intensity.
   */
  double getEmissiveIntensity() const;

  ~Material() = default; ///< Default destructor.
};

#endif // SURFACE_MATERIAL_HPP