#include "Export/OutputFormatPng.hpp"
#include "Export/RenderExporter.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/OBJLoader.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"
#include "Geometry/SphereMeshBuilder.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Camera.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureLoader.hpp"

#include <climits>
#include <iostream>
#include <vector>

void renderCubeExemple() {
  CubeMeshBuilder cube_builder(1.0);
  Mesh            cube_mesh   = cube_builder.build();
  auto            cube_object = std::make_unique<Object3D>(cube_mesh);
  cube_object->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));

  auto texture  = std::make_shared<Texture>(Eigen::Vector3d(1.0, 1.0, 1.0));
  auto material = std::make_shared<Material>();
  material->setAlbedoTexture(texture);
  cube_object->setMaterial(material);

  std::unique_ptr<DirectionalLight> directional_light = std::make_unique<DirectionalLight>();
  directional_light->setColor(Eigen::Vector3d(1.0, 1.0, 1.0));
  directional_light->setIntensity(1);
  directional_light->setDirection(Eigen::Vector3d(-3.0, -1.0, -2.0));

  std::unique_ptr<PointLight> point_light_red = std::make_unique<PointLight>();
  point_light_red->setColor(Eigen::Vector3d(1.0, 0.0, 0.0));
  point_light_red->setIntensity(1.0);
  point_light_red->setPosition(Eigen::Vector3d(0.2, 1.5, -1.0));

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setPosition(Eigen::Vector3d(-1.1, 1.1, 3.0));
  camera->setRotation(Eigen::Vector3d(-20.0, -20.0, 0.0));
  camera->setAperture(22.0);

  Scene scene;
  scene.addLight(std::move(directional_light));
  scene.addLight(std::move(point_light_red));
  scene.addObject(std::move(cube_object));
  scene.setCamera(std::move(camera));

  RenderSettings render_settings;
  render_settings.setWidth(800);
  render_settings.setHeight(600);
  render_settings.setChannelCount(3);
  render_settings.setSamplesPerPixel(4);

  Renderer renderer(&render_settings, &scene);
  renderer.renderFrame();

  std::unique_ptr<OutputFormat> output_format = std::make_unique<OutputFormatPng>();

  RenderExporter exporter(renderer.getFramebuffer());
  exporter.setOutputFormat(std::move(output_format));
  exporter.setPath("Gallery");
  exporter.setFilename("CubeExample");
  exporter.setToneMapping(ToneMapping::NONE);

  exporter.exportRender();
}

