/**
 * @file TextureLoader.hpp
 * @brief Header file for the TextureLoader class.
 */
#ifndef SURFACE_TEXTURELOADER_HPP
#define SURFACE_TEXTURELOADER_HPP

#include "Surface/Texture.hpp"
#include <memory>

namespace TextureLoader {
/**
 * @brief Loads a texture from a file.
 * @param filename The name of the file to load the texture from.
 * @return A shared pointer to the loaded texture.
 */
std::shared_ptr<Texture> load(const char* filename);
} // namespace TextureLoader

#endif // SURFACE_TEXTURELOADER_HPP