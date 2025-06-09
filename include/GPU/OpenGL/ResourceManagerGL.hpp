/**
 * @file ResourceManagerGL.hpp
 * @brief Header file for the ResourceManagerGL class, which manages OpenGL resources.
 */
#ifndef GPU_OPENGL_RESOURCEMANAGERGL_HPP
#define GPU_OPENGL_RESOURCEMANAGERGL_HPP

// GCOVR_EXCL_START

#include <memory>
#include <unordered_map>
#include <vector>

#include "GPU/IResourceManagerGPU.hpp"
#include "GPU/OpenGL/CameraGL.hpp"
#include "GPU/OpenGL/Lights/DirectionalGL.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "GPU/OpenGL/Lights/SpotGL.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

/**
 * @class ResourceManagerGL
 * @brief Manages OpenGL resources such as textures, materials, lights, and 3D objects.
 *
 * This class provides methods to add and manage various GPU resources,
 * including textures, materials, lights, and 3D objects. It is designed to
 * work with OpenGL for rendering graphics.
 */
class ResourceManagerGL : public IResourceManagerGPU {
private:
  std::vector<std::unique_ptr<ObjectGL>>   m_dataBuffers;
  std::vector<std::unique_ptr<TextureGL>>  m_textures;
  std::vector<std::unique_ptr<MaterialGL>> m_materials;
  std::unique_ptr<TextureGL>               m_skyboxTexture;

  std::unique_ptr<CameraGL> m_viewport_camera;

  std::vector<std::unique_ptr<DirectionalLightGL>> m_directionalLights;
  std::vector<std::unique_ptr<PointLightGL>>       m_pointLights;
  std::vector<std::unique_ptr<SpotLightGL>>        m_spotLights;

  void loadScene(int viewportWidth, int viewportHeight, const Scene& scene);

  const TextureGL*  addTexture(const Texture& texture) override;
  const MaterialGL* addMaterial(const Material& material);

  void setCamera(Camera* camera, int viewportWidth, int viewportHeight) override;

public:
  /**
   * @brief Constructor for ResourceManagerGL.
   * @param viewportWidth The width of the viewport in pixels.
   * @param viewportHeight The height of the viewport in pixels.
   * @param scene The Scene object containing the initial objects and lights.
   */
  ResourceManagerGL(int viewportWidth, int viewportHeight, const Scene& scene);

  ResourceManagerGL(const ResourceManagerGL&)            = delete;
  ResourceManagerGL& operator=(const ResourceManagerGL&) = delete;
  ResourceManagerGL(ResourceManagerGL&&)                 = delete;
  ResourceManagerGL& operator=(ResourceManagerGL&&)      = delete;

  /**
   * @brief Adds a 3D object to the resource manager.
   * @param object Reference of the Object3D to be added.
   */
  void addObject3D(const Object3D& object) override;

  /**
   * @brief Adds a directional light to the resource manager.
   * @param light Reference of the DirectionalLight to be added.
   */
  void addLight(const DirectionalLight& light) override;

  /**
   * @brief Adds a point light to the resource manager.
   * @param light Reference of the PointLight to be added.
   */
  void addLight(const PointLight& light) override;

  /**
   * @brief Adds a spot light to the resource manager.
   * @param light Reference of the SpotLight to be added.
   */
  void addLight(const SpotLight& light) override;

  /**
   * @brief Sets the skybox texture for the resource manager.
   * @param texture Reference of the Texture to be set as the skybox texture.
   */
  void setSkyboxTexture(const Texture& texture) override;

  /**
   * @brief Deletes the skybox texture.
   */
  void deleteSkyboxTexture() override;

  /**
   * @brief Gets the list of objects managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to ObjectGL.
   */
  const std::vector<std::unique_ptr<ObjectGL>>& getObjectList() const { return m_dataBuffers; }

  /**
   * @brief Gets the list of textures managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to TextureGL.
   */
  const std::vector<std::unique_ptr<TextureGL>>& getTextures() const { return m_textures; }

  /**
   * @brief Gets the pointer to the camera used for the viewport.
   * @return A pointer to the CameraGL object.
   */
  CameraGL* getCamera() const { return m_viewport_camera.get(); }

  /**
   * @brief Gets the pointer to the skybox texture.
   * @return A pointer to the TextureGL object representing the skybox texture.
   */
  const TextureGL* getSkyboxTexture() const { return m_skyboxTexture.get(); }

  /**
   * @brief Gets the list of directional lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to DirectionalLightGL.
   */
  const std::vector<std::unique_ptr<DirectionalLightGL>>& getDirectionalLights() const { return m_directionalLights; }

  /**
   * @brief Gets the list of point lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to PointLightGL.
   */
  const std::vector<std::unique_ptr<PointLightGL>>& getPointLights() const { return m_pointLights; }

  /**
   * @brief Gets the list of spot lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to SpotLightGL.
   */
  const std::vector<std::unique_ptr<SpotLightGL>>& getSpotLights() const { return m_spotLights; }

  /**
   * @brief Clears all resources managed by the resource manager.
   */
  void clearAllRessources() override;

  ~ResourceManagerGL() override; ///< Default destructor.
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_RESOURCEMANAGERGL_HPP