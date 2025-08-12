#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/Color.hpp"
#include "Core/ImageTypes.hpp"
#include "Surface/TextureLoader.hpp"

namespace TextureLoader {

namespace {
std::string getFileExtension(const std::string& filename) {
  const size_t dot_pos = filename.find_last_of('.');
  if(dot_pos == std::string::npos) {
    return "";
  }
  std::string ext = filename.substr(dot_pos + 1);
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
  return ext;
}

void handleLoadFailure(std::vector<double>& image_data, ImageProperties& texture_properties, const char* reason) {
  std::cerr << "Error loading texture: " << reason << '\n';
  texture_properties = {1, 1, 3};
  image_data         = {1.0, 0.0, 1.0};
}

void loadHDR(const char* filename, std::vector<double>& image_data, ImageProperties& texture_properties) {
  int    width    = 0;
  int    height   = 0;
  int    channels = 0;
  float* data     = stbi_loadf(filename, &width, &height, &channels, 0);
  if(data == nullptr) {
    handleLoadFailure(image_data, texture_properties, stbi_failure_reason());
    return;
  }

  texture_properties = {width, height, channels};
  image_data.resize(texture_properties.bufferSize());

  for(std::uint64_t i = 0; i < image_data.size(); ++i) {
    image_data[i] = static_cast<double>(data[i]);
  }
  stbi_image_free(data);
}

void loadLDR(const char* filename, std::vector<double>& image_data, ImageProperties& texture_properties) {
  int            width    = 0;
  int            height   = 0;
  int            channels = 0;
  unsigned char* data     = stbi_load(filename, &width, &height, &channels, 0);
  if(data == nullptr) {
    handleLoadFailure(image_data, texture_properties, stbi_failure_reason());
    return;
  }

  texture_properties = {width, height, channels};
  image_data.resize(texture_properties.bufferSize());

  for(std::uint64_t i = 0; i < image_data.size(); ++i) {
    image_data[i] = static_cast<double>(data[i]) * COLOR8_TO_NORMALIZED;
  }

  stbi_image_free(data);
}
} // namespace

void load(const char* filename, std::vector<double>& image_data, ImageProperties& texture_properties) {
  const std::string extension = getFileExtension(filename);

  if(extension.empty()) {
    handleLoadFailure(image_data, texture_properties, "No file extension found");
  } else if(extension == "hdr") {
    loadHDR(filename, image_data, texture_properties);
  } else {
    loadLDR(filename, image_data, texture_properties);
  }
}

}; // namespace TextureLoader