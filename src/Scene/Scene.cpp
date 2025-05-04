#include "Scene/Scene.hpp"
#include "Scene/Skybox.hpp"

#include <memory>

Scene::Scene() : m_current_camera(nullptr), m_skybox(std::make_unique<Skybox>()) {}