void renderPlaneExemple() {
  PlaneMeshBuilder plane_builder(8.0, 8.0);
  Mesh             plane_mesh   = plane_builder.build();
  auto             plane_object = std::make_unique<Object3D>(plane_mesh);
  plane_object->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));

  auto rock_albedo = TextureLoader::load("Resources/Textures/Rock/RockAlbedo.png");
  rock_albedo->setColorSpace(ColorSpace::RGB);
  auto rock_normal = TextureLoader::load("Resources/Textures/Rock/RockNormal.png");
  auto material    = std::make_shared<Material>();
  material->setAlbedoTexture(rock_albedo);
  material->setNormalTexture(rock_normal);
  plane_object->setMaterial(material);

  std::unique_ptr<DirectionalLight> directional_light = std::make_unique<DirectionalLight>();
  directional_light->setColor(Eigen::Vector3d(1.0, 1.0, 1.0));
  directional_light->setIntensity(1.0);
  directional_light->setDirection(Eigen::Vector3d(0.0, -1.0, 0.0));

  std::unique_ptr<SpotLight> spot_light_red = std::make_unique<SpotLight>();
  spot_light_red->setColor(Eigen::Vector3d(1.0, 0.0, 0.0));
  spot_light_red->setIntensity(10.0);
  spot_light_red->setPosition(Eigen::Vector3d(-2.5, 1.5, -2.5));
  spot_light_red->setDirection(Eigen::Vector3d(0.0, -1.0, 0.0));
  spot_light_red->setInnerAngle(5.0);
  spot_light_red->setOuterAngle(60.0);

  std::unique_ptr<SpotLight> spot_light_green = std::make_unique<SpotLight>();
  spot_light_green->setColor(Eigen::Vector3d(0.0, 1.0, 0.0));
  spot_light_green->setIntensity(10.0);
  spot_light_green->setPosition(Eigen::Vector3d(0.0, 1.5, 0.0));
  spot_light_green->setDirection(Eigen::Vector3d(0.0, -1.0, 0.0));
  spot_light_green->setInnerAngle(5.0);
  spot_light_green->setOuterAngle(60.0);

  std::unique_ptr<SpotLight> spot_light_blue = std::make_unique<SpotLight>();
  spot_light_blue->setColor(Eigen::Vector3d(0.0, 0.0, 1.0));
  spot_light_blue->setIntensity(20.0);
  spot_light_blue->setPosition(Eigen::Vector3d(2.5, 1.5, 2.5));
  spot_light_blue->setDirection(Eigen::Vector3d(0.0, -1.0, 0.0));
  spot_light_blue->setInnerAngle(5.0);
  spot_light_blue->setOuterAngle(60.0);

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setPosition(Eigen::Vector3d(0.0, 6.0, 12.0));
  camera->setRotation(Eigen::Vector3d(-30.0, 0.0, 0.0));
  camera->setAperture(22.0);

  Scene scene;
  scene.addLight(std::move(directional_light));
  scene.addLight(std::move(spot_light_red));
  scene.addLight(std::move(spot_light_green));
  scene.addLight(std::move(spot_light_blue));
  scene.addObject(std::move(plane_object));
  scene.setCamera(std::move(camera));

  RenderSettings render_settings;
  render_settings.setWidth(800);
  render_settings.setHeight(600);
  render_settings.setChannelCount(3);
  render_settings.setSamplesPerPixel(4);

  Renderer renderer(&render_settings, &scene);
  renderer.renderFrame();

  std::unique_ptr<OutputFormat> output_format = std::make_unique<OutputFormatPng>();

  RenderExporter exporter(renderer.getFramebuffer());
  exporter.setOutputFormat(std::move(output_format));
  exporter.setPath("Gallery");
  exporter.setFilename("PlaneExampleExposure-1");
  exporter.setToneMapping(ToneMapping::EXPOSURE);
  exporter.setParameter("exposure", 1.0);

  exporter.exportRender();
}

void renderSphereExemple() {
  SphereMeshBuilder sphere_builder(1.0, 16, 32);
  Mesh              sphere_mesh   = sphere_builder.build();
  auto              sphere_object = std::make_unique<Object3D>(sphere_mesh);
  sphere_object->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));

  auto brick_albedo = TextureLoader::load("Resources/Textures/BrickWall/sloppy-brick-wall_albedo.png");
  brick_albedo->setColorSpace(ColorSpace::RGB);
  auto brick_normal = TextureLoader::load("Resources/Textures/BrickWall/sloppy-brick-wall_normal-ogl.png");
  auto material     = std::make_shared<Material>();
  material->setAlbedoTexture(brick_albedo);
  material->setNormalTexture(brick_normal);
  sphere_object->setMaterial(material);

  std::unique_ptr<DirectionalLight> directional_light = std::make_unique<DirectionalLight>();
  directional_light->setColor(Eigen::Vector3d(1.0, 1.0, 1.0));
  directional_light->setIntensity(1.0);
  directional_light->setDirection(Eigen::Vector3d(1.0, 0.0, -2.0));

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setPosition(Eigen::Vector3d(0.0, 2.5, 8.0));
  camera->setRotation(Eigen::Vector3d(-20.0, 0.0, 0.0));
  camera->setAperture(22.0);

  Scene scene;
  scene.addLight(std::move(directional_light));
  scene.addObject(std::move(sphere_object));
  scene.setCamera(std::move(camera));

  RenderSettings render_settings;
  render_settings.setWidth(800);
  render_settings.setHeight(600);
  render_settings.setChannelCount(1);
  render_settings.setSamplesPerPixel(4);

  Renderer renderer(&render_settings, &scene);
  renderer.renderFrame();

  std::unique_ptr<OutputFormat> output_format = std::make_unique<OutputFormatPng>();

  RenderExporter exporter(renderer.getFramebuffer());
  exporter.setOutputFormat(std::move(output_format));
  exporter.setPath("Gallery");
  exporter.setFilename("SphereExampleWithNormal");
  exporter.setToneMapping(ToneMapping::NONE);

  exporter.exportRender();
}

