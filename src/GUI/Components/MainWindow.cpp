#include "MainWindow.hpp"
#include "CubeDialog.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Geometry/OBJLoader.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"
#include "Geometry/SphereMeshBuilder.hpp"
#include "Lighting/Light.hpp"
#include "PlaneDialog.hpp"
#include "SphereDialog.hpp"
#include "Scene/Scene.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"
#include "ui_MainWindow.h"

#include "DefaultOptionProxyModel.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(Scene* scene, TextureManager* texture_manager, MaterialManager* material_manager,
                       QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_scene(scene), m_texture_manager(texture_manager),
      m_material_manager(material_manager), m_textures_list_model(new TexturesListModel(m_texture_manager, this)),
      m_materials_list_model(new MaterialsListModel(m_material_manager, this)) {
  ui->setupUi(this);

  ui->nameLabel->setText("");

  ui->texturesWidget->setModel(m_textures_list_model);
  ui->texturesWidget->setTextureManager(m_texture_manager);

  ui->materialsWidget->setModel(m_materials_list_model);
  ui->materialsWidget->setMaterialManager(m_material_manager);
  ui->materialsWidget->setTextureManager(m_texture_manager);

  auto* proxy_textures_list = new DefaultOptionProxyModel();
  proxy_textures_list->setSourceModel(m_textures_list_model);

  ui->materialsWidget->setTexturesListModel(proxy_textures_list);

  ui->skyboxWidget->setSkybox(m_scene->getSkybox());
  ui->skyboxWidget->setTexturesListModel(proxy_textures_list);

  auto* proxy_materials_list = new DefaultOptionProxyModel();
  proxy_materials_list->setSourceModel(m_materials_list_model);

  ui->objectWidget->setMaterialsListModel(proxy_materials_list);
  ui->objectWidget->setMaterialManager(m_material_manager);

  ui->sceneView->initializeFromScene(m_scene);
  ui->openGLWidget->setScene(m_scene);
  ui->renderSettingsWidget->setScene(m_scene);

  ui->tabWidget->tabBar()->setExpanding(true);

  ui->transformWidget->setVisible(false);
  ui->cameraWidget->setVisible(false);
  ui->lightWidget->setVisible(false);
  ui->objectWidget->setVisible(false);

  connect(ui->actionAdd_Cube, &QAction::triggered, this, &MainWindow::onActionAddCubeTriggered);
  connect(ui->actionAdd_Sphere, &QAction::triggered, this, &MainWindow::onActionAddSphereTriggered);
  connect(ui->actionAdd_Plane, &QAction::triggered, this, &MainWindow::onActionAddPlaneTriggered);
  connect(ui->actionLoad_OBJ, &QAction::triggered, this, &MainWindow::onActionLoadObjTriggered);

  connect(ui->actionAdd_Directional_Light, &QAction::triggered, this,
          &MainWindow::onActionAddDirectionalLightTriggered);
  connect(ui->actionAdd_Point_Light, &QAction::triggered, this, &MainWindow::onActionAddPointLightTriggered);
  connect(ui->actionAdd_Spotlight, &QAction::triggered, this, &MainWindow::onActionAddSpotLightTriggered);

  connect(ui->sceneView, &SceneTreeView::objectSelected, this, &MainWindow::onObjectSelected);
  connect(ui->sceneView, &SceneTreeView::cameraSelected, this, &MainWindow::onCameraSelected);
  connect(ui->sceneView, &SceneTreeView::lightSelected, this, &MainWindow::onLightSelected);
  connect(ui->sceneView, &SceneTreeView::selectionCleared, this, &MainWindow::onSelectionCleared);

  connect(ui->openGLWidget, &OpenGLWidget::objectSelected, this, &MainWindow::onObjectSelected);
  connect(ui->openGLWidget, &OpenGLWidget::objectUnselected, this, &MainWindow::onSelectionCleared);
  connect(ui->openGLWidget, &OpenGLWidget::objectDeleted, ui->sceneView, &SceneTreeView::deleteSelectedItem);
  connect(ui->openGLWidget, &OpenGLWidget::engineInitialized, ui->viewportSettingsWidget,
          &ViewportSettingsWidget::setEngine);

  connect(ui->cameraWidget, &CameraWidget::viewportCameraUpdated, this, &MainWindow::onViewportCameraUpdated);
  connect(ui->cameraWidget, &CameraWidget::renderCameraUpdated, this, &MainWindow::onRenderCameraUpdated);

  connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

  connect(ui->skyboxWidget, &SkyboxWidget::skyboxChanged, this, &MainWindow::onSkyboxChanged);
  connect(ui->skyboxWidget, &SkyboxWidget::displayInViewportToggled, this, &MainWindow::onDisplaySkyboxToggled);
  connect(ui->skyboxWidget, &SkyboxWidget::skyboxTextureCreated, this, &MainWindow::onSkyboxTextureCreated);

  connect(ui->objectWidget, &ObjectWidget::materialChanged, this, &MainWindow::onMaterialChanged);

  connect(ui->materialsWidget, &MaterialsWidget::diffuseTextureCreated, this, &MainWindow::onDiffuseTextureCreated);
  connect(ui->materialsWidget, &MaterialsWidget::normalTextureCreated, this, &MainWindow::onNormalTextureCreated);

  connect(ui->viewportSettingsWidget, &ViewportSettingsWidget::lightBakeRequested, this,
          &MainWindow::onBakeLightRequested);
}

void MainWindow::addObjectToScene(std::unique_ptr<Object3D> object, const std::string& base_name) {
  const std::string object_name = m_scene->getAvailableObjectName(base_name);

  ui->openGLWidget->makeCurrent();
  m_scene->addObject(object_name, std::move(object));
  ui->openGLWidget->doneCurrent();

  ui->sceneView->addObject(QString::fromStdString(object_name));
}

void MainWindow::onActionAddCubeTriggered() {
  CubeDialog dialog(this);
  if(dialog.exec() == QDialog::Accepted) {
    const double          size = dialog.getSize();
    const CubeMeshBuilder cube_builder(size);
    const Mesh            cube_mesh = cube_builder.build();
    addObjectToScene(std::make_unique<Object3D>(cube_mesh), "Cube");
  }
}

void MainWindow::onActionAddSphereTriggered() {
  SphereDialog dialog(this);
  if(dialog.exec() == QDialog::Accepted) {
    const double radius   = dialog.getRadius();
    const int    segments = dialog.getSegments();
    const int    rings    = dialog.getRings();

    const SphereMeshBuilder sphere_builder(radius, segments, rings);
    const Mesh              sphere_mesh = sphere_builder.build();

    addObjectToScene(std::make_unique<Object3D>(sphere_mesh), "Sphere");
  }
}

void MainWindow::onActionAddPlaneTriggered() {
  PlaneDialog dialog(this);
  if(dialog.exec() == QDialog::Accepted) {
    const double width  = dialog.getWidth();
    const double length = dialog.getLength();

    const PlaneMeshBuilder plane_builder(width, length);
    const Mesh             plane_mesh = plane_builder.build();

    addObjectToScene(std::make_unique<Object3D>(plane_mesh), "Plane");
  }
}

void MainWindow::onActionLoadObjTriggered() {
  const QString file_name = QFileDialog::getOpenFileName(this, "Load .obj File", "", "OBJ Files (*.obj)");
  if(file_name.isEmpty()) {
    return;
  }

  const Mesh obj_mesh = OBJLoader::load(file_name.toStdString());
  auto       object   = std::make_unique<Object3D>(obj_mesh);

  if(!object) {
    QMessageBox::warning(this, "Error", "Failed to load .obj file.");
    return;
  }

  const std::string base_name = QFileInfo(file_name).baseName().toStdString();
  addObjectToScene(std::move(object), base_name);
}

void MainWindow::addLightToScene(const std::string& base_name, std::unique_ptr<Light> light) {
  const std::string light_name = m_scene->getAvailableLightName(base_name);
  ui->sceneView->addLight(QString::fromStdString(light_name));

  ui->openGLWidget->makeCurrent();
  m_scene->addLight(light_name, std::move(light));
  ui->openGLWidget->doneCurrent();
}

void MainWindow::onActionAddDirectionalLightTriggered() {
  addLightToScene("Directional Light", std::make_unique<DirectionalLight>());
}

void MainWindow::onActionAddPointLightTriggered() { addLightToScene("Point Light", std::make_unique<PointLight>()); }

void MainWindow::onActionAddSpotLightTriggered() { addLightToScene("Spot Light", std::make_unique<SpotLight>()); }

void MainWindow::onObjectSelected(const QString& object_name) {
  Object3D* object = m_scene->getObject(object_name.toStdString());

  ui->nameLabel->setText(object_name);

  ui->openGLWidget->selectObject(object_name);
  ui->sceneView->selectObject(object_name);
  ui->transformWidget->setTransform(object);
  ui->objectWidget->setObject3D(object);

  ui->transformWidget->setVisible(true);
  ui->cameraWidget->setVisible(false);
  ui->lightWidget->setVisible(false);
  ui->objectWidget->setVisible(true);
}

