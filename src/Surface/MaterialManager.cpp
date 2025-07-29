#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "Core/Config.hpp"
#include "Surface/Material.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

std::unique_ptr<Material> MaterialManager::s_default_material = nullptr;

Material* MaterialManager::DefaultMaterial() {
  if(s_default_material == nullptr) {
    s_default_material = std::make_unique<Material>();
    s_default_material->setDiffuseTexture(TextureManager::DefaultDiffuseTexture());
    s_default_material->setNormalTexture(TextureManager::DefaultNormalTexture());
  }
  return s_default_material.get();
}

void MaterialManager::addMaterial(const std::string& material_name) {
  if(m_material_map.find(material_name) == m_material_map.end()) {
    m_material_map[material_name] = std::make_unique<Material>();
    m_material_added_observer.notify(material_name);
  }
}

std::string MaterialManager::getAvailableMaterialName(const std::string& name) const {
  if(m_material_map.find(name) == m_material_map.end()) {
    return name;
  }
  size_t      index    = 1;
  std::string new_name = name + "_" + std::to_string(index);
  while(m_material_map.find(new_name) != m_material_map.end()) {
    ++index;
    new_name = name + "_" + std::to_string(index);
  }
  return new_name;
}

bool MaterialManager::renameMaterial(const std::string& old_name, const std::string& new_name) {
  if(new_name.empty()) {
    return false;
  }
  for(auto it = m_material_map.begin(); it != m_material_map.end(); ++it) {
    if(it->first == old_name) {
      if(m_material_map.find(new_name) != m_material_map.end()) {
        return false;
      }
      auto node = m_material_map.extract(it);
      m_material_renamed_observer.notify(node.key(), new_name);

      node.key() = new_name;
      m_material_map.insert(std::move(node));

      return true;
    }
  }
  return false;
}

Material* MaterialManager::getMaterial(const std::string& material_name) const {
  auto it = m_material_map.find(material_name);
  if(it != m_material_map.end()) {
    return it->second.get();
  }
  return nullptr;
}

std::string MaterialManager::getMaterialName(const Material* material) const {
  for(const auto& pair : m_material_map) {
    if(pair.second.get() == material) {
      return pair.first;
    }
  }
  return {"Default"};
}

void MaterialManager::removeMaterial(const std::string& material_name) {
  auto it = m_material_map.find(material_name);
  if(it != m_material_map.end()) {
    m_material_map.erase(it);
    m_material_removed_observer.notify(material_name);
  }
}

std::vector<std::string> MaterialManager::getAllMaterialsName() const {
  std::vector<std::string> material_names;
  material_names.reserve(m_material_map.size());
  for(const auto& pair : m_material_map) {
    material_names.push_back(pair.first);
  }
  return material_names;
}