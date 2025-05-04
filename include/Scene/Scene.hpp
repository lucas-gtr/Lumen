/**
 * @file Scene.hpp
 * @brief Header file for the Scene class.
 */
#ifndef SCENE_SCENE_HPP
#define SCENE_SCENE_HPP

#include <Eigen/Core>
#include <memory>
#include <utility>
#include <vector>

#include "Lighting/Light.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Object3D.hpp"
#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"

/**
 * @class Scene
 * @brief Represents a 3D scene that holds objects and a camera.
 */
class Scene {
private:
  std::vector<std::unique_ptr<Object3D>> m_object_list;
  std::vector<std::unique_ptr<Light>>    m_light_list;
  std::unique_ptr<Camera>                m_current_camera;
  std::unique_ptr<Skybox>                m_skybox;

public:
  Scene();

  Scene(const Scene& other)            = delete;
  Scene& operator=(const Scene& other) = delete;
  Scene(Scene&& other)                 = delete;
  Scene& operator=(Scene&& other)      = delete;

  /**
   * @brief Adds a 3D object to the scene.
   *
   * @param object A unique pointer to an Object3D to be added to the scene.
   */
  void addObject(std::unique_ptr<Object3D> object) { m_object_list.push_back(std::move(object)); }

  /**
   * @brief Adds a light to the scene.
   *
   * @param light A unique pointer to a Light to be added to the scene.
   */
  void addLight(std::unique_ptr<Light> light) { m_light_list.push_back(std::move(light)); }

  /**
   * @brief Sets the camera for the scene.
   *
   * @param camera A unique pointer to the Camera to be set as the current camera.
   */
  void setCamera(std::unique_ptr<Camera> camera) { m_current_camera = std::move(camera); }

  /**
   * @brief Gets the list of objects in the scene.
   *
   * @return A constant reference to the list of Object3D pointers in the scene.
   */
  const std::vector<std::unique_ptr<Object3D>>& getObjectList() const { return m_object_list; }

  /**
   * @brief Gets the list of lights in the scene.
   *
   * @return A constant reference to the list of Light pointers in the scene.
   */
  const std::vector<std::unique_ptr<Light>>& getLightList() const { return m_light_list; }

  /**
   * @brief Gets the current camera of the scene.
   *
   * @return A pointer to the Camera object.
   */
  const Camera* getCamera() const { return m_current_camera.get(); }

  /**
   * @brief Gets the color of the skybox based on a given direction.
   *
   * @param direction The direction vector for which to get the skybox color.
   * @return The color of the skybox as an Eigen::Vector4d.
   */
  Eigen::Vector4d getSkyboxColor(const Eigen::Vector3d direction) const { return m_skybox->getColor(direction); }

  /**
   * @brief Sets the skybox texture for the scene.
   *
   * @param skybox_texture A shared pointer to the Texture to be used as the skybox.
   */
  void setSkybox(const std::shared_ptr<Texture>& skybox_texture) { m_skybox->setTexture(skybox_texture); }

  ~Scene() = default; ///< Default destructor.
};

#endif // SCENE_SCENE_HPP