void MainWindow::onCameraSelected() {
  ui->nameLabel->setText("Camera");

  ui->openGLWidget->unselectObject();
  ui->transformWidget->setTransform(m_scene->getCamera());
  ui->cameraWidget->setCamera(m_scene->getCamera());

  ui->transformWidget->setVisible(true);
  ui->cameraWidget->setVisible(true);
  ui->lightWidget->setVisible(false);
  ui->objectWidget->setVisible(false);
}

void MainWindow::onLightSelected(const QString& light_name) {
  ui->nameLabel->setText(light_name);

  Light* light = m_scene->getLight(light_name.toStdString());
  ui->openGLWidget->unselectObject();
  ui->transformWidget->setTransform(light);
  ui->lightWidget->setLight(light);

  ui->transformWidget->setVisible(true);
  ui->cameraWidget->setVisible(false);
  ui->lightWidget->setVisible(true);
  ui->objectWidget->setVisible(false);
}

void MainWindow::onSelectionCleared() {
  ui->nameLabel->setText("");

  ui->openGLWidget->unselectObject();
  ui->sceneView->clearSelection();
  ui->transformWidget->setTransform(nullptr);

  ui->transformWidget->setVisible(false);
  ui->cameraWidget->setVisible(false);
  ui->lightWidget->setVisible(false);
  ui->objectWidget->setVisible(false);
}

void MainWindow::onTabChanged(int index) {
  if(index == 1) { // Skybox tab
    ui->skyboxWidget->updateSkyboxPreview();
  }
}

void MainWindow::onViewportCameraUpdated() { ui->openGLWidget->updateViewportCamera(); }

void MainWindow::onRenderCameraUpdated() {
  ui->openGLWidget->updateRenderCamera();
  ui->transformWidget->updateWidget();
}

void MainWindow::onSkyboxChanged(const QString& new_skybox) {
  ui->openGLWidget->makeCurrent();
  if(new_skybox.isEmpty() || new_skybox == "Default") {
    m_scene->setSkybox(TextureManager::DefaultSkyboxTexture());
  } else {
    Texture* texture = m_texture_manager->getTexture(new_skybox.toStdString());
    m_scene->setSkybox(texture);
  }
  ui->openGLWidget->doneCurrent();
  ui->skyboxWidget->updateSkyboxPreview();
}

void MainWindow::onDisplaySkyboxToggled(bool checked) { ui->openGLWidget->getEngine()->setSkyboxVisible(checked); }

void MainWindow::onSkyboxTextureCreated(const QString& texture_path) {
  const QString texture_name = createTextureFromFile(texture_path.toStdString(), ColorSpace::LINEAR);

  ui->openGLWidget->makeCurrent();
  ui->skyboxWidget->setCurrentTexture(texture_name);
  ui->openGLWidget->doneCurrent();

  ui->skyboxWidget->updateSkyboxPreview();
}

void MainWindow::onBakeLightRequested() {
  ui->openGLWidget->makeCurrent();
  ui->openGLWidget->getEngine()->bakeLights();
  ui->openGLWidget->doneCurrent();
}

void MainWindow::onMaterialChanged(Object3D* object, const QString& new_material) {
  ui->openGLWidget->makeCurrent();
  if(new_material.isEmpty() || new_material == "Default") {
    object->setMaterial(MaterialManager::DefaultMaterial());
  } else {
    Material* material = m_material_manager->getMaterial(new_material.toStdString());
    object->setMaterial(material);
  }
  ui->openGLWidget->doneCurrent();
}

void MainWindow::onDiffuseTextureCreated(const QString& texture_path) {
  const QString texture_name = createTextureFromFile(texture_path.toStdString(), ColorSpace::LINEAR);
  ui->openGLWidget->makeCurrent();
  ui->materialsWidget->setDiffuseTexture(texture_name);
  ui->openGLWidget->doneCurrent();
}

void MainWindow::onNormalTextureCreated(const QString& texture_path) {
  const QString texture_name = createTextureFromFile(texture_path.toStdString(), ColorSpace::S_RGB);
  ui->openGLWidget->makeCurrent();
  ui->materialsWidget->setNormalTexture(texture_name);
  ui->openGLWidget->doneCurrent();
}

QString MainWindow::createTextureFromFile(const std::string& file_path, ColorSpace color_space) {
  QString texture_name = QFileInfo(QString::fromStdString(file_path)).completeBaseName();
  texture_name         = QString::fromStdString(m_texture_manager->getAvailableTextureName(texture_name.toStdString()));
  m_textures_list_model->addItem(texture_name);

  Texture* new_texture = m_texture_manager->getTexture(texture_name.toStdString());

  new_texture->loadFromFile(file_path.c_str());
  new_texture->setColorSpace(color_space);

  return texture_name;
}

MainWindow::~MainWindow() {
  delete ui;
  delete m_textures_list_model;
  delete m_materials_list_model;
}