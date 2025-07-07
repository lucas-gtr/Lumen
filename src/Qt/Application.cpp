#include <memory>
#include <qapplication.h>
#include <qfile.h>
#include <qnamespace.h>
#include <qobject.h>
#include <utility>

#include "Geometry/CubeMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Qt/Application.hpp"
#include "Qt/MainWindow.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
Application::Application(int& argc, char** argv)
    : QApplication(argc, argv), m_scene(std::make_unique<Scene>()),
      m_render_settings(std::make_unique<RenderSettings>()), m_texture_manager(std::make_unique<TextureManager>()),
      m_material_manager(std::make_unique<MaterialManager>()),
      m_window(
          new MainWindow(m_scene.get(), m_texture_manager.get(), m_material_manager.get(), m_render_settings.get())) {

  m_window->setAttribute(Qt::WA_QuitOnClose);
  SetStylesheet();
  createDefaultScene();
}
// NOLINTEND(cppcoreguidelines-owning-memory)

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
void Application::createDefaultScene() {
  auto cube_mesh_builder = std::make_shared<CubeMeshBuilder>(1.0);
  auto cube            = cube_mesh_builder->build();
  auto cube_object      = std::make_unique<Object3D>(cube);
  m_scene->addObject("Default Cube", std::move(cube_object));

  auto directional_light = std::make_unique<DirectionalLight>();
  directional_light->setIntensity(1.0);
  directional_light->setDirection({0.5, -1.0, -2.0});
  m_scene->addLight("Directional Light", std::move(directional_light));

  m_scene->getCamera()->setPosition(linalg::Vec3d(-3.0, 1.5, 5.0));
  m_scene->getCamera()->setRotation(linalg::Vec3d(-15.0, -30.0, 0.0));
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

void Application::SetStylesheet() { setStyle("Fusion"); }

int Application::run() {
  m_window->show();
  return exec();
}
