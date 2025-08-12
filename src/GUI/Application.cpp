// GCOVR_EXCL_START
#include <QApplication>
#include <QFile>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>

#include "Core/Color.hpp"
#include "GUI/Application.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"
#include "Geometry/SphereMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv), m_scene(std::make_unique<Scene>()),
      m_render_settings(std::make_unique<RenderSettings>()), m_texture_manager(std::make_unique<TextureManager>()),
      m_material_manager(std::make_unique<MaterialManager>()) {
  SetStylesheet();

  // createDefaultScene();
  createCornellBoxScene();
  // createSphereScene();
  // createRoughnessScene();

  // debugPBR();

  m_window = new MainWindow(m_scene.get(), m_texture_manager.get(), m_material_manager.get()); // NOLINT
  m_window->setAttribute(Qt::WA_QuitOnClose);
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
void Application::createDefaultScene() {
  const CubeMeshBuilder cube_mesh_builder(1.0);
  auto                  cube        = cube_mesh_builder.build();
  auto                  cube_object = std::make_unique<Object3D>(cube);
  m_scene->addObject("Default Cube", std::move(cube_object));

  auto directional_light = std::make_unique<DirectionalLight>();
  directional_light->setIntensity(1.0);
  directional_light->setDirection({0.5, -1.0, -2.0});
  m_scene->addLight("Directional Light", std::move(directional_light));

  m_scene->getCamera()->setPosition(linalg::Vec3d(-3.0, 1.5, 5.0));
  m_scene->getCamera()->setRotationDeg(linalg::Vec3d(-15.0, -30.0, 0.0));
  m_scene->setSkybox(TextureManager::DefaultSkyboxTexture());
}

void Application::createCornellBoxScene() {
  m_material_manager->addMaterial("Red");
  m_texture_manager->addTexture("RedDiffuse");
  m_texture_manager->getTexture("RedDiffuse")->setValue(ColorRGB(0.65, 0.05, 0.05));
  m_texture_manager->getTexture("RedDiffuse")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("Red")->setDiffuseTexture(m_texture_manager->getTexture("RedDiffuse"));

  m_material_manager->addMaterial("Green");
  m_texture_manager->addTexture("GreenDiffuse");
  m_texture_manager->getTexture("GreenDiffuse")->setValue(ColorRGB(0.12, 0.45, 0.15));
  m_texture_manager->getTexture("GreenDiffuse")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("Green")->setDiffuseTexture(m_texture_manager->getTexture("GreenDiffuse"));

  m_material_manager->addMaterial("White");
  m_texture_manager->addTexture("WhiteDiffuse");
  m_texture_manager->getTexture("WhiteDiffuse")->setValue(ColorRGB(0.73));
  m_texture_manager->getTexture("WhiteDiffuse")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("White")->setDiffuseTexture(m_texture_manager->getTexture("WhiteDiffuse"));
  m_material_manager->getMaterial("White")->setRoughnessValue(0.5);

  m_material_manager->addMaterial("Mirror");
  m_material_manager->getMaterial("Mirror")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Light");
  m_texture_manager->addTexture("LightEmissive");
  m_texture_manager->getTexture("LightEmissive")->setValue(ColorRGB(1.0, 1.0, 1.0));
  m_texture_manager->getTexture("LightEmissive")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("Light")->setEmissiveTexture(m_texture_manager->getTexture("LightEmissive"));
  m_material_manager->getMaterial("Light")->setEmissiveIntensity(15.0);

  const PlaneMeshBuilder plane_builder(555.0, 555.0);
  auto                   plane_mesh = plane_builder.build();

  auto floor_object = std::make_unique<Object3D>(plane_mesh);
  floor_object->setPosition({0, 0.0, 277.5});
  floor_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Floor", std::move(floor_object));

  auto ceiling_object = std::make_unique<Object3D>(plane_mesh);
  ceiling_object->setPosition({0, 555.0, 277.5});
  ceiling_object->setRotationDeg({180.0, 0.0, 0.0});
  ceiling_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Ceiling", std::move(ceiling_object));

  auto right_wall_object = std::make_unique<Object3D>(plane_mesh);
  right_wall_object->setPosition({-277.5, 277.5, 277.5});
  right_wall_object->setRotationDeg({0.0, 0.0, -90.0});
  right_wall_object->setMaterial(m_material_manager->getMaterial("Red"));
  m_scene->addObject("Right Wall", std::move(right_wall_object));

  auto left_wall_object = std::make_unique<Object3D>(plane_mesh);
  left_wall_object->setPosition({277.5, 277.5, 277.5});
  left_wall_object->setRotationDeg({0.0, 0.0, 90.0});
  left_wall_object->setMaterial(m_material_manager->getMaterial("Green"));
  m_scene->addObject("Left Wall", std::move(left_wall_object));

  auto back_wall_object = std::make_unique<Object3D>(plane_mesh);
  back_wall_object->setPosition({0.0, 277.5, 555.5});
  back_wall_object->setRotationDeg({-90.0, 0.0, 0.0});
  back_wall_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Back Wall", std::move(back_wall_object));

  const PlaneMeshBuilder light_plane_builder(130.0, 105.0);
  auto                   light_plane_mesh = light_plane_builder.build();

  auto light_object = std::make_unique<Object3D>(light_plane_mesh);
  light_object->setPosition({0, 554.0, 277.5});
  light_object->setRotationDeg({180.0, 0.0, 0.0});
  light_object->setMaterial(m_material_manager->getMaterial("Light"));
  m_scene->addObject("Light", std::move(light_object));

  const CubeMeshBuilder cube_mesh_builder(165.0);
  auto                  cube_mesh = cube_mesh_builder.build();

  auto right_cube_object = std::make_unique<Object3D>(cube_mesh);
  right_cube_object->setPosition({-92.5, 165, 327.5});
  right_cube_object->setRotationDeg({0.0, -18.0, 0.0});
  right_cube_object->setScaleY(2.0);
  right_cube_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Left Cube", std::move(right_cube_object));

  const SphereMeshBuilder sphere_mesh_builder(82.5, 128, 128);
  auto                    sphere_mesh = sphere_mesh_builder.build();

  auto left_sphere_object = std::make_unique<Object3D>(sphere_mesh);
  left_sphere_object->setPosition({92.5, 82.5, 152.5});
  left_sphere_object->setRotationDeg({0.0, 15.0, 0.0});
  left_sphere_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Right Cube", std::move(left_sphere_object));

  Camera* camera = m_scene->getCamera();
  camera->setPosition({0.0, 278.0, -800.0});
  camera->setRotationDeg({0.0, -180.0, 0.0});
  camera->setHorizontalFov(40.0);
  camera->setFarPlane(5000.0);
  camera->setAperture(22.0);
  camera->setFocusDistance(900.0);

  auto point_light = std::make_unique<PointLight>();
  point_light->setIntensity(100000.0);
  point_light->setPosition({0.0, 554.0, 277.5});
  m_scene->addLight("Point Light", std::move(point_light));
}

void Application::createSphereScene() {
  m_material_manager->addMaterial("Red");
  m_texture_manager->addTexture("RedDiffuse");
  m_texture_manager->getTexture("RedDiffuse")->setValue(ColorRGB(0.65, 0.05, 0.05));
  m_texture_manager->getTexture("RedDiffuse")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("Red")->setDiffuseTexture(m_texture_manager->getTexture("RedDiffuse"));
  m_material_manager->getMaterial("Red")->setRoughnessValue(0.2);
  m_material_manager->getMaterial("Red")->setMetallicValue(1.0);

  m_material_manager->addMaterial("White");
  m_texture_manager->addTexture("WhiteDiffuse");
  m_texture_manager->getTexture("WhiteDiffuse")->setValue(ColorRGB(0.73, 0.73, 0.73));
  m_texture_manager->getTexture("WhiteDiffuse")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("White")->setDiffuseTexture(m_texture_manager->getTexture("WhiteDiffuse"));
  m_material_manager->getMaterial("White")->setRoughnessValue(0.0);

  m_material_manager->addMaterial("Mirror");
  m_material_manager->getMaterial("Mirror")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Light");
  m_texture_manager->addTexture("LightEmissive");
  m_texture_manager->getTexture("LightEmissive")->setValue(ColorRGB(1.0, 1.0, 1.0));
  m_texture_manager->getTexture("LightEmissive")->setColorSpace(ColorSpace::S_RGB);
  m_material_manager->getMaterial("Light")->setEmissiveTexture(m_texture_manager->getTexture("LightEmissive"));
  m_material_manager->getMaterial("Light")->setEmissiveIntensity(1.0);

  const PlaneMeshBuilder plane_builder(2.5, 2.5);
  auto                   plane_mesh = plane_builder.build();

  auto floor_object = std::make_unique<Object3D>(plane_mesh);
  floor_object->setPosition({0, 0.0, 0.0});
  floor_object->setScale(4.0);
  floor_object->setMaterial(m_material_manager->getMaterial("White"));
  m_scene->addObject("Floor", std::move(floor_object));

  auto ceiling_light_object = std::make_unique<Object3D>(plane_mesh);
  ceiling_light_object->setPosition({0.0, 5.0, 0.0});
  ceiling_light_object->setScale(2.0);
  ceiling_light_object->setRotationDeg({180.0, 0.0, 0.0});
  ceiling_light_object->setMaterial(m_material_manager->getMaterial("Light"));
  m_scene->addObject("Ceiling Light", std::move(ceiling_light_object));

  const SphereMeshBuilder sphere_mesh_builder(1.0, 32, 16);
  auto                    sphere        = sphere_mesh_builder.build();
  auto                    sphere_object = std::make_unique<Object3D>(sphere);
  sphere_object->setPosition({0.0, 1.0, 0.0});
  sphere_object->setMaterial(m_material_manager->getMaterial("Red"));
  m_scene->addObject("Sphere", std::move(sphere_object));

  Camera* camera = m_scene->getCamera();
  camera->setPosition({0.0, 3.0, 15.0});
  camera->setRotationDeg({-10.0, 0.0, 0.0});
  camera->setHorizontalFov(40.0);
}

void Application::createRoughnessScene() {
  m_texture_manager->addTexture("HDR_Sky");
  m_texture_manager->getTexture("HDR_Sky")->loadFromFile("Resources/Textures/golden_gate_hills_4k.hdr");
  m_texture_manager->getTexture("HDR_Sky")->setColorSpace(ColorSpace::LINEAR);

  m_texture_manager->addTexture("Silver");
  m_texture_manager->getTexture("Silver")->setValue(ColorRGB(0.984, 0.946, 0.851));
  m_texture_manager->getTexture("Silver")->setColorSpace(ColorSpace::S_RGB);

  Texture* silver_texture = m_texture_manager->getTexture("Silver");

  m_material_manager->addMaterial("Silver0");
  m_material_manager->getMaterial("Silver0")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("Silver0")->setRoughnessValue(0.0);
  m_material_manager->getMaterial("Silver0")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Silver25");
  m_material_manager->getMaterial("Silver25")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("Silver25")->setRoughnessValue(0.25);
  m_material_manager->getMaterial("Silver25")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Silver50");
  m_material_manager->getMaterial("Silver50")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("Silver50")->setRoughnessValue(0.5);
  m_material_manager->getMaterial("Silver50")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Silver75");
  m_material_manager->getMaterial("Silver75")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("Silver75")->setRoughnessValue(0.75);
  m_material_manager->getMaterial("Silver75")->setMetallicValue(1.0);

  m_material_manager->addMaterial("Silver100");
  m_material_manager->getMaterial("Silver100")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("Silver100")->setRoughnessValue(1.0);
  m_material_manager->getMaterial("Silver100")->setMetallicValue(1.0);

  m_material_manager->addMaterial("White0");
  m_material_manager->getMaterial("White0")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("White0")->setRoughnessValue(0.0);
  m_material_manager->getMaterial("White0")->setMetallicValue(0.0);

  m_material_manager->addMaterial("White25");
  m_material_manager->getMaterial("White25")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("White25")->setRoughnessValue(0.25);
  m_material_manager->getMaterial("White25")->setMetallicValue(0.0);

  m_material_manager->addMaterial("White50");
  m_material_manager->getMaterial("White50")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("White50")->setRoughnessValue(0.5);
  m_material_manager->getMaterial("White50")->setMetallicValue(0.0);

  m_material_manager->addMaterial("White75");
  m_material_manager->getMaterial("White75")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("White75")->setRoughnessValue(0.75);
  m_material_manager->getMaterial("White75")->setMetallicValue(0.0);

  m_material_manager->addMaterial("White100");
  m_material_manager->getMaterial("White100")->setDiffuseTexture(silver_texture);
  m_material_manager->getMaterial("White100")->setRoughnessValue(1.0);
  m_material_manager->getMaterial("White100")->setMetallicValue(0.0);

  const PlaneMeshBuilder plane_builder(10.0, 10.0);
  auto                   plane_mesh = plane_builder.build();

  auto floor = std::make_unique<Object3D>(plane_mesh);
  floor->setPosition({0.0, 0.0, 0.0});
  // floor->setMaterial(m_material_manager->getMaterial("Silver0"));
  // m_scene->addObject("Floor", std::move(floor));

  const SphereMeshBuilder sphere_builder(0.5, 16, 8);
  auto                    sphere_mesh = sphere_builder.build();

  auto silver0 = std::make_unique<Object3D>(sphere_mesh);
  silver0->setPosition({-2.5, -1.0, 0.0});
  silver0->setMaterial(m_material_manager->getMaterial("Silver0"));
  m_scene->addObject("Sphere0", std::move(silver0));

  auto silver25 = std::make_unique<Object3D>(sphere_mesh);
  silver25->setPosition({-1.25, -1.0, 0.0});
  silver25->setMaterial(m_material_manager->getMaterial("Silver25"));
  m_scene->addObject("Sphere25", std::move(silver25));

  auto silver50 = std::make_unique<Object3D>(sphere_mesh);
  silver50->setPosition({0.0, -1.0, 0.0});
  silver50->setMaterial(m_material_manager->getMaterial("Silver50"));
  m_scene->addObject("Sphere50", std::move(silver50));

  auto silver75 = std::make_unique<Object3D>(sphere_mesh);
  silver75->setPosition({1.25, -1.0, 0.0});
  silver75->setMaterial(m_material_manager->getMaterial("Silver75"));
  m_scene->addObject("Sphere75", std::move(silver75));

  auto silver100 = std::make_unique<Object3D>(sphere_mesh);
  silver100->setPosition({2.5, -1.0, 0.0});
  silver100->setMaterial(m_material_manager->getMaterial("Silver100"));
  m_scene->addObject("Sphere100", std::move(silver100));

  auto white0 = std::make_unique<Object3D>(sphere_mesh);
  white0->setPosition({-2.5, 1.0, 0.0});
  white0->setMaterial(m_material_manager->getMaterial("White0"));
  m_scene->addObject("SphereWhite0", std::move(white0));

  auto white25 = std::make_unique<Object3D>(sphere_mesh);
  white25->setPosition({-1.25, 1.0, 0.0});
  white25->setMaterial(m_material_manager->getMaterial("White25"));
  m_scene->addObject("SphereWhite25", std::move(white25));

  auto white50 = std::make_unique<Object3D>(sphere_mesh);
  white50->setPosition({0.0, 1.0, 0.0});
  white50->setMaterial(m_material_manager->getMaterial("White50"));
  m_scene->addObject("SphereWhite50", std::move(white50));

  auto white75 = std::make_unique<Object3D>(sphere_mesh);
  white75->setPosition({1.25, 1.0, 0.0});
  white75->setMaterial(m_material_manager->getMaterial("White75"));
  m_scene->addObject("SphereWhite75", std::move(white75));

  auto white100 = std::make_unique<Object3D>(sphere_mesh);
  white100->setPosition({2.5, 1.0, 0.0});
  white100->setMaterial(m_material_manager->getMaterial("White100"));
  m_scene->addObject("SphereWhite100", std::move(white100));

  Camera* camera = m_scene->getCamera();
  camera->setPosition({0.0, 0.0, 8.0});
  camera->setRotationDeg({0.0, 0.0, 0.0});
  camera->setHorizontalFov(40.0);

  m_scene->setSkybox(m_texture_manager->getTexture("HDR_Sky"));
}

void Application::debugPBR() {
  RenderSettings render_settings;
  render_settings.setWidth(800);
  render_settings.setHeight(800);

  Renderer renderer(&render_settings);
  renderer.setScene(m_scene.get());
  m_scene->buildBVH();
  renderer.setupRayEmitterParameters();

  const PixelCoord pixel(120, 307);
  const double     dx = (1 / 800.0);
  const double     dy = (1 / 800.0);

  PixelCoord sub_pixel(1, 1);

  ColorRGB  color(0.0);
  const int iterations = 100; // 3x3 grid of sub-pixels
  for(int i = 0; i < iterations; ++i) {
    sub_pixel.x = i % static_cast<int>(std::sqrt(iterations));
    sub_pixel.y = i / static_cast<int>(std::sqrt(iterations));
  }

  convertToSRGBSpace(color.r);
  convertToSRGBSpace(color.g);
  convertToSRGBSpace(color.b);

  color *= 255.0; // Convert to 0-255 range

  std::cout << "Color at pixel (" << pixel.x << ", " << pixel.y << "): " << "R: " << color.r << ", G: " << color.g
            << ", B: " << color.b << '\n';
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

// GCOVR_EXCL_STOP
