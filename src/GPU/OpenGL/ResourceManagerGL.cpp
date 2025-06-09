// GCOVR_EXCL_START
#include "GPU/OpenGL/ResourceManagerGL.hpp"
#include "GPU/OpenGL/CameraGL.hpp"
#include "GPU/OpenGL/Lights/DirectionalGL.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "GPU/OpenGL/Lights/SpotGL.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/Light.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

#include <iostream>
#include <memory>
#include <utility>

ResourceManagerGL::ResourceManagerGL(int viewportWidth, int viewportHeight, const Scene& scene) {
  loadScene(viewportWidth, viewportHeight, scene);
}

void ResourceManagerGL::loadScene(int viewportWidth, int viewportHeight, const Scene& scene) {
  if(scene.getCamera() != nullptr) {
    ResourceManagerGL::setCamera(scene.getCamera(), viewportWidth, viewportHeight);
  }

  if(scene.getSkybox() != nullptr && scene.getSkybox()->getTexture() != nullptr) {
    ResourceManagerGL::setSkyboxTexture(*scene.getSkybox()->getTexture());
  }

  for(const auto& object : scene.getObjectList()) {
    ResourceManagerGL::addObject3D(*object);
  }

  for(const auto& light : scene.getLightList()) {
    switch(light->getType()) {
    case LightType::Directional:
      if(const auto* directional = dynamic_cast<const DirectionalLight*>(light.get())) {
        ResourceManagerGL::addLight(*directional);
      } else {
        std::cerr << "Failed to cast to DirectionalLight\n";
      }
      break;
    case LightType::Point:
      if(const auto* point = dynamic_cast<const PointLight*>(light.get())) {
        ResourceManagerGL::addLight(*point);
      } else {
        std::cerr << "Failed to cast to PointLight\n";
      }
      break;
    case LightType::Spot:
      if(const auto* spot = dynamic_cast<const SpotLight*>(light.get())) {
        ResourceManagerGL::addLight(*spot);
      } else {
        std::cerr << "Failed to cast to SpotLight\n";
      }
      break;
    default:
      std::cerr << "Unknown light type encountered in OpenGLResourceManager initialization.\n";
      break;
    }
  }
}

void ResourceManagerGL::addObject3D(const Object3D& object) {
  const auto&       material    = object.getMaterial();
  const MaterialGL* gl_material = addMaterial(material);

  auto dataBuffer = std::make_unique<ObjectGL>(object, gl_material);
  dataBuffer->uploadToGPU();
  m_dataBuffers.push_back(std::move(dataBuffer));
}

void ResourceManagerGL::setCamera(Camera* camera, int viewportWidth, int viewportHeight) {
  m_viewport_camera = std::make_unique<CameraGL>(*camera, viewportWidth, viewportHeight);
}

const TextureGL* ResourceManagerGL::addTexture(const Texture& texture) {
  for(const auto& existingTexture : m_textures) {
    if(existingTexture->getSource() == &texture) {
      return existingTexture.get();
    }
  }

  auto newTexture = std::make_unique<TextureGL>(texture);
  newTexture->uploadToGPU();
  m_textures.push_back(std::move(newTexture));
  return m_textures.back().get();
}

const MaterialGL* ResourceManagerGL::addMaterial(const Material& material) {
  for(const auto& existingMaterial : m_materials) {
    if(existingMaterial->getSource() == &material) {
      return existingMaterial.get();
    }
  }

  const TextureGL* albedoTexture = ResourceManagerGL::addTexture(material.getAlbedoTexture());
  const TextureGL* normalTexture = ResourceManagerGL::addTexture(material.getNormalTexture());

  auto newMaterial = std::make_unique<MaterialGL>(material, albedoTexture, normalTexture);
  m_materials.push_back(std::move(newMaterial));
  return m_materials.back().get();
}

void ResourceManagerGL::addLight(const DirectionalLight& light) {
  m_directionalLights.push_back(std::make_unique<DirectionalLightGL>(light));
}

void ResourceManagerGL::addLight(const PointLight& light) {
  m_pointLights.push_back(std::make_unique<PointLightGL>(light));
}

void ResourceManagerGL::addLight(const SpotLight& light) {
  m_spotLights.push_back(std::make_unique<SpotLightGL>(light));
}

void ResourceManagerGL::setSkyboxTexture(const Texture& texture) {
  m_skyboxTexture = std::make_unique<TextureGL>(texture);
  m_skyboxTexture->uploadToGPU();
}

void ResourceManagerGL::deleteSkyboxTexture() { m_skyboxTexture = nullptr; }

void ResourceManagerGL::clearAllRessources() {
  m_dataBuffers.clear();
  m_textures.clear();
  m_materials.clear();
  m_directionalLights.clear();
  m_pointLights.clear();
  m_spotLights.clear();
  m_skyboxTexture.reset();
  m_viewport_camera.reset();
}

ResourceManagerGL::~ResourceManagerGL() { ResourceManagerGL::clearAllRessources(); }

// GCOVR_EXCL_STOP