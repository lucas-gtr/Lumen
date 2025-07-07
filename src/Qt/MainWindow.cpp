#include <qaction.h>
#include <qboxlayout.h>
#include <qevent.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsplitter.h>
#include <qstackedwidget.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtabbar.h>
#include <qwidget.h>
#include <string>

#include "Core/Config.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/Light.hpp"
#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Qt/Controller/CameraController.hpp"
#include "Qt/Controller/Light/DirectionalLightController.hpp"
#include "Qt/Controller/Light/PointLightController.hpp"
#include "Qt/Controller/Light/SpotLightController.hpp"
#include "Qt/Controller/MaterialController.hpp"
#include "Qt/Controller/MenuController.hpp"
#include "Qt/Controller/ObjectController.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/RenderController.hpp"
#include "Qt/Controller/SceneController.hpp"
#include "Qt/Controller/SkyboxController.hpp"
#include "Qt/Controller/TextureController.hpp"
#include "Qt/Controller/TransformController.hpp"
#include "Qt/Controller/ViewportController.hpp"
#include "Qt/MainWindow.hpp"
#include "Qt/View/CameraView.hpp"
#include "Qt/View/Light/DirectionalLightView.hpp"
#include "Qt/View/Light/PointLightView.hpp"
#include "Qt/View/Light/SpotLightView.hpp"
#include "Qt/View/MaterialView.hpp"
#include "Qt/View/MenuView.hpp"
#include "Qt/View/ObjectView.hpp"
#include "Qt/View/RealtimeView.hpp"
#include "Qt/View/RenderView.hpp"
#include "Qt/View/SceneView.hpp"
#include "Qt/View/SkyboxView.hpp"
#include "Qt/View/TextureView.hpp"
#include "Qt/View/TransformView.hpp"
#include "Qt/View/ViewportView.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Scene/Scene.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

