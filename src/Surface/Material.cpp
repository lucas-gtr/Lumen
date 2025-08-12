#include <algorithm>

#include "Core/Color.hpp"
#include "Core/Config.hpp"
#include "Core/ImageTypes.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

void Material::setDiffuseTexture(Texture* texture) {
  if(texture != nullptr) {
    m_diffuse_texture = texture;
  } else {
    m_diffuse_texture = TextureManager::DefaultDiffuseTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setNormalTexture(Texture* texture) {
  if(texture != nullptr) {
    m_normal_texture = texture;
  } else {
    m_normal_texture = TextureManager::DefaultNormalTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setRoughnessTexture(Texture* texture) {
  if(texture != nullptr) {
    m_roughness_texture = texture;
  } else {
    m_roughness_texture = TextureManager::DefaultMidGrayTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setRoughnessValue(double value) {
  m_roughness_value = std::clamp(value, 0.0, 1.0);
  m_material_changed_observer.notify(this);
}

void Material::setUseTextureRoughness(bool use_texture) {
  m_use_texture_roughness = use_texture;
  m_material_changed_observer.notify(this);
}

void Material::setMetallicTexture(Texture* texture) {
  if(texture != nullptr) {
    m_metallic_texture = texture;
  } else {
    m_metallic_texture = TextureManager::DefaultBlackTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setMetallicValue(double value) {
  m_metallic_value = std::clamp(value, 0.0, 1.0);
  m_material_changed_observer.notify(this);
}

void Material::setUseTextureMetallic(bool use_texture) {
  m_use_texture_metallic = use_texture;
  m_material_changed_observer.notify(this);
}

void Material::setEmissiveTexture(Texture* texture) {
  if(texture != nullptr) {
    m_emissive_texture = texture;
  } else {
    m_emissive_texture = TextureManager::DefaultBlackTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setEmissiveIntensity(double intensity) {
  m_emissive_intensity = std::max(0.0, intensity);
  m_material_changed_observer.notify(this);
}

void Material::setTransmissionTexture(Texture* texture) {
  if(texture != nullptr) {
    m_transmission_texture = texture;
  } else {
    m_transmission_texture = TextureManager::DefaultBlackTexture();
  }
  m_material_changed_observer.notify(this);
}

void Material::setTransmissionValue(double value) {
  m_transmission_value = std::clamp(value, 0.0, 1.0);
  m_material_changed_observer.notify(this);
}

void Material::setUseTextureTransmission(bool use_texture) {
  m_use_texture_transmission = use_texture;
  m_material_changed_observer.notify(this);
}

void Material::setIndexOfRefraction(double ior) {
  m_index_of_refraction = std::clamp(ior, 1.0, MAX_IOR);
  m_material_changed_observer.notify(this);
}

ColorRGB Material::getDiffuse(TextureUV uv_coord) const { return m_diffuse_texture->getValue3d(uv_coord); }

ColorRGB Material::getNormal(TextureUV uv_coord) const { return m_normal_texture->getValue3d(uv_coord); }

double Material::getRoughness(TextureUV uv_coord) const {
  if(m_use_texture_roughness) {
    return m_roughness_texture->getValue1d(uv_coord);
  }
  return m_roughness_value;
}

double Material::getMetalness(TextureUV uv_coord) const {
  if(m_use_texture_metallic) {
    return m_metallic_texture->getValue1d(uv_coord);
  }
  return m_metallic_value;
}

ColorRGB Material::getEmissive(TextureUV uv_coord) const { return m_emissive_texture->getValue3d(uv_coord); }

double Material::getTransmission(TextureUV uv_coord) const {
  if(m_use_texture_transmission) {
    return m_transmission_texture->getValue1d(uv_coord);
  }
  return m_transmission_value;
}

double Material::getEmissiveIntensity() const { return m_emissive_intensity; }

double Material::getIndexOfRefraction() const { return m_index_of_refraction; }
