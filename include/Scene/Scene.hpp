/**
 * @file Scene.hpp
 * @brief Header file for the Scene class.
 */
#ifndef SCENE_SCENE_HPP
#define SCENE_SCENE_HPP

#include <linalg/Vec3.hpp>
#include <memory>
#include <utility>
#include <vector>

#include "BVH/BVHNode.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Observer.hpp"
#include "Lighting/Light.hpp"
#include "Scene/Skybox.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Texture.hpp"

/**
 * @class Scene
 * @brief Represents a 3D scene that holds objects and a camera.
 */
class Scene {
private:
  std::unordered_map<std::string, std::unique_ptr<Object3D>> m_object_map;
  std::vector<Object3D*>                                     m_object_index;

  std::unordered_map<std::string, std::unique_ptr<Light>> m_light_map;
  std::vector<Light*>                                     m_light_index;

  std::unique_ptr<Camera> m_current_camera;
  std::unique_ptr<Skybox> m_skybox;

  std::shared_ptr<BVHNode> m_bvh_root = nullptr;

  Observer<Object3D*> m_object_added_observer;
  Observer<Light*>    m_light_added_observer;

public:
  Scene();

  Scene(const Scene& other)            = delete;
  Scene& operator=(const Scene& other) = delete;
  Scene(Scene&& other)                 = delete;
  Scene& operator=(Scene&& other)      = delete;

  /**
   * @brief Gets the available name for a new object in the scene.
   * @param name The base name for the object.
   * @return A unique name for the object that does not conflict with existing names.
   */
  std::string getAvailableObjectName(const std::string& name) const;

  /**
   * @brief Gets the available name for a new light in the scene.
   * @param name The base name for the light.
   * @return A unique name for the light that does not conflict with existing names.
   */
  std::string getAvailableLightName(const std::string& name) const;

  /**
   * @brief Adds a 3D object to the scene.
   * @param name The name of the object to be added.
   * @param object A unique pointer to the Object3D to be added to the scene.
   */
  void addObject(const std::string& name, std::unique_ptr<Object3D> object);

  /**
   * @brief Renames an existing object in the scene.
   * @param old_name The current name of the object.
   * @param new_name The new name for the object.
   * @return True if the renaming was successful, false if the object with old_name does not exist or new_name is
   * already taken.
   */
  bool renameObject(const std::string& old_name, const std::string& new_name);

  /**
   * @brief Removes an object from the scene by its name.
   * @param name The name of the object to be removed.
   */
  void removeObject(const std::string& name);

  /**
   * @brief Gets the name of an object in the scene.
   * @param object A pointer to the Object3D for which to get the name.
   * @return The name of the object as a string.
   */
  std::string getObjectName(const Object3D* object) const;

  /**
   * @brief Gets the observer for when an object is added to the scene.
   * @return A reference to the observer that notifies when an object is added.
   */
  Observer<Object3D*>& getObjectAddedObserver() { return m_object_added_observer; }

  /**
   * @brief Adds a light to the scene.
   * @param name The name of the light to be added.
   * @param light A unique pointer to a Light object to be added to the scene.
   */
  void addLight(const std::string& name, std::unique_ptr<Light> light);

  /**
   * @brief Renames an existing light in the scene.
   * @param old_name The current name of the light.
   * @param new_name The new name for the light.
   * @return True if the renaming was successful, false if the light with old_name does not exist or new_name is already
   * taken.
   */
  bool renameLight(const std::string& old_name, const std::string& new_name);

  /**
   * @brief Removes a light from the scene by its name.
   * @param name The name of the light to be removed.
   */
  void removeLight(const std::string& name);

  /**
   * @brief Gets the name of a light in the scene.
   * @param light A pointer to the Light for which to get the name.
   * @return The name of the light as a string.
   */
  std::string getLightName(const Light* light) const;

  /**
   * @brief Gets the observer for when a light is added to the scene.
   * @return A reference to the observer that notifies when a light is added.
   */
  Observer<Light*>& getLightAddedObserver() { return m_light_added_observer; }

  /**
   * @brief Gets an object by its name.
   * @param name The name of the object to retrieve.
   * @return A pointer to the Object3D with the specified name, or nullptr if not found.
   */
  Object3D* getObject(const std::string& name) const;

  /**
   * @brief Gets the list of objects in the scene.
   *
   * @return A constant reference to the list of Object3D pointers in the scene.
   */
  const std::vector<Object3D*>& getObjectList() const { return m_object_index; }

  /**
   * @brief Gets the list of lights in the scene.
   *
   * @return A constant reference to the list of Light pointers in the scene.
   */
  const std::vector<Light*>& getLightList() const { return m_light_index; }

  /**
   * @brief Gets a light by its name.
   * @param name The name of the light to retrieve.
   * @return A pointer to the Light with the specified name, or nullptr if not found.
   */
  Light* getLight(const std::string& name) const;

  /**
   * @brief Gets the current camera of the scene.
   *
   * @return A pointer to the Camera object.
   */
  Camera* getCamera() const { return m_current_camera.get(); }

  /**
   * @brief Gets the color of the skybox based on a given direction.
   *
   * @param direction The direction vector for which to get the skybox color.
   * @return The color of the skybox.
   */
  ColorRGBA getSkyboxColor(const linalg::Vec3d& direction) const { return m_skybox->getColor(direction); }

  /**
   * @brief Sets the skybox texture for the scene.
   *
   * @param skybox_texture A shared pointer to the Texture to be used as the skybox.
   */
  void setSkybox(Texture* skybox_texture) { m_skybox->setTexture(skybox_texture); }

  /**
   * @brief Gets the skybox of the scene.
   * @return A pointer to the Skybox object.
   */
  Skybox* getSkybox() const { return m_skybox.get(); }

  /**
   * @brief Builds the bounding volume hierarchy (BVH) for the objects in the scene.
   */
  void buildBVH();

  /**
   * @brief Gets the bounding volume hierarchy (BVH) root node.
   *
   * @return A pointer to the BVHNode representing the root of the BVH.
   */
  const BVHNode* getBVHRoot() const { return m_bvh_root.get(); }

  ~Scene() = default; ///< Default destructor.
};

#endif // SCENE_SCENE_HPP