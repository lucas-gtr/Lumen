/**
 * @file MaterialManager.hpp
 * @brief Header file for the MaterialManager class, which manages materials in the application.
 */
#ifndef SURFACE_MATERIALMANAGER_HPP
#define SURFACE_MATERIALMANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Surface/Material.hpp"

/**
 * @class MaterialManager
 * @brief Manages materials in the application.
 *
 * The MaterialManager class is responsible for loading, retrieving, and managing materials.
 */
class MaterialManager {
private:
  std::unordered_map<std::string, std::unique_ptr<Material>> m_material_map;

  static std::unique_ptr<Material> s_default_material;

  Observer<std::string>              m_material_added_observer;
  Observer<std::string>              m_material_removed_observer;
  Observer<std::string, std::string> m_material_renamed_observer;

public:
  /**
   * @brief Default constructor for the MaterialManager class.
   */
  MaterialManager() = default;

  MaterialManager(const MaterialManager&)            = delete;
  MaterialManager& operator=(const MaterialManager&) = delete;
  MaterialManager(MaterialManager&&)                 = delete;
  MaterialManager& operator=(MaterialManager&&)      = delete;

  /**
   * @brief Gets the observer that notifies when a material is added.
   * @return A reference to the observer that notifies about material additions.
   */
  Observer<std::string>& getMaterialAddedObserver() { return m_material_added_observer; }

  /**
   * @brief Gets the observer that notifies when a material is removed.
   * @return A reference to the observer that notifies about material removals.
   */
  Observer<std::string>& getMaterialRemovedObserver() { return m_material_removed_observer; }

  /**
   * @brief Gets the observer that notifies when a material is renamed.
   * @return A reference to the observer that notifies about material renames.
   */
  Observer<std::string, std::string>& getMaterialRenamedObserver() { return m_material_renamed_observer; }

  /**
   * @brief Adds a material to the manager.
   * @param material_name The name of the material to add.
   */
  void addMaterial(const std::string& material_name);

  /**
   * @brief Gets a unique name for a new material based on the provided name.
   * @param name The base name for the material.
   * @return A unique name for the material, ensuring it does not conflict with existing names.
   */
  std::string getAvailableMaterialName(const std::string& name) const;

  /**
   * @brief Renames a material.
   * @param old_name The current name of the material.
   * @param new_name The new name for the material.
   * @return True if the material was successfully renamed, false if the new name is already in use.
   */
  bool renameMaterial(const std::string& old_name, const std::string& new_name);

  /**
   * @brief Retrieves a material by its name.
   * @param material_name The name of the material to retrieve.
   */
  Material* getMaterial(const std::string& material_name) const;

  /**
   * @brief Retrieves the name of a material.
   * @param material The material whose name is to be retrieved.
   * @return The name of the material.
   */
  std::string getMaterialName(const Material* material) const;

  /**
   * @brief Removes a material from the manager by its name.
   * @param material_name The name of the material to remove.
   */
  void removeMaterial(const std::string& material_name);

  /**
   * @brief Gets all material names managed by the MaterialManager.
   * @return A vector of strings containing the names of all materials.
   */
  std::vector<std::string> getAllMaterialsName() const;

  /**
   * @brief Gets the default material.
   * @return A pointer to the default material.
   */
  static Material* DefaultMaterial();

  ~MaterialManager() = default; ///< Default destructor for the MaterialManager class.
};

#endif // SURFACE_MATERIALMANAGER_HPP