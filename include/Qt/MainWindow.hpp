/**
 * @file MainWindow.hpp
 * @brief Header file for the MainWindow class, which serves as the main application window.
 */
#ifndef QT_MAINWINDOW_HPP
#define QT_MAINWINDOW_HPP

#include <qevent.h>
#include <qmainwindow.h>
#include <qstackedwidget.h>
#include <qstring.h>
#include <qtabbar.h>

#include "Core/Transform.hpp"
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

/**
 * @class MainWindow
 * @brief The main application window that contains various views and controllers for managing the scene.
 *
 * This class serves as the main interface for the application, allowing users to interact with the scene,
 * manage objects, lights, cameras, materials, textures, and rendering settings.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /**
   * @brief Constructs the MainWindow with the specified scene, texture manager, material manager, and render settings.
   * @param scene The scene to be managed by this window.
   * @param texture_manager The manager for textures used in the scene.
   * @param material_manager The manager for materials used in the scene.
   * @param render_settings The rendering settings for the scene.
   */
  MainWindow(Scene* scene, TextureManager* texture_manager, MaterialManager* material_manager,
             RenderSettings* render_settings);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  // Slots
  void onCameraSelected();
  void onObjectSelected(const QString& object_name);
  void onLightSelected(const QString& light_name);
  void onSelectionCleared();
  void onItemRenamed(const QString& new_name, const QString& old_name);

  // Members
  Scene*           m_scene            = nullptr;
  RenderSettings*  m_render_settings  = nullptr;
  TextureManager*  m_texture_manager  = nullptr;
  MaterialManager* m_material_manager = nullptr;

  QWidget* m_central_widget = nullptr;

  QLabel*         m_selected_object_name    = nullptr;
  QTabBar*        m_settings_tab_bar        = nullptr;
  QStackedWidget* m_settings_stacked_widget = nullptr;

  RealtimeView*       m_realtime_view       = nullptr;
  RealtimeController* m_realtime_controller = nullptr;

  SceneView*       m_scene_view       = nullptr;
  SceneController* m_scene_controller = nullptr;

  MenuView*       m_menu_view       = nullptr;
  MenuController* m_menu_controller = nullptr;

  Transform* m_current_transform = nullptr;

  TransformView*       m_transform_view       = nullptr;
  TransformController* m_transform_controller = nullptr;

  ObjectView*       m_object_view       = nullptr;
  ObjectController* m_object_controller = nullptr;

  TextureView*       m_texture_view       = nullptr;
  TextureController* m_texture_controller = nullptr;

  CameraView*       m_camera_view       = nullptr;
  CameraController* m_camera_controller = nullptr;

  DirectionalLightView*       m_directional_light_view       = nullptr;
  DirectionalLightController* m_directional_light_controller = nullptr;

  PointLightView*       m_point_light_view       = nullptr;
  PointLightController* m_point_light_controller = nullptr;

  SpotLightView*       m_spot_light_view       = nullptr;
  SpotLightController* m_spot_light_controller = nullptr;

  ViewportView*       m_viewport_view       = nullptr;
  ViewportController* m_viewport_controller = nullptr;

  RenderView*       m_render_view       = nullptr;
  RenderController* m_render_controller = nullptr;

  SkyboxView*       m_skybox_view       = nullptr;
  SkyboxController* m_skybox_controller = nullptr;

  MaterialView*       m_material_view       = nullptr;
  MaterialController* m_material_controller = nullptr;

  void addSettingsTab(std::string_view tab_name, QWidget* widget);
  void hideAllViews();

  void initializeViews();
  void initializeControllers();

  void connectSignals();
  void setupUI();
};

#endif // QT_MAINWINDOW_HPP
