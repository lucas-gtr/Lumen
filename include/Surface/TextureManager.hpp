/**
 * @file TextureManager.hpp
 * @brief Header file for the TextureManager class, which manages textures in the application.
 */
#ifndef SURFACE_TEXTUREMANAGER_HPP
#define SURFACE_TEXTUREMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Core/Observer.hpp"

class Texture;

/**
 * @class TextureManager
 * @brief Manages textures in the application.
 *
 * The TextureManager class is responsible for loading, retrieving, and managing textures.
 */
class TextureManager {
private:
  std::unordered_map<std::string, std::unique_ptr<Texture>> m_texture_map;

  static std::unique_ptr<Texture> s_default_skybox_texture;
  static std::unique_ptr<Texture> s_default_diffuse_texture;
  static std::unique_ptr<Texture> s_default_normal_texture;
  static std::unique_ptr<Texture> s_default_white_texture;
  static std::unique_ptr<Texture> s_default_mid_gray_texture;
  static std::unique_ptr<Texture> s_default_black_texture;

  Observer<std::string>              m_texture_added_observer;
  Observer<std::string>              m_texture_removed_observer;
  Observer<std::string, std::string> m_texture_renamed_observer;

public:
  /**
   * @brief Default constructor for the TextureManager class.
   */
  TextureManager() = default;

  TextureManager(const TextureManager&)            = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager(TextureManager&&)                 = delete;
  TextureManager& operator=(TextureManager&&)      = delete;

  Observer<std::string>&              getTextureAddedObserver() { return m_texture_added_observer; }
  Observer<std::string>&              getTextureRemovedObserver() { return m_texture_removed_observer; }
  Observer<std::string, std::string>& getTextureRenamedObserver() { return m_texture_renamed_observer; }

  /**
   * @brief Adds a texture to the manager.
   * @param texture_name The name of the texture to add.
   */
  void addTexture(const std::string& texture_name);

  /**
   * @brief Gets a unique name for a texture, ensuring it is not already in use.
   * @param name The base name for the texture.
   * @return A unique name for the texture that is not already in use.
   */
  std::string getAvailableTextureName(const std::string& name) const;

  /**
   * @brief Renames a texture in the manager.
   * @param old_name The current name of the texture.
   * @param new_name The new name for the texture.
   * @return True if the texture was successfully renamed, false otherwise.
   */
  bool renameTexture(const std::string& old_name, const std::string& new_name);

  /**
   * @brief Gets a texture by its name.
   * @param texture_name The name of the texture to retrieve.
   * @return A pointer to the Texture object, or nullptr if not found.
   */
  Texture* getTexture(const std::string& texture_name) const;

  /**
   * @brief Gets the name of a texture.
   * @param texture The texture to get the name for.
   * @return The name of the texture.
   */
  std::string getTextureName(const Texture* texture) const;

  /**
   * @brief Removes a texture from the manager by its name.
   * @param texture_name The name of the texture to remove.
   */
  void removeTexture(const std::string& texture_name);

  /**
   * @brief Gets all texture names managed by the TextureManager.
   * @return A vector of strings containing the names of all textures.
   */
  std::vector<std::string> getAllTexturesName() const;

  /**
   * @brief Gets the default skybox texture.
   * @return A pointer to the default skybox texture.
   */
  static Texture* DefaultSkyboxTexture();

  /**
   * @brief Gets the default diffuse texture.
   * @return A pointer to the default diffuse texture.
   */
  static Texture* DefaultDiffuseTexture();

  /**
   * @brief Gets the default normal texture.
   * @return A pointer to the default normal texture.
   */
  static Texture* DefaultNormalTexture();

  /**
   * @brief Gets the default white texture.
   * @return A pointer to the default white texture.
   */
  static Texture* DefaultWhiteTexture();

  /**
   * @brief Gets the default mid gray texture.
   * @return A pointer to the default mid gray texture.
   */
  static Texture* DefaultMidGrayTexture();

  /**
   * @brief Gets the default black texture.
   * @return A pointer to the default black texture.
   */
  static Texture* DefaultBlackTexture();

  ~TextureManager() = default; ///< Default destructor for the TextureManager class.
};

#endif // SURFACE_TEXTUREMANAGER_HPP