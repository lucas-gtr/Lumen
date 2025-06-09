#include <memory>
#include <string>
#include <unordered_map>

#include "Surface/Texture.hpp"
#include "Surface/TextureLoader.hpp"
#include "Surface/TextureManager.hpp"

void TextureManager::loadTexture(const std::string& file_path, const std::string& texture_name) {
  if(m_textures.find(texture_name) != m_textures.end()) {
    return;
  }

  auto texture = TextureLoader::load(file_path.c_str());
  if(texture) {
    m_textures[texture_name] = texture;
  }
}

std::weak_ptr<Texture> TextureManager::getTexture(const std::string& texture_name) const {
  auto it = m_textures.find(texture_name);
  if(it != m_textures.end()) {
    return it->second;
  }
  return {};
}

void TextureManager::removeTexture(const std::string& texture_name) {
  auto it = m_textures.find(texture_name);
  if(it != m_textures.end()) {
    m_textures.erase(it);
  }
}