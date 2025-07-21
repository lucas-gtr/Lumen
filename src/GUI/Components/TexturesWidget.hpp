/**
 * @file TexturesWidget.hpp
 * @brief Header file for the TexturesWidget class, which provides a user interface for managing
 *        textures in a 3D scene.
 */
#ifndef GUI_WIDGETS_TEXTURESWIDGET_HPP
#define GUI_WIDGETS_TEXTURESWIDGET_HPP

#include <QListView>
#include <QWidget>

#include "Core/CommonTypes.hpp"
#include "TexturesListModel.hpp"

class Texture;
class TextureManager;

namespace Ui {
  class TexturesWidget;
}

/**
 * @class TexturesWidget
 * @brief A widget for displaying and editing properties of textures.
 *
 * This class provides a user interface for managing textures, including adding, removing,
 * renaming, and configuring texture properties such as type, color, path, and sampling modes.
 */
class TexturesWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the TexturesWidget class.
   * @param parent The parent widget for this textures widget.
   */
  explicit TexturesWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the texture manager for this widget.
   * @param texture_manager The TextureManager instance to set.
   */
  void setTextureManager(TextureManager* texture_manager);
  
  /**
   * @brief Sets the model for the textures list view.
   * @param model The TexturesListModel instance to set as the model for the textures list view.
   */
  void setModel(TexturesListModel* model);

  /**
   * @brief Updates the widget to reflect the current state of textures and their properties.
   */
  void updateWidget();

  ~TexturesWidget() override; ///< Default destructor for the TexturesWidget class.

private slots:
  void onAddButtonClicked();
  void onRemoveButtonClicked();
  void onTextureSelectionChanged(const QItemSelection& selected, const QItemSelection&);
  void onTextureRenamed(const QModelIndex& top_left, const QModelIndex& bottom_right, const QList<int>& roles);
  void onTextureNameEdited();
  void onTextureTypeChanged(const QString& type);
  void onColorChanged(const ColorRGB& new_color);
  void onTexturePathChanged(const QString& new_path);
  void onFlipVerticallyToggled(bool checked);
  void onColorSpaceChanged(const QString& color_space);
  void onFilteringModeChanged(const QString& mode);
  void onWrappingModeChanged(const QString& mode);
  void onBorderColorChanged(const ColorRGB& new_color);

private:
  Ui::TexturesWidget* ui;
  TextureManager*     m_texture_manager = nullptr;
  Texture*            m_current_texture = nullptr;

  TexturesListModel* m_textures_list_model = nullptr;

  void updateTexturePreview();
};

#endif // GUI_WIDGETS_TEXTURESWIDGET_HPP
