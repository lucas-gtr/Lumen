/**
 * @file TextureLoader.hpp
 * @brief Header file for the TextureLoader namespace.
 */
#ifndef SURFACE_TEXTURELOADER_HPP
#define SURFACE_TEXTURELOADER_HPP

#include <memory>
#include <vector>

#include "Core/CommonTypes.hpp"

/**
 * @namespace TextureLoader
 * @brief Namespace for loading textures from files.
 */
namespace TextureLoader {

/**
 * @brief Loads a texture from a file.
 * @param filename The name of the file to load the texture from.
 * @param image_data A vector to store the loaded image data.
 * @param texture_properties A reference to an ImageProperties object to store the texture properties.
 */
void load(const char* filename, std::vector<double>& image_data, ImageProperties& texture_properties);
} // namespace TextureLoader

#endif // SURFACE_TEXTURELOADER_HPP