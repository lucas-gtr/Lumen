#include "Core/Camera.hpp"
#include "Core/Mesh.hpp"
#include "Core/Object3D.hpp"
#include "Core/Scene.hpp"
#include "Export/OutputFormatPng.hpp"
#include "Export/RenderExporter.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"

#include <climits>
#include <iostream>
#include <vector>

int main() {
  Vertex v1, v2, v3;
  v1.position = Eigen::Vector3d(0.5, -0.5, 0);
  v1.normal   = Eigen::Vector3d(0, 0, 1);
  v1.texCoord = Eigen::Vector2d(0, 0);

  v2.position = Eigen::Vector3d(-0.5, 0.5, 0);
  v2.normal   = Eigen::Vector3d(0, 0, 1);
  v2.texCoord = Eigen::Vector2d(1, 0);

  v3.position = Eigen::Vector3d(0.5, 0.5, 0);
  v3.normal   = Eigen::Vector3d(0, 0, 1);
  v3.texCoord = Eigen::Vector2d(0, 1);

  const std::vector<Vertex> vertices = {v1, v2, v3};
  const std::vector<Face>   faces    = {{0, 1, 2}};

  Mesh triangle_mesh(vertices, faces);

  std::unique_ptr<Object3D> triangle = std::make_unique<Object3D>(triangle_mesh);
  triangle->setPosition(Eigen::Vector3d(0.0, 0.0, -5));
  triangle->setRotation(Eigen::Vector3d(45, 0, 180));
  triangle->setScale(1.5);

  std::unique_ptr<Camera> camera = std::make_unique<Camera>();
  camera->setAperture(14.0);
  camera->setFocusDistance(5.0);
  camera->setPosition(Eigen::Vector3d(0, 0.0, 0));
  camera->setRotation(Eigen::Vector3d(0, 0.0, 0));

  Scene scene;
  scene.addObject(std::move(triangle));
  scene.setCamera(std::move(camera));
  scene.setBackgroundColor(Eigen::Vector3d(1.0, 0.0, 0.0));

  RenderSettings render_settings;
  render_settings.setSamplesPerPixel(1);

  Renderer renderer(&render_settings, &scene);
  renderer.renderFrame();

  OutputFormatPng output_format;
  RenderExporter  exporter(renderer.getFramebuffer());
  exporter.setPath("RenderImages");
  exporter.setFilename("image");
  exporter.setOutputFormat(&output_format);
  exporter.exportRender();

  return 0;
}