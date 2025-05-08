#include <cstddef>
#include <memory>
#include <vector>

#include "BVH/BVHBuilder.hpp"
#include "BVH/BVHNode.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Skybox.hpp"

Scene::Scene() : m_current_camera(nullptr), m_skybox(std::make_unique<Skybox>()) {}

void Scene::buildBVH() {
  std::vector<std::shared_ptr<BVHNode>> bvh_leaf_list;
  for(size_t i = 0; i < m_object_list.size(); ++i) {
    m_object_list[i]->getMesh().buildBVH();
    bvh_leaf_list.push_back(
        std::make_shared<BVHNode>(m_object_list[i]->getMinBound(), m_object_list[i]->getMaxBound(), i));
  }
  BVH::constructNode(m_bvh_root, bvh_leaf_list, 0, static_cast<int>(bvh_leaf_list.size()));
}