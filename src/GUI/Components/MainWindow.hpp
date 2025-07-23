/**
 * @file MainWindow.hpp
 * @brief Header file for the MainWindow class, which provides the main user interface for the application.
 */
#ifndef GUI_WIDGETS_MAINWINDOW_HPP
#define GUI_WIDGETS_MAINWINDOW_HPP

#include <QAbstractListModel>
#include <QMainWindow>

#include "MaterialsListModel.hpp"
#include "Scene/Scene.hpp"
#include "TexturesListModel.hpp"

class TextureManager;
class MaterialManager;
class Light;
enum class ColorSpace : uint8_t;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the MainWindow class.
   * @param scene The Scene instance to manage the 3D scene.
   * @param texture_manager The TextureManager instance to manage textures.
   * @param material_manager The MaterialManager instance to manage materials.
   * @param parent The parent widget for this main window.
   */
  explicit MainWindow(Scene* scene, TextureManager* texture_manager, MaterialManager* material_manager,
                      QWidget* parent = nullptr);

  ~MainWindow(); ///< Default destructor for the MainWindow class.

private slots:
  void onActionAddCubeTriggered();
  void onActionAddSphereTriggered();
  void onActionAddPlaneTriggered();
  void onActionLoadObjTriggered();

  void onActionAddDirectionalLightTriggered();
  void onActionAddPointLightTriggered();
  void onActionAddSpotLightTriggered();

  void onObjectSelected(const QString& object_name);
  void onCameraSelected();
  void onLightSelected(const QString& light_name);
  void onSelectionCleared();

  void onTabChanged(int index);

  void onViewportCameraUpdated();
  void onRenderCameraUpdated();

  void onSkyboxChanged(const QString& new_skybox);
  void onDisplaySkyboxToggled(bool checked);
  void onSkyboxTextureCreated(const QString& texture_path);

  void onMaterialChanged(Object3D* object, const QString& new_material);

  void onDiffuseTextureCreated(const QString& texture_path);
  void onNormalTextureCreated(const QString& texture_path);
  void onEmissiveTextureCreated(const QString& texture_path);
  void onRoughnessTextureCreated(const QString& texture_path);
  void onMetalTextureCreated(const QString& texture_path);

  void onBakeLightRequested();

private:
  Ui::MainWindow* ui;

  Scene*           m_scene;
  TextureManager*  m_texture_manager;
  MaterialManager* m_material_manager;

  TexturesListModel*  m_textures_list_model;
  MaterialsListModel* m_materials_list_model;

  void addLightToScene(const std::string& base_name, std::unique_ptr<Light> light);
  void addObjectToScene(std::unique_ptr<Object3D> object, const std::string& base_name);

  QString createTextureFromFile(const std::string& file_path, ColorSpace color_space);
};

#endif // GUI_WIDGETS_MAINWINDOW_HPP
