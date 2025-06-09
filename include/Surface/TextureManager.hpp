/**
 * @file TextureManager.hpp
 * @brief Header file for the TextureManager class, which manages textures in the application.
 */
#ifndef SURFACE_TEXTUREMANAGER_HPP
#define SURFACE_TEXTUREMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Surface/Texture.hpp"

/**
 * @class TextureManager
 * @brief Manages textures in the application.
 *
 * The TextureManager class is responsible for loading, retrieving, and managing textures.
 * It uses a hash map to store textures by their file paths or names.
 */
class TextureManager {
private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

public:
  /**
   * @brief Default constructor for the TextureManager class.
   */
  TextureManager() = default;

  TextureManager(const TextureManager&)            = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager(TextureManager&&)                 = delete;
  TextureManager& operator=(TextureManager&&)      = delete;

  /**
   * @brief Loads a texture from a file and stores it in the manager.
   * @param file_path The path to the texture file.
   * @param texture_name Optional name for the texture. If not provided, the file path will be used as the name.
   */
  void loadTexture(const std::string& file_path, const std::string& texture_name = "");

  /**
   * @brief Retrieves a texture by its name.
   * @param texture_name The name of the texture to retrieve.
   * @return A weak pointer to the texture if it exists, otherwise an empty weak pointer.
   */
  std::weak_ptr<Texture> getTexture(const std::string& texture_name) const;

  /**
   * @brief Removes a texture from the manager by its name.
   * @param texture_name The name of the texture to remove.
   */
  void removeTexture(const std::string& texture_name);

  ~TextureManager() = default; ///< Default destructor for the TextureManager class.
};

#endif // SURFACE_TEXTUREMANAGER_HPP