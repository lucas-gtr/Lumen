/**
 * @file SkyboxWidget.hpp
 * @brief Header file for the SkyboxWidget class, which provides a user interface for managing
 *        skyboxes in a 3D scene.
 */
#ifndef GUI_COMPONENTS_SKYBOXWIDGET_HPP
#define GUI_COMPONENTS_SKYBOXWIDGET_HPP

#include <QWidget>

#include "DefaultOptionProxyModel.hpp"

class Skybox;

namespace Ui {
class SkyboxWidget;
}

/**
 * @class SkyboxWidget
 * @brief A widget for displaying and editing properties of a skybox.
 *
 * This class provides a user interface for managing a skybox, including its texture and visibility in the viewport.
 * It allows users to select textures from a list and update the skybox properties accordingly.
 */
class SkyboxWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the SkyboxWidget class.
   * @param parent The parent widget for this skybox widget.
   */
  explicit SkyboxWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the skybox for this widget.
   * @param skybox The Skybox instance to set.
   */
  void setSkybox(Skybox* skybox);

  /**
   * @brief Sets the textures list model for this widget.
   * @param model The DefaultOptionProxyModel instance to set as the textures list model.
   */
  void setTexturesListModel(DefaultOptionProxyModel* model);

  /**
   * @brief Updates the widget to reflect the current state of the skybox and textures.
   */
  void updateWidget();

  /**
   * @brief Updates the skybox preview based on the current texture.
   */
  void updateSkyboxPreview();

  /**
   * @brief Sets the current texture for the skybox.
   * @param texture_name The name of the texture to set for the skybox.
   */
  void setCurrentTexture(const QString& texture_name);

  ~SkyboxWidget() override; ///< Default destructor for the SkyboxWidget class.

signals:
  void displayInViewportToggled(bool checked);
  void skyboxChanged(const QString& new_skybox);
  void skyboxTextureCreated(const QString& texture_path);

private slots:
  void onTextureSelected(int index);
  void onBrowseButtonClicked();

private:
  Ui::SkyboxWidget* ui;
  Skybox*           m_skybox = nullptr;

  DefaultOptionProxyModel* m_textures_list_model = nullptr;
};

#endif // GUI_COMPONENTS_SKYBOXWIDGET_HPP