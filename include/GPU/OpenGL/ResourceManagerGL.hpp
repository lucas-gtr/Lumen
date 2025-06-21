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
#include "GPU/OpenGL/SkyboxGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Qt/OpenGLContext.hpp"
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
  std::vector<std::unique_ptr<ObjectGL>>   m_object_list;
  std::vector<std::unique_ptr<TextureGL>>  m_textures;
  std::vector<std::unique_ptr<MaterialGL>> m_materials;
  std::unique_ptr<SkyboxGL>                m_skybox;

  std::unique_ptr<CameraGL> m_viewport_camera;

  std::vector<std::unique_ptr<DirectionalLightGL>> m_directional_lights;
  std::vector<std::unique_ptr<PointLightGL>>       m_point_lights;
  std::vector<std::unique_ptr<SpotLightGL>>        m_spot_lights;

  void loadScene(int viewport_width, int viewport_height, Scene* scene);

  TextureGL*  addTexture(Texture* texture) override;
  MaterialGL* addMaterial(Material* material);

  void setCamera(Camera* camera, int viewport_width, int viewport_height) override;
  void setSkybox(Skybox* skybox) override;

  void onObjectMaterialChanged(const Object3D* object);
  void onMaterialTextureChanged(const Material* material);

public:
  /**
   * @brief Constructor for ResourceManagerGL.
   * @param viewport_width The width of the viewport in pixels.
   * @param viewport_height The height of the viewport in pixels.
   * @param scene The Scene object containing the initial objects and lights.
   */
  ResourceManagerGL(int viewport_width, int viewport_height, Scene* scene);

  ResourceManagerGL(const ResourceManagerGL&)            = delete;
  ResourceManagerGL& operator=(const ResourceManagerGL&) = delete;
  ResourceManagerGL(ResourceManagerGL&&)                 = delete;
  ResourceManagerGL& operator=(ResourceManagerGL&&)      = delete;

  /**
   * @brief Adds a 3D object to the resource manager.
   * @param object Reference of the Object3D to be added.
   */
  void addObject3D(Object3D* object) override;

  /**
   * @brief Adds a light to the resource manager.
   * @param light Pointer to the Light object to be added.
   */
  void addLight(Light* light);

  /**
   * @brief Adds a directional light to the resource manager.
   * @param light Reference of the DirectionalLight to be added.
   */
  void addLight(DirectionalLight* light) override;

  /**
   * @brief Adds a point light to the resource manager.
   * @param light Reference of the PointLight to be added.
   */
  void addLight(PointLight* light) override;

  /**
   * @brief Adds a spot light to the resource manager.
   * @param light Reference of the SpotLight to be added.
   */
  void addLight(SpotLight* light) override;

  /**
   * @brief Sets the selected object for the viewport.
   * @param object Pointer to the Object3D to be set as selected.
   */
  void setSelectedObject(Object3D* object);

  /**
   * @brief Gets the list of objects managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to ObjectGL.
   */
  const std::vector<std::unique_ptr<ObjectGL>>& getObjectList() const { return m_object_list; }

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
  TextureGL* getSkyboxTexture() const { return m_skybox->getTexture(); }

  /**
   * @brief Gets the list of directional lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to DirectionalLightGL.
   */
  const std::vector<std::unique_ptr<DirectionalLightGL>>& getDirectionalLights() const { return m_directional_lights; }

  /**
   * @brief Gets the list of point lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to PointLightGL.
   */
  const std::vector<std::unique_ptr<PointLightGL>>& getPointLights() const { return m_point_lights; }

  /**
   * @brief Gets the list of spot lights managed by the resource manager.
   * @return A constant reference to the vector of unique pointers to SpotLightGL.
   */
  const std::vector<std::unique_ptr<SpotLightGL>>& getSpotLights() const { return m_spot_lights; }

  /**
   * @brief Clears all resources managed by the resource manager.
   */
  void clearAllRessources() override;

  ~ResourceManagerGL() override; ///< Default destructor.
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_RESOURCEMANAGERGL_HPP