/**
 * @file Scene.hpp
 * @brief Header file for the Scene class.
 */
#ifndef CORE_SCENE_HPP
#define CORE_SCENE_HPP

#include "Core/Camera.hpp"
#include "Core/Object3D.hpp"
#include <Eigen/Core>
#include <memory>
#include <utility>
#include <vector>

/**
 * @class Scene
 * @brief Represents a 3D scene that holds objects and a camera.
 */
class Scene {
private:
  std::vector<std::unique_ptr<Object3D>> m_object_list;
  std::unique_ptr<Camera>                m_current_camera;

  Eigen::Vector3d m_background_color = Eigen::Vector3d::Zero(); ///< Background color of the scene.

public:
  Scene() = default; ///< Default constructor.

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
   * @brief Gets the current camera of the scene.
   *
   * @return A pointer to the Camera object.
   */
  const Camera* getCamera() const { return m_current_camera.get(); }

  /**
   * @brief Get the background color.
   * @return The background color as an Eigen::Vector3d representing RGB values.
   */
  const Eigen::Vector3d& getBackgroundColor() const { return m_background_color; }

  /**
   * @brief Set the background color.
   * @param color The background color as an Eigen::Vector3d representing RGB values.
   */
  void setBackgroundColor(const Eigen::Vector3d& color) { this->m_background_color = color; }

  ~Scene() = default; ///< Default destructor.
};

#endif // CORE_SCENE_HPP