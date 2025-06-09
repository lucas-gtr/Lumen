#include "Surface/Material.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/MathConstants.hpp"
#include "Surface/Texture.hpp"

Material::Material() : m_albedoTexture(nullptr), m_normalTexture(nullptr) {}

ColorRGBA Material::getAlbedo(TextureUV uv_coord) const {
  if(m_albedoTexture) {
    return m_albedoTexture->getValue4d(uv_coord);
  }
  return {1.0, 1.0, 1.0, 1.0};
}

ColorRGB Material::getNormal(TextureUV uv_coord) const {
  if(m_normalTexture) {
    return m_normalTexture->getValue3d(uv_coord);
  }
  return {HALF, HALF, 1.0};
}

const Texture& Material::getAlbedoTexture() const {
  if(m_albedoTexture) {
    return *m_albedoTexture;
  }
  static const Texture whiteTexture(ColorRGBA{1.0, 1.0, 1.0, 1.0});
  return whiteTexture;
}

const Texture& Material::getNormalTexture() const {
  if(m_normalTexture) {
    return *m_normalTexture;
  }
  static const Texture defaultNormalTexture(ColorRGB{HALF, HALF, 1.0});
  return defaultNormalTexture;
}