#include <cstddef>
#include <linalg/Mat4.hpp>
#include <linalg/linalg.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "BVH/BVHBuilder.hpp"
#include "BVH/BVHNode.hpp"
#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "Core/Random.hpp"
#include "Geometry/Mesh.hpp"
#include "Lighting/Light.hpp"
#include "Scene/LightSample.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Skybox.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"

Scene::Scene() : m_current_camera(std::make_unique<Camera>()), m_skybox(std::make_unique<Skybox>()) {}

std::string Scene::getAvailableObjectName(const std::string& name) const {
  if(m_object_map.find(name) == m_object_map.end()) {
    return name;
  }
  size_t      index    = 1;
  std::string new_name = name + "_" + std::to_string(index);
  while(m_object_map.find(new_name) != m_object_map.end()) {
    ++index;
    new_name = name + "_" + std::to_string(index);
  }
  return new_name;
}

std::string Scene::getAvailableLightName(const std::string& name) const {
  if(m_light_map.find(name) == m_light_map.end()) {
    return name;
  }
  size_t      index    = 1;
  std::string new_name = name + "_" + std::to_string(index);
  while(m_light_map.find(new_name) != m_light_map.end()) {
    ++index;
    new_name = name + "_" + std::to_string(index);
  }
  return new_name;
}

void Scene::addObject(const std::string& name, std::unique_ptr<Object3D> object) {
  Object3D* ptr = object.get();
  m_object_index.push_back(ptr);
  if(m_object_map.find(name) != m_object_map.end()) {
    throw std::runtime_error(
        "Object with name '" + name +
        "' already exists in the scene. You must check for existing names before adding a new object.");
  }
  m_object_map.emplace(name, std::move(object));

  m_object_added_observer.notify(ptr);
}

bool Scene::renameObject(const std::string& old_name, const std::string& new_name) {
  if(m_object_map.find(new_name) != m_object_map.end()) {
    return false; // Name already in use
  }
  auto it = m_object_map.find(old_name);
  if(it != m_object_map.end()) {
    auto object = std::move(it->second);
    m_object_map.erase(it);
    m_object_map.emplace(new_name, std::move(object));
    return true; // Rename successful
  }
  return false; // Old name not found
}

void Scene::removeObject(const std::string& name) {
  auto it = m_object_map.find(name);
  if(it != m_object_map.end()) {
    Object3D* object = it->second.get();
    m_object_index.erase(std::remove(m_object_index.begin(), m_object_index.end(), object), m_object_index.end());
    m_object_map.erase(it);
  }
}

std::string Scene::getObjectName(const Object3D* object) const {
  for(const auto& pair : m_object_map) {
    if(pair.second.get() == object) {
      return pair.first;
    }
  }
  return "";
}

Object3D* Scene::getObject(const std::string& name) const {
  auto it = m_object_map.find(name);
  if(it != m_object_map.end()) {
    return it->second.get();
  }
  return nullptr;
}

void Scene::addLight(const std::string& name, std::unique_ptr<Light> light) {
  Light* ptr = light.get();
  m_light_index.push_back(ptr);
  if(m_light_map.find(name) != m_light_map.end()) {
    throw std::runtime_error(
        "Light with name '" + name +
        "' already exists in the scene. You must check for existing names before adding a new light.");
  }
  m_light_map.emplace(name, std::move(light));

  m_light_added_observer.notify(ptr);
}

bool Scene::renameLight(const std::string& old_name, const std::string& new_name) {
  if(m_light_map.find(new_name) != m_light_map.end()) {
    return false; // Name already in use
  }
  auto it = m_light_map.find(old_name);
  if(it != m_light_map.end()) {
    auto light = std::move(it->second);
    m_light_map.erase(it);
    m_light_map.emplace(new_name, std::move(light));
    return true; // Rename successful
  }
  return false; // Old name not found
}

void Scene::removeLight(const std::string& name) {
  auto it = m_light_map.find(name);
  if(it != m_light_map.end()) {
    Light* light = it->second.get();
    m_light_index.erase(std::remove(m_light_index.begin(), m_light_index.end(), light), m_light_index.end());
    m_light_map.erase(it);
  }
}

Light* Scene::getLight(const std::string& name) const {
  auto it = m_light_map.find(name);
  if(it != m_light_map.end()) {
    return it->second.get();
  }
  return nullptr;
}

std::string Scene::getLightName(const Light* light) const {
  for(const auto& pair : m_light_map) {
    if(pair.second.get() == light) {
      return pair.first;
    }
  }
  return "";
}

void Scene::addLightSample(const Object3D& object, double intensity) {
  const Mesh&          mesh      = object.getMesh();
  const linalg::Mat4d& transform = object.getTransformationMatrix();
  for(const auto& face : mesh.getFaces()) {
    LightSample sample;
    sample.v1 = toVec3(transform * toVec4(mesh.getVertex(face.vertex_indices[0]).position));
    sample.v2 = toVec3(transform * toVec4(mesh.getVertex(face.vertex_indices[1]).position));
    sample.v3 = toVec3(transform * toVec4(mesh.getVertex(face.vertex_indices[2]).position));

    const linalg::Vec3d edge1 = sample.v2 - sample.v1;
    const linalg::Vec3d edge2 = sample.v3 - sample.v1;
    sample.area               = HALF * edge1.cross(edge2).length();
    sample.intensity          = intensity;
    m_light_samples.push_back(sample);
  }
}

const LightSample* Scene::getLightSample() const {
  if(m_light_samples.empty()) {
    return nullptr;
  }
  const double random_value = randomUniform01();
  const int    index        = static_cast<int>(random_value * static_cast<double>(m_light_samples.size()));

  return &m_light_samples[index];
}

int Scene::getLightSampleCount() const { return static_cast<int>(m_light_samples.size()); }

void Scene::buildBVH() {
  std::vector<std::shared_ptr<BVHNode>> bvh_leaf_list;
  m_light_samples.clear();
  for(size_t i = 0; i < m_object_index.size(); ++i) {
    const double emissive_intensity = m_object_index[i]->getMaterial()->getEmissiveIntensity();
    if(emissive_intensity > 0.0) {
      addLightSample(*m_object_index[i], m_object_index[i]->getMaterial()->getEmissiveIntensity());
    }
    m_object_index[i]->getMesh().buildBVH();
    bvh_leaf_list.push_back(
        std::make_shared<BVHNode>(m_object_index[i]->getMinBound(), m_object_index[i]->getMaxBound(), i));
  }
  BVH::constructNode(m_bvh_root, bvh_leaf_list, 0, static_cast<int>(bvh_leaf_list.size()));
}