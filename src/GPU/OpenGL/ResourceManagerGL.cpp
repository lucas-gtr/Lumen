// GCOVR_EXCL_START
#include <iostream>
#include <memory>
#include <utility>

#include "GPU/OpenGL/CameraGL.hpp"
#include "GPU/OpenGL/Lights/DirectionalGL.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "GPU/OpenGL/Lights/SpotGL.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "GPU/OpenGL/ResourceManagerGL.hpp"
#include "GPU/OpenGL/SkyboxGL.hpp"
#include "GPU/OpenGL/TextureGL.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/Light.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Qt/OpenGLContext.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"

ResourceManagerGL::ResourceManagerGL(int viewport_width, int viewport_height, Scene* scene) {
  loadScene(viewport_width, viewport_height, scene);

  scene->getObjectAddedObserver().add([this](Object3D* object) { this->addObject3D(object); });
  scene->getLightAddedObserver().add([this](Light* light) { this->addLight(light); });
}

void ResourceManagerGL::loadScene(int viewport_width, int viewport_height, Scene* scene) {
  if(scene->getCamera() != nullptr) {
    ResourceManagerGL::setCamera(scene->getCamera(), viewport_width, viewport_height);
  }

  if(scene->getSkybox() != nullptr && scene->getSkybox()->getTexture() != nullptr) {
    ResourceManagerGL::setSkybox(scene->getSkybox());
  }

  for(const auto& object : scene->getObjectList()) {
    ResourceManagerGL::addObject3D(object);
  }

  for(const auto& light : scene->getLightList()) {
    ResourceManagerGL::addLight(light);
  }
}

void ResourceManagerGL::addObject3D(Object3D* object) {
  Material*   material    = object->getMaterial();
  MaterialGL* gl_material = addMaterial(material);

  OpenGLContext::instance().makeContextCurrent();
  auto object_gl = std::make_unique<ObjectGL>(object, gl_material);
  object_gl->uploadToGPU();
  m_object_list.push_back(std::move(object_gl));
  OpenGLContext::instance().doneContext();

  object->getMaterialChangedObserver().add([this](const Object3D* obj) { this->onObjectMaterialChanged(obj); });
}

void ResourceManagerGL::setCamera(Camera* camera, int viewport_width, int viewport_height) {
  m_viewport_camera = std::make_unique<CameraGL>(*camera, viewport_width, viewport_height);
}

void ResourceManagerGL::setSkybox(Skybox* skybox) { m_skybox = std::make_unique<SkyboxGL>(skybox); }

TextureGL* ResourceManagerGL::addTexture(Texture* texture) {
  for(const auto& existing_texture : m_textures) {
    if(existing_texture->getSource() == texture) {
      return existing_texture.get();
    }
  }

  OpenGLContext::instance().makeContextCurrent();
  auto new_texture = std::make_unique<TextureGL>(texture);
  new_texture->uploadToGPU();
  m_textures.push_back(std::move(new_texture));
  OpenGLContext::instance().doneContext();
  return m_textures.back().get();
}

MaterialGL* ResourceManagerGL::addMaterial(Material* material) {
  for(const auto& existing_material : m_materials) {
    if(existing_material->getSource() == material) {
      return existing_material.get();
    }
  }

  TextureGL* diffuse_texture = ResourceManagerGL::addTexture(material->getDiffuseTexture());
  TextureGL* normal_texture  = ResourceManagerGL::addTexture(material->getNormalTexture());

  OpenGLContext::instance().makeContextCurrent();
  auto new_material = std::make_unique<MaterialGL>(material, diffuse_texture, normal_texture);
  m_materials.push_back(std::move(new_material));
  material->getMaterialChangedObserver().add(
      [this](const Material* material) { this->onMaterialTextureChanged(material); });
  OpenGLContext::instance().doneContext();
  return m_materials.back().get();
}

void ResourceManagerGL::addLight(Light* light) {
  switch(light->getType()) {
  case LightType::Directional:
    if(auto* directional = dynamic_cast<DirectionalLight*>(light)) {
      ResourceManagerGL::addLight(directional);
    } else {
      std::cerr << "Failed to cast to DirectionalLight\n";
    }
    break;
  case LightType::Point:
    if(auto* point = dynamic_cast<PointLight*>(light)) {
      ResourceManagerGL::addLight(point);
    } else {
      std::cerr << "Failed to cast to PointLight\n";
    }
    break;
  case LightType::Spot:
    if(auto* spot = dynamic_cast<SpotLight*>(light)) {
      ResourceManagerGL::addLight(spot);
    } else {
      std::cerr << "Failed to cast to SpotLight\n";
    }
    break;
  default:
    std::cerr << "Unknown light type encountered in OpenGLResourceManager initialization.\n";
    break;
  }
}

void ResourceManagerGL::addLight(DirectionalLight* light) {
  m_directional_lights.push_back(std::make_unique<DirectionalLightGL>(light));
}

void ResourceManagerGL::addLight(PointLight* light) { m_point_lights.push_back(std::make_unique<PointLightGL>(light)); }

void ResourceManagerGL::addLight(SpotLight* light) { m_spot_lights.push_back(std::make_unique<SpotLightGL>(light)); }

void ResourceManagerGL::setSelectedObject(Object3D* object) {
  for(auto& object_gl : m_object_list) {
    if(object_gl->getSource() == object) {
      object_gl->setSelected(true);
    } else {
      object_gl->setSelected(false);
    }
  }
}

void ResourceManagerGL::onObjectMaterialChanged(const Object3D* object) {
  for(auto& object_gl : m_object_list) {
    if(object_gl->getSource() == object) {
      Material*   material    = object->getMaterial();
      MaterialGL* gl_material = addMaterial(material);
      object_gl->setMaterial(gl_material);
      return;
    }
  }
}

void ResourceManagerGL::onMaterialTextureChanged(const Material* material) {
  for(auto& gl_material : m_materials) {
    if(gl_material->getSource() == material) {
      TextureGL* diffuse_texture = ResourceManagerGL::addTexture(material->getDiffuseTexture());
      gl_material->setDiffuseTexture(diffuse_texture);

      TextureGL* normal_texture = ResourceManagerGL::addTexture(material->getNormalTexture());
      gl_material->setNormalTexture(normal_texture);
    }
  }
}

void ResourceManagerGL::clearAllRessources() {
  m_object_list.clear();
  m_textures.clear();
  m_materials.clear();
  m_directional_lights.clear();
  m_point_lights.clear();
  m_spot_lights.clear();
  m_viewport_camera.reset();
}

ResourceManagerGL::~ResourceManagerGL() { ResourceManagerGL::clearAllRessources(); }

// GCOVR_EXCL_STOP
