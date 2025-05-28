/**
 * @file TextureLoader.hpp
 * @brief Header file for the TextureLoader namespace.
 */
#ifndef SURFACE_TEXTURELOADER_HPP
#define SURFACE_TEXTURELOADER_HPP

#include <memory>

#include "Surface/Texture.hpp"

/**
 * @namespace TextureLoader
 * @brief Namespace for loading textures from files.
 */
namespace TextureLoader {
/**
 * @brief Loads a texture from a file.
 * @param filename The name of the file to load the texture from.
 * @return A shared pointer to the loaded texture.
 */
std::shared_ptr<Texture> load(const char* filename);
} // namespace TextureLoader

#endif // SURFACE_TEXTURELOADER_HPP