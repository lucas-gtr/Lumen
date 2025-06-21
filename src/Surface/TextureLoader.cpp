#include <cstdint>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Surface/TextureLoader.hpp"

namespace TextureLoader {

void load(const char* filename, std::vector<double>& image_data, ImageProperties& texture_properties) {
  int            width    = 0;
  int            height   = 0;
  int            channels = 0;
  unsigned char* data     = stbi_load(filename, &width, &height, &channels, 0);
  if(data == nullptr) {
    std::cerr << "Error loading texture: " << stbi_failure_reason() << '\n';
    texture_properties = {1, 1, 3};
    image_data.resize(3);
    image_data = {1.0, 0.0, 1.0};
    return;
  }
  texture_properties = {width, height, channels};
  image_data.resize(texture_properties.bufferSize());

  for(std::uint64_t i = 0; i < texture_properties.bufferSize(); ++i) {
    // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
    image_data[i] = static_cast<double>(data[i]) * COLOR8_TO_NORMALIZED;
  }

  stbi_image_free(data);
}

}; // namespace TextureLoader