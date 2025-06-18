/**
 * @file IResourceManagerGPU.hpp
 * @brief Header file for the IResourceManagerGPU interface, which manages GPU resources.
 */
#ifndef GPU_IRESOURCEMANAGERGPU_HPP
#define GPU_IRESOURCEMANAGERGPU_HPP

#include <memory>
#include <vector>

#include "GPU/ITextureGPU.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Scene/Skybox.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Texture.hpp"

/**
 * @class IResourceManagerGPU
 * @brief Interface for managing GPU resources such as textures, lights, and 3D objects.
 *
 * This interface defines methods for adding and managing various GPU resources,
 * including textures, lights, and 3D objects. It is designed to be implemented
 * by specific resource manager classes for different GPU backends.
 */
class IResourceManagerGPU {
private:
  virtual void setCamera(Camera* camera, int viewportWidth, int viewportHeight) = 0;

  virtual const ITextureGPU* addTexture(Texture* texture) = 0;

public:
  /**
   * @brief Default constructor for the resource manager interface.
   */
  IResourceManagerGPU() = default;

  IResourceManagerGPU(const IResourceManagerGPU&)            = delete;
  IResourceManagerGPU& operator=(const IResourceManagerGPU&) = delete;
  IResourceManagerGPU(IResourceManagerGPU&&)                 = delete;
  IResourceManagerGPU& operator=(IResourceManagerGPU&&)      = delete;

  /**
   * @brief Adds a 3D object to the resource manager.
   * @param object Reference to the Object3D to be added.
   */
  virtual void addObject3D(Object3D* object) = 0;

  /**
   * @brief Adds a directional light to the resource manager.
   * @param light Reference to the DirectionalLight to be added.
   */
  virtual void addLight(DirectionalLight* light) = 0;

  /**
   * @brief Adds a point light to the resource manager.
   * @param light Reference to the PointLight to be added.
   */
  virtual void addLight(PointLight* light) = 0;

  /**
   * @brief Adds a spot light to the resource manager.
   * @param light Reference to the SpotLight to be added.
   */
  virtual void addLight(SpotLight* light) = 0;

  /**
   * @brief Sets the skybox texture for the resource manager.
   * @param texture Reference to the Texture to be set as the skybox texture.
   */
  virtual void setSkybox(Skybox* texture) = 0;

  virtual void clearAllRessources() = 0; ///< Clears all resources managed by the resource manager.

  virtual ~IResourceManagerGPU() = default; ///< Virtual destructor for the resource manager interface.
};

#endif // GPU_IRESOURCEMANAGERGPU_HPP