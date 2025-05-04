#include <iostream>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/CommonTypes.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureLoader.hpp"

namespace TextureLoader {

std::shared_ptr<Texture> load(const char* filename) {
  int            width    = 1;
  int            height   = 1;
  int            channels = 1;
  unsigned char* data     = stbi_load(filename, &width, &height, &channels, 0);

  if(data == nullptr) {
    std::cerr << "Failed to load texture: " << filename << '\n';
    return std::make_shared<Texture>(Eigen::Vector4d(1.0, 0.0, 1.0, 1.0));
  }
  const ImageProperties texture_properties = {width, height, channels};
  auto                  texture            = std::make_shared<Texture>(data, texture_properties);

  stbi_image_free(data);

  return texture;
}

}; // namespace TextureLoader