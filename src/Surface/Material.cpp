#include "Surface/Material.hpp"
#include "Core/CommonTypes.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

Material::Material()
    : m_diffuse_texture(TextureManager::DefaultDiffuseTexture()),
      m_normal_texture(TextureManager::DefaultNormalTexture()) {}

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

ColorRGBA Material::getDiffuse(TextureUV uv_coord) const { return m_diffuse_texture->getValue4d(uv_coord); }

ColorRGB Material::getNormal(TextureUV uv_coord) const { return m_normal_texture->getValue3d(uv_coord); }

Texture* Material::getDiffuseTexture() const {
  if(m_diffuse_texture == nullptr) {
    return TextureManager::DefaultDiffuseTexture();
  }
  return m_diffuse_texture;
}

Texture* Material::getNormalTexture() const {
  if(m_normal_texture == nullptr) {
    return TextureManager::DefaultNormalTexture();
  }
  return m_normal_texture;
}