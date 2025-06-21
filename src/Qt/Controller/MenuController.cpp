#include <memory>
#include <qdialog.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstring.h>
#include <string>
#include <utility>

#include "Geometry/Mesh.hpp"
#include "Geometry/OBJLoader.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Qt/Controller/MenuController.hpp"
#include "Qt/MeshBuilderDialog/CubeMeshBuilderDialog.hpp"
#include "Qt/MeshBuilderDialog/PlaneMeshBuilderDialog.hpp"
#include "Qt/MeshBuilderDialog/SphereMeshBuilderDialog.hpp"
#include "Qt/View/MenuView.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"

MenuController::MenuController(MenuView* view, Scene* scene) : QObject(view), m_view(view), m_scene(scene) {
  connectSignals();
}

void MenuController::connectSignals() {
  connect(m_view, &MenuView::addCubeRequested, this, &MenuController::onAddCubeRequested);
  connect(m_view, &MenuView::addPlaneRequested, this, &MenuController::onAddPlaneRequested);
  connect(m_view, &MenuView::addSphereRequested, this, &MenuController::onAddSphereRequested);
  connect(m_view, &MenuView::loadObjRequested, this, &MenuController::onLoadObjRequested);

  connect(m_view, &MenuView::addDirectionalLightRequested, this, &MenuController::onAddDirectionalLightRequested);
  connect(m_view, &MenuView::addPointLightRequested, this, &MenuController::onAddPointLightRequested);
  connect(m_view, &MenuView::addSpotLightRequested, this, &MenuController::onAddSpotLightRequested);
}

void MenuController::onAddCubeRequested() {
  auto cube_builder = std::make_unique<CubeMeshBuilderDialog>(m_view);
  if(cube_builder->exec() == QDialog::Accepted) {
    auto mesh = cube_builder->buildMesh();
    auto obj  = std::make_unique<Object3D>(*mesh);

    const std::string object_name = m_scene->getAvailableObjectName("Cube");

    m_scene->addObject(object_name, std::move(obj));
  }
}

void MenuController::onAddPlaneRequested() {
  auto plane_builder = std::make_unique<PlaneMeshBuilderDialog>(m_view);
  if(plane_builder->exec() == QDialog::Accepted) {
    auto mesh = plane_builder->buildMesh();
    auto obj  = std::make_unique<Object3D>(*mesh);

    const std::string object_name = m_scene->getAvailableObjectName("Plane");

    m_scene->addObject(object_name, std::move(obj));
  }
}

void MenuController::onAddSphereRequested() {
  auto sphere_builder = std::make_unique<SphereMeshBuilderDialog>(m_view);
  if(sphere_builder->exec() == QDialog::Accepted) {
    auto mesh = sphere_builder->buildMesh();
    auto obj  = std::make_unique<Object3D>(*mesh);

    const std::string object_name = m_scene->getAvailableObjectName("Sphere");

    m_scene->addObject(object_name, std::move(obj));
  }
}

void MenuController::onLoadObjRequested() {
  const QString file_name = QFileDialog::getOpenFileName(m_view, "Load .obj File", "", "OBJ Files (*.obj)");
  if(file_name.isEmpty()) {
    return;
  }

  const Mesh obj_mesh   = OBJLoader::load(file_name.toStdString());
  auto       obj_object = std::make_unique<Object3D>(obj_mesh);
  if(!obj_object) {
    QMessageBox::warning(m_view, "Error", "Failed to load .obj file.");
    return;
  }

  const std::string object_name = m_scene->getAvailableObjectName(QFileInfo(file_name).baseName().toStdString());

  m_scene->addObject(object_name, std::move(obj_object));
}

void MenuController::onAddDirectionalLightRequested() {
  const std::string light_name        = m_scene->getAvailableLightName("Directional Light");
  auto              directional_light = std::make_unique<DirectionalLight>();
  m_scene->addLight(light_name, std::move(directional_light));
}

void MenuController::onAddPointLightRequested() {
  const std::string light_name  = m_scene->getAvailableLightName("Point Light");
  auto              point_light = std::make_unique<PointLight>();
  m_scene->addLight(light_name, std::move(point_light));
}

void MenuController::onAddSpotLightRequested() {
  const std::string light_name = m_scene->getAvailableLightName("Spot Light");
  auto              spot_light = std::make_unique<SpotLight>();
  m_scene->addLight(light_name, std::move(spot_light));
}