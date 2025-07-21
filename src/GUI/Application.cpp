#include <QApplication>
#include <QFile>
#include <memory>
#include <utility>

#include "GUI/Application.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv), m_scene(std::make_unique<Scene>()),
      m_render_settings(std::make_unique<RenderSettings>()), m_texture_manager(std::make_unique<TextureManager>()),
      m_material_manager(std::make_unique<MaterialManager>()) {
  SetStylesheet();
  createDefaultScene();
  m_window = new MainWindow(m_scene.get(), m_texture_manager.get(), m_material_manager.get()); // NOLINT
  m_window->setAttribute(Qt::WA_QuitOnClose);
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
void Application::createDefaultScene() {
  auto cube_mesh_builder = std::make_shared<CubeMeshBuilder>(1.0);
  auto cube              = cube_mesh_builder->build();
  auto cube_object       = std::make_unique<Object3D>(cube);
  m_scene->addObject("Default Cube", std::move(cube_object));

  auto directional_light = std::make_unique<DirectionalLight>();
  directional_light->setIntensity(1.0);
  directional_light->setDirection({0.5, -1.0, -2.0});
  m_scene->addLight("Directional Light", std::move(directional_light));

  m_scene->getCamera()->setPosition(linalg::Vec3d(-3.0, 1.5, 5.0));
  m_scene->getCamera()->setRotationDeg(linalg::Vec3d(-15.0, -30.0, 0.0));
  m_scene->setSkybox(TextureManager::DefaultSkyboxTexture());
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

void Application::SetStylesheet() { setStyle("Fusion"); }

int Application::run() {
  m_window->show();
  return exec();
}

Application::~Application() {
  if(m_window != nullptr) {
    m_window->close();
    delete m_window;
  }
}
