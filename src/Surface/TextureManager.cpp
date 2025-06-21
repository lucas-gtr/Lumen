#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

std::unique_ptr<Texture> TextureManager::s_default_skybox_texture  = nullptr;
std::unique_ptr<Texture> TextureManager::s_default_diffuse_texture = nullptr;
std::unique_ptr<Texture> TextureManager::s_default_normal_texture  = nullptr;

Texture* TextureManager::defaultSkyboxTexture() {
  if(s_default_skybox_texture == nullptr) {
    s_default_skybox_texture = std::make_unique<Texture>();
    s_default_skybox_texture->setTextureType(TextureType::COLOR_TEXTURE);
    s_default_skybox_texture->setValue(
        ColorRGBA{DEFAULT_SKYBOX_COLOR_R, DEFAULT_SKYBOX_COLOR_G, DEFAULT_SKYBOX_COLOR_B, 1.0});
    s_default_skybox_texture->setColorSpace(ColorSpace::Linear);
  }
  return s_default_skybox_texture.get();
}

Texture* TextureManager::defaultDiffuseTexture() {
  if(s_default_diffuse_texture == nullptr) {
    s_default_diffuse_texture = std::make_unique<Texture>();
    s_default_diffuse_texture->setTextureType(TextureType::COLOR_TEXTURE);
    s_default_diffuse_texture->setValue(ColorRGBA{1.0, 1.0, 1.0, 1.0});
    s_default_diffuse_texture->setColorSpace(ColorSpace::Linear);
  }
  return s_default_diffuse_texture.get();
}

Texture* TextureManager::defaultNormalTexture() {
  if(s_default_normal_texture == nullptr) {
    s_default_normal_texture = std::make_unique<Texture>();
    s_default_normal_texture->setTextureType(TextureType::COLOR_TEXTURE);
    s_default_normal_texture->setValue(ColorRGB{HALF, HALF, 1.0});
    s_default_normal_texture->setColorSpace(ColorSpace::sRGB);
  }
  return s_default_normal_texture.get();
}

void TextureManager::addTexture(const std::string& texture_name) {
  if(m_texture_map.find(texture_name) == m_texture_map.end()) {
    m_texture_map[texture_name] = std::make_unique<Texture>();
    m_texture_added_observer.notify(texture_name);
  }
}

std::string TextureManager::getAvailableTextureName(const std::string& name) const {
  if(m_texture_map.find(name) == m_texture_map.end()) {
    return name;
  }
  size_t      index    = 1;
  std::string new_name = name + "_" + std::to_string(index);
  while(m_texture_map.find(new_name) != m_texture_map.end()) {
    ++index;
    new_name = name + "_" + std::to_string(index);
  }
  return new_name;
}

Texture* TextureManager::getTexture(const std::string& texture_name) const {
  auto it = m_texture_map.find(texture_name);
  if(it != m_texture_map.end()) {
    return it->second.get();
  }
  return nullptr;
}

std::string TextureManager::getTextureName(const Texture* texture) const {
  for(const auto& pair : m_texture_map) {
    if(pair.second.get() == texture) {
      return pair.first;
    }
  }
  return "";
}

bool TextureManager::renameTexture(const Texture* texture, const std::string& new_name) {
  if(new_name.empty()) {
    return false;
  }
  for(auto it = m_texture_map.begin(); it != m_texture_map.end(); ++it) {
    if(it->second.get() == texture) {
      if(m_texture_map.find(new_name) != m_texture_map.end()) {
        return false;
      }
      auto node = m_texture_map.extract(it);
      m_texture_renamed_observer.notify(node.key(), new_name);

      node.key() = new_name;
      m_texture_map.insert(std::move(node));

      return true;
    }
  }
  return false;
}

void TextureManager::removeTexture(const std::string& texture_name) {
  auto it = m_texture_map.find(texture_name);
  if(it != m_texture_map.end()) {
    m_texture_map.erase(it);
    m_texture_removed_observer.notify(texture_name);
  }
}