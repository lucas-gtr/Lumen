#include <cmath>
#include <iostream>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/SingleThreaded.hpp"

bool SingleThreaded::render() {
  const int samples_per_pixel = renderer()->getRenderSettings().getSamplesPerPixel();
  renderer()->getRenderTime()->start(samples_per_pixel);

  const double sample_weight   = 1.0 / samples_per_pixel;
  const int    samples_per_row = static_cast<int>(std::sqrt(samples_per_pixel));
  const double cell_size       = 1.0 / static_cast<double>(samples_per_row);

  const int height = renderer()->getRenderSettings().getHeight();
  const int width  = renderer()->getRenderSettings().getWidth();

  renderer()->getFramebuffer()->initThreadBuffers(1);
  Framebuffer::SetThreadId(0);

  for(int s = 0; s < samples_per_pixel; ++s) {
    if(renderer()->isStopRequested()) {
      std::cerr << "Render cancelled by user.\n";
      renderer()->getRenderTime()->stop();
      return false;
    }
    std::cout << "Sample: " << s + 1 << "/" << samples_per_pixel << '\n';
    const PixelCoord grid_pos{s % samples_per_row, s / samples_per_row};
    renderer()->renderSample({0, 0}, {width, height}, sample_weight, grid_pos, cell_size);
    renderer()->getRenderTime()->update(s + 1);
    renderer()->getRenderProgressObserver().notify(static_cast<double>(s + 1) / static_cast<double>(samples_per_pixel));
  }

  renderer()->getFramebuffer()->reduceThreadBuffers();
  renderer()->getFramebuffer()->clearThreadBuffers();

  renderer()->getRenderTime()->stop();

  return true;
}