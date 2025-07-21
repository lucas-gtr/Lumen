/**
 * @file MaterialsWidget.hpp
 * @brief Header file for the MaterialsWidget class, which provides a user interface for managing materials.
 */
#ifndef GUI_WIDGETS_MATERIALSWIDGET_HPP
#define GUI_WIDGETS_MATERIALSWIDGET_HPP

#include <QListView>
#include <QWidget>

#include "DefaultOptionProxyModel.hpp"
#include "MaterialsListModel.hpp"

class Material;
class MaterialManager;
class TextureManager;

namespace Ui {
class MaterialsWidget;
}

/**
 * @class MaterialsWidget
 * @brief A widget for displaying and editing materials.
 *
 * This class provides a user interface for managing materials, including adding, removing,
 * renaming materials, and selecting textures for diffuse and normal maps.
 */
class MaterialsWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the MaterialsWidget class.
   * @param parent The parent widget for this materials widget.
   */
  explicit MaterialsWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the MaterialManager instance for this widget.
   * @param material_manager The MaterialManager instance to set.
   */
  void setMaterialManager(MaterialManager* material_manager);

  /**
   * @brief Sets the TextureManager instance for this widget.
   * @param texture_manager The TextureManager instance to set.
   */
  void setTextureManager(TextureManager* texture_manager);

  /**
   * @brief Sets the model for the materials list view.
   * @param model The MaterialsListModel instance to set.
   */
  void setModel(MaterialsListModel* model);

  /**
   * @brief Sets the textures list model for the materials widget.
   * @param model The DefaultOptionProxyModel instance to set.
   */
  void setTexturesListModel(DefaultOptionProxyModel* model);

  /**
   * @brief Updates the materials widget to reflect the current state of materials and textures.
   */
  void updateWidget();

  /**
   * @brief Sets the diffuse texture for the current material.
   * @param texture_name The name of the texture to set as the diffuse texture.
   */
  void setDiffuseTexture(const QString& texture_name);

  /**
   * @brief Sets the normal texture for the current material.
   * @param texture_name The name of the texture to set as the normal texture.
   */
  void setNormalTexture(const QString& texture_name);

  ~MaterialsWidget() override; ///< Default destructor for the MaterialsWidget class.

signals:
  void diffuseTextureCreated(const QString& texture_path);
  void normalTextureCreated(const QString& texture_path);

private slots:
  void onAddButtonClicked();
  void onRemoveButtonClicked();
  void onMaterialSelectionChanged(const QItemSelection& selected, const QItemSelection&);
  void onMaterialRenamed(const QModelIndex& top_left, const QModelIndex& bottomRight, const QList<int>& roles);
  void onMaterialNameEdited();
  void onDiffuseTextureSelected(int index);
  void onBrowseDiffuseButtonClicked();
  void onNormalTextureSelected(int index);
  void onBrowseNormalButtonClicked();

private:
  Ui::MaterialsWidget* ui;
  MaterialManager*     m_material_manager = nullptr;
  TextureManager*      m_texture_manager  = nullptr;
  Material*            m_current_material = nullptr;

  MaterialsListModel*      m_materials_list_model = nullptr;
  DefaultOptionProxyModel* m_textures_list_model  = nullptr;

  void updateDiffuseTexturePreview();
  void updateNormalTexturePreview();
};

#endif // GUI_WIDGETS_MATERIALSWIDGET_HPP