MainWindow::MainWindow(Scene* scene, TextureManager* texture_manager, MaterialManager* material_manager,
                       RenderSettings* render_settings)
    : QMainWindow(nullptr), m_scene(scene), m_render_settings(render_settings), m_texture_manager(texture_manager),
      m_material_manager(material_manager) {
  setAttribute(Qt::WA_DeleteOnClose, true);
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void MainWindow::setupUI() {
  setGeometry(0, 0, QT_DEFAULT_WINDOW_WIDTH, QT_DEFAULT_WINDOW_HEIGHT);
  setWindowTitle(QString::fromStdString(std::string(QT_WINDOW_TITLE)));

  m_central_widget = new QWidget(this);
  initializeViews();
  initializeControllers();

  setCentralWidget(m_central_widget);
  setMenuBar(m_menu_view);

  auto* main_splitter = new QSplitter(Qt::Horizontal, m_central_widget);
  main_splitter->setContentsMargins(0, 0, 0, 0);

  // Vue de droite (infos + paramètres)
  auto* right_container = new QWidget(main_splitter);
  auto* right_layout    = new QVBoxLayout(right_container);
  right_layout->setContentsMargins(0, 0, 0, 0);

  m_selected_object_name = new QLabel(right_container);
  m_selected_object_name->setStyleSheet("font-weight: bold; font-size: 16px;");
  right_layout->addWidget(m_selected_object_name, 0, Qt::AlignTop);
  right_layout->addWidget(m_transform_view, 0, Qt::AlignTop);
  right_layout->addWidget(m_object_view, 0, Qt::AlignTop);
  right_layout->addWidget(m_directional_light_view, 0, Qt::AlignTop);
  right_layout->addWidget(m_point_light_view, 0, Qt::AlignTop);
  right_layout->addWidget(m_spot_light_view, 0, Qt::AlignTop);
  right_layout->addWidget(m_camera_view, 0, Qt::AlignTop);

  m_settings_tab_bar        = new QTabBar(right_container);
  m_settings_stacked_widget = new QStackedWidget(right_container);
  m_settings_stacked_widget->setMinimumWidth(MAIN_WIDOW_RIGHT_LAYOUT_MIN_WIDTH);

  right_layout->addWidget(m_settings_tab_bar);
  right_layout->addWidget(m_settings_stacked_widget);
  right_layout->addStretch(1);

  main_splitter->addWidget(m_scene_view);
  main_splitter->addWidget(m_realtime_view);
  main_splitter->addWidget(right_container);
  main_splitter->setStretchFactor(1, 1); // Vue gauche prioritaire pour l’espace

  m_transform_view->disableAllInputs();

  addSettingsTab(TAB_VIEWPORT_NAME, m_viewport_view);
  addSettingsTab(TAB_SKYBOX_NAME, m_skybox_view);
  addSettingsTab(TAB_TEXTURES_NAME, m_texture_view);
  addSettingsTab(TAB_MATERIALS_NAME, m_material_view);
  addSettingsTab(TAB_RENDER_NAME, m_render_view);

  m_settings_tab_bar->setCurrentIndex(0);

  hideAllViews();

  auto* layout = new QHBoxLayout(m_central_widget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(main_splitter);
}

void MainWindow::initializeViews() {
  m_menu_view              = new MenuView(this);
  m_realtime_view          = new RealtimeView(this);
  m_scene_view             = new SceneView(m_central_widget);
  m_transform_view         = new TransformView(m_central_widget);
  m_object_view            = new ObjectView(m_central_widget);
  m_camera_view            = new CameraView(m_central_widget);
  m_directional_light_view = new DirectionalLightView(m_central_widget);
  m_point_light_view       = new PointLightView(m_central_widget);
  m_spot_light_view        = new SpotLightView(m_central_widget);
  m_viewport_view          = new ViewportView(m_central_widget);
  m_render_view            = new RenderView(m_central_widget);
  m_skybox_view            = new SkyboxView(m_central_widget);
  m_texture_view           = new TextureView(m_central_widget);
  m_material_view          = new MaterialView(m_central_widget);
}

void MainWindow::initializeControllers() {
  m_menu_controller      = new MenuController(m_menu_view, m_scene);
  m_realtime_controller  = new RealtimeController(m_realtime_view, m_scene);
  m_scene_controller     = new SceneController(m_scene_view, m_scene);
  m_transform_controller = new TransformController(m_transform_view);
  m_object_controller    = new ObjectController(m_object_view, m_material_manager);
  m_camera_controller =
      new CameraController(m_camera_view, m_scene->getCamera(), m_realtime_controller, m_transform_controller);
  m_directional_light_controller = new DirectionalLightController(m_directional_light_view);
  m_point_light_controller       = new PointLightController(m_point_light_view);
  m_spot_light_controller        = new SpotLightController(m_spot_light_view);
  m_texture_controller           = new TextureController(m_texture_view, m_texture_manager);
  m_viewport_controller          = new ViewportController(m_viewport_view, m_realtime_controller);
  m_render_controller            = new RenderController(m_render_view, m_render_settings, m_scene);
  m_skybox_controller            = new SkyboxController(m_skybox_view, m_scene->getSkybox(), m_texture_controller,
                                                        m_texture_manager, m_realtime_controller);
  m_material_controller =
      new MaterialController(m_material_view, m_material_manager, m_texture_manager, m_texture_controller);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void MainWindow::addSettingsTab(std::string_view tab_name, QWidget* widget) {
  m_settings_tab_bar->addTab(QString::fromStdString(std::string(tab_name)));
  m_settings_stacked_widget->addWidget(widget);
}

void MainWindow::connectSignals() {
  connect(m_scene_view, &SceneView::cameraSelected, this, &MainWindow::onCameraSelected);
  connect(m_scene_view, &SceneView::objectSelected, this, &MainWindow::onObjectSelected);
  connect(m_scene_view, &SceneView::lightSelected, this, &MainWindow::onLightSelected);
  connect(m_scene_view, &SceneView::selectionCleared, this, &MainWindow::onSelectionCleared);

  connect(m_realtime_controller, &RealtimeController::objectSelected, this, &MainWindow::onObjectSelected);
  connect(m_realtime_controller, &RealtimeController::objectUnselected, this, &MainWindow::onSelectionCleared);

  connect(m_settings_tab_bar, &QTabBar::currentChanged, this, [this](int index) {
    m_settings_stacked_widget->setCurrentIndex(index);
    const QString tab_text = m_settings_tab_bar->tabText(index);
    if(tab_text == QString::fromStdString(std::string(TAB_VIEWPORT_NAME))) {
      m_viewport_controller->updateViewFromModel();
    } else if(tab_text == QString::fromStdString(std::string(TAB_SKYBOX_NAME))) {
      m_skybox_controller->updateViewFromModel();
    } else if(tab_text == QString::fromStdString(std::string(TAB_TEXTURES_NAME))) {
      m_texture_controller->updateViewFromModel();
    } else if(tab_text == QString::fromStdString(std::string(TAB_MATERIALS_NAME))) {
      m_material_controller->updateViewFromModel();
    }
  });

  connect(m_scene_controller, &SceneController::itemRenamed, this, &MainWindow::onItemRenamed);
}

void MainWindow::hideAllViews() {
  m_camera_view->setVisible(false);
  m_object_view->setVisible(false);
  m_directional_light_view->setVisible(false);
  m_point_light_view->setVisible(false);
  m_spot_light_view->setVisible(false);
  m_transform_view->setVisible(false);
}

void MainWindow::onCameraSelected() {
  hideAllViews();

  m_realtime_controller->unselectObject();

  m_camera_view->setVisible(true);

  m_transform_view->setVisible(true);
  m_transform_view->disableAllInputs();
  m_transform_view->enablePositionInputs();
  m_transform_view->enableRotationInputs();

  m_transform_controller->setTransform(m_scene->getCamera());
  m_camera_controller->updateViewFromModel();

  m_selected_object_name->setText("Camera");
}

void MainWindow::onObjectSelected(const QString& object_name) {
  hideAllViews();

  m_realtime_controller->selectObject(object_name);
  m_scene_view->setSelectedObject(object_name);
  m_selected_object_name->setText(object_name);

  auto* object = m_scene->getObject(object_name.toStdString());
  m_object_view->setVisible(true);
  m_object_controller->setObject(object);

  m_transform_view->setVisible(true);
  m_transform_view->enablePositionInputs();
  m_transform_view->enableRotationInputs();
  m_transform_view->enableScaleInputs();
  m_transform_controller->setTransform(object);
}

void MainWindow::onLightSelected(const QString& light_name) {
  auto* light = m_scene->getLight(light_name.toStdString());
  if(light == nullptr) {
    return;
  }
  hideAllViews();

  m_realtime_controller->unselectObject();
  m_transform_view->disableAllInputs();
  m_transform_view->setVisible(true);

  switch(light->getType()) {
  case LightType::DIRECTIONAL:
    m_directional_light_view->setVisible(true);
    m_directional_light_controller->setDirectionalLight(dynamic_cast<DirectionalLight*>(light));
    m_selected_object_name->setText("Directional Light");
    break;
  case LightType::POINT:
    m_transform_view->enablePositionInputs();

    m_point_light_view->setVisible(true);
    m_point_light_controller->setPointLight(dynamic_cast<PointLight*>(light));
    m_selected_object_name->setText("Point Light");
    break;
  case LightType::SPOT:
    m_transform_view->enablePositionInputs();

    m_spot_light_view->setVisible(true);
    m_spot_light_controller->setSpotLight(dynamic_cast<SpotLight*>(light));
    m_selected_object_name->setText("Spot Light");
    break;
  default:
    break;
  }
  m_transform_controller->setTransform(light);
}

void MainWindow::onSelectionCleared() {
  hideAllViews();

  m_scene_view->clearSelection();
  m_selected_object_name->setText("");
  m_realtime_controller->unselectObject();
}

void MainWindow::onItemRenamed(const QString& new_name, const QString& old_name) {
  if(m_selected_object_name->text() == old_name) {
    m_selected_object_name->setText(new_name);
  }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  if(event->key() == Qt::Key_Escape) {
    close();
  }
}