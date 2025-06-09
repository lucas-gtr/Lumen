#include <cmath>
#include <iostream>
#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/MathConstants.hpp"
#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"

Skybox::Skybox() {
  m_texture =
      std::make_shared<Texture>(ColorRGBA(DEFAULT_SKYBOX_COLOR_R, DEFAULT_SKYBOX_COLOR_G, DEFAULT_SKYBOX_COLOR_B, 1.0));
  m_texture->setColorSpace(ColorSpace::RGB);
}

void Skybox::setTexture(const std::shared_ptr<Texture>& texture) {
  if(texture == nullptr) {
    std::cerr << "Error: Texture is null." << '\n';
    return;
  }
  m_texture = texture;
  m_texture->setColorSpace(ColorSpace::RGB);
}

TextureUV Skybox::getUVCoordinates(const lin::Vec3d& direction) {
  TextureUV uv_coord;
  uv_coord.u = HALF + (std::atan2(direction.z, direction.x) * INV_2PI);
  uv_coord.v = HALF - (std::asin(direction.y) * INV_PI);
  return uv_coord;
}

ColorRGBA Skybox::getColor(const lin::Vec3d& direction) const {
  const TextureUV uv_coord = getUVCoordinates(direction);
  return m_texture->getValue4d(uv_coord);
}