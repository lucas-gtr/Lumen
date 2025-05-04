#include <algorithm>
#include <cmath>

#include "Core/CommonTypes.hpp"
#include "Surface/TextureFiltering.hpp"

namespace TextureSampling {

PixelCoord sampleNearest(TextureUV uv_coord, Resolution resolution) {
  const int x = static_cast<int>(uv_coord.u * static_cast<double>(resolution.width));
  const int y = static_cast<int>(uv_coord.v * static_cast<double>(resolution.height));

  PixelCoord pixel_coord;
  pixel_coord.x = std::clamp(x, 0, resolution.width - 1);
  pixel_coord.y = std::clamp(y, 0, resolution.height - 1);

  return pixel_coord;
}

BilinearSampleInfo sampleBilinear(TextureUV uv_coord, Resolution resolution) {
  const double x = uv_coord.u * static_cast<double>(resolution.width) - 0.5;
  const double y = uv_coord.v * static_cast<double>(resolution.height) - 0.5;

  const int x0 = static_cast<int>(x);
  const int y0 = static_cast<int>(y);
  const int x1 = std::min(static_cast<int>(x + 1.0), resolution.width - 1);
  const int y1 = std::min(static_cast<int>(y + 1.0), resolution.height - 1);

  const double dx = std::clamp(x - static_cast<double>(x0), 0.0, 1.0);
  const double dy = std::clamp(y - static_cast<double>(y0), 0.0, 1.0);

  return {x0, x1, y0, y1, dx, dy};
}

} // namespace TextureSampling
