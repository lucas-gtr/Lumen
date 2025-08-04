#include <algorithm>
#include <cmath>

#include "Core/ImageTypes.hpp"
#include "Surface/TextureWrapping.hpp"

namespace TextureSampling {

void wrapCoordinates(TextureUV& uv_coord, TextureWrapping wrapping_mode) {
  switch(wrapping_mode) {
  case TextureWrapping::REPEAT:
    wrapRepeat(uv_coord);
    break;
  case TextureWrapping::MIRRORED_REPEAT:
    wrapMirroredRepeat(uv_coord);
    break;
  case TextureWrapping::CLAMP_TO_EDGE:
    wrapClampToEdge(uv_coord);
    break;
  case TextureWrapping::CLAMP_TO_BORDER:
    wrapClampToBorder(uv_coord);
    break;
  }
}

void wrapRepeat(TextureUV& uv_coord) {
  uv_coord.u = uv_coord.u - std::floor(uv_coord.u);
  uv_coord.v = uv_coord.v - std::floor(uv_coord.v);
}

double mirror(double coord) {
  const double t = std::fmod(std::abs(coord), 2);
  return t <= 1 ? t : 2 - t;
}

void wrapMirroredRepeat(TextureUV& uv_coord) {
  uv_coord.u = mirror(uv_coord.u);
  uv_coord.v = mirror(uv_coord.v);
}

void wrapClampToEdge(TextureUV& uv_coord) {
  uv_coord.u = std::clamp(uv_coord.u, 0.0, 1.0);
  uv_coord.v = std::clamp(uv_coord.v, 0.0, 1.0);
}

void wrapClampToBorder(TextureUV& uv_coord) {
  if(uv_coord.u < 0.0 || uv_coord.u > 1.0 || uv_coord.v < 0.0 || uv_coord.v > 1.0) {
    uv_coord.u = -1.0;
    uv_coord.v = -1.0;
  }
}

} // namespace TextureSampling