void renderOBJExample() {
  Mesh obj_mesh = OBJLoader::load("Resources/obj/african_head/african_head.obj");
  auto obj      = std::make_unique<Object3D>(obj_mesh);
  obj->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));

  auto african_head_albedo = TextureLoader::load("Resources/obj/african_head/african_head_diffuse.tga");
  african_head_albedo->setColorSpace(ColorSpace::RGB);
  african_head_albedo->flipVertically();
  auto african_head_normal = TextureLoader::load("Resources/obj/african_head/african_head_nm.tga");
  african_head_normal->flipVertically();
  auto material = std::make_shared<Material>();
  material->setAlbedoTexture(african_head_albedo);
  material->setNormalTexture(african_head_normal);
  obj->setMaterial(material);

  Mesh eye_inner_mesh   = OBJLoader::load("Resources/obj/african_head/african_head_eye_inner.obj");
  auto eye_inner_object = std::make_unique<Object3D>(eye_inner_mesh);
  eye_inner_object->setPosition(Eigen::Vector3d(0.0, 0.0, 0.0));

  auto eye_inner_albedo = TextureLoader::load("Resources/obj/african_head/african_head_eye_inner_diffuse.tga");
  eye_inner_albedo->setColorSpace(ColorSpace::RGB);
  eye_inner_albedo->flipVertically();
  auto eye_inner_normal = TextureLoader::load("Resources/obj/african_head/african_head_eye_inner_nm.tga");
  eye_inner_normal->flipVertically();
  auto eye_inner_material = std::make_shared<Material>();
  eye_inner_material->setAlbedoTexture(eye_inner_albedo);
  eye_inner_material->setNormalTexture(eye_inner_normal);
  eye_inner_object->setMaterial(eye_inner_material);

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setPosition(Eigen::Vector3d(-1.0, 0.0, 4.0));
  camera->setRotation(Eigen::Vector3d(0.0, -15.0, 0.0));
  camera->setAperture(22.0);

  std::unique_ptr<DirectionalLight> directional_light_orange = std::make_unique<DirectionalLight>();
  directional_light_orange->setColor(Eigen::Vector3d(1.0, 0.5, 0.0));
  directional_light_orange->setIntensity(1.0);
  directional_light_orange->setDirection(Eigen::Vector3d(-1.0, 0.0, -1.0));

  std::unique_ptr<DirectionalLight> directional_light_blue = std::make_unique<DirectionalLight>();
  directional_light_blue->setColor(Eigen::Vector3d(0.2, 0.5, 1.0));
  directional_light_blue->setIntensity(1.0);
  directional_light_blue->setDirection(Eigen::Vector3d(1.0, 0.0, -1.0));

  auto skybox_texture = TextureLoader::load("Resources/Textures/studio.png");
  skybox_texture->setColorSpace(ColorSpace::RGB);

  Scene scene;
  scene.addObject(std::move(obj));
  scene.addObject(std::move(eye_inner_object));
  scene.setCamera(std::move(camera));
  scene.addLight(std::move(directional_light_orange));
  scene.addLight(std::move(directional_light_blue));
  scene.setSkybox(skybox_texture);

  RenderSettings render_settings;
  render_settings.setWidth(800);
  render_settings.setHeight(600);
  render_settings.setChannelCount(3);
  render_settings.setSamplesPerPixel(4);

  Renderer renderer(&render_settings, &scene);
  renderer.renderFrame();

  std::unique_ptr<OutputFormat> output_format = std::make_unique<OutputFormatPng>();

  RenderExporter exporter(renderer.getFramebuffer());
  exporter.setOutputFormat(std::move(output_format));
  exporter.setPath("Gallery");
  exporter.setFilename("OBJExample");
  exporter.setToneMapping(ToneMapping::NONE);

  exporter.exportRender();
}

int main() {
  // renderCubeExemple();
  // renderPlaneExemple();
  renderSphereExemple();
  // renderOBJExample();

  return 0;
}