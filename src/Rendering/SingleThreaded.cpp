#include <cmath>
#include <iostream>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/SingleThreaded.hpp"

bool SingleThreaded::render() {
  const int samples_per_pixel = getRenderer()->getRenderSettings().getSamplesPerPixel();

  const double sample_weight   = 1.0 / samples_per_pixel;
  const int    samples_per_row = static_cast<int>(std::sqrt(samples_per_pixel));
  const double cell_size       = 1.0 / static_cast<double>(samples_per_row);

  const int height = getRenderer()->getRenderSettings().getHeight();
  const int width  = getRenderer()->getRenderSettings().getWidth();

  getRenderer()->getFramebuffer()->initThreadBuffers(1);
  Framebuffer::setThreadId(0);

  for(int s = 0; s < samples_per_pixel; ++s) {
    std::cout << "Sample: " << s + 1 << "/" << samples_per_pixel << '\n';
    const PixelCoord grid_pos{s % samples_per_row, s / samples_per_row};
    getRenderer()->renderSample({0, 0}, {width, height}, sample_weight, grid_pos, cell_size);
  }

  getRenderer()->getFramebuffer()->reduceThreadBuffers();
  return true;
}