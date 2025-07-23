/**
 * @file MaterialsWidget.hpp
 * @brief Header file for the MaterialsWidget class, which provides a user interface for managing materials.
 */
#ifndef GUI_WIDGETS_MATERIALSWIDGET_HPP
#define GUI_WIDGETS_MATERIALSWIDGET_HPP

#include <QComboBox>
#include <QListView>
#include <QPushButton>
#include <QWidget>
#include <functional>

#include "DefaultOptionProxyModel.hpp"
#include "DoubleSliderSpinBox.hpp"
#include "MaterialsListModel.hpp"
#include "TexturePreview.hpp"

class Material;
class MaterialManager;
class TextureManager;

namespace Ui {
class MaterialsWidget;
}

struct TextureBinding {
  QComboBox*                            combo_box;
  QPushButton*                          browse_button;
  TexturePreview*                       texture_preview;
  Texture*                              default_texture;
  std::function<void(const QString&)>   emitTextureCreatedSignal;
  std::function<void(Texture* texture)> materialTextureSetter;
};

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
   * @brief Sets the texture binding for the diffuse texture.
   * @param binding The TextureBinding instance to set for the diffuse texture.
   */
  void setupTextureBinding(TextureBinding* binding);

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
  static void setTexture(QComboBox* combo_box, const QString& name);

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

  /**
   * @brief Sets the emissive texture for the current material.
   * @param texture_name The name of the texture to set as the emissive texture.
   */
  void setEmissiveTexture(const QString& texture_name);

  /**
   * @brief Sets the roughness texture for the current material.
   * @param texture_name The name of the texture to set as the roughness texture.
   */
  void setRoughnessTexture(const QString& texture_name);

  /**
   * @brief Sets the metallic texture for the current material.
   * @param texture_name The name of the texture to set as the metallic texture.
   */
  void setMetalTexture(const QString& texture_name);

  ~MaterialsWidget() override; ///< Default destructor for the MaterialsWidget class.

signals:
  void diffuseTextureCreated(const QString& texture_path);
  void normalTextureCreated(const QString& texture_path);
  void emissiveTextureCreated(const QString& texture_path);
  void roughnessTextureCreated(const QString& texture_path);
  void metalTextureCreated(const QString& texture_path);

private slots:
  void onAddButtonClicked();
  void onRemoveButtonClicked();
  void onMaterialSelectionChanged(const QItemSelection& selected, const QItemSelection&);
  void onMaterialRenamed(const QModelIndex& top_left, const QModelIndex& bottomRight, const QList<int>& roles);
  void onMaterialNameEdited();
  void onEmissiveStrengthChanged(double value);
  void onRoughnessValueChanged(double value);
  void onUseTextureRoughnessChanged(bool checked);
  void onMetallicValueChanged(double value);
  void onUseTextureMetallicChanged(bool checked);

private:
  Ui::MaterialsWidget* ui;
  MaterialManager*     m_material_manager = nullptr;
  TextureManager*      m_texture_manager  = nullptr;
  Material*            m_current_material = nullptr;

  MaterialsListModel*      m_materials_list_model = nullptr;
  DefaultOptionProxyModel* m_textures_list_model  = nullptr;

  TextureBinding m_diffuse_binding;
  TextureBinding m_normal_binding;
  TextureBinding m_emissive_binding;
  TextureBinding m_roughness_binding;
  TextureBinding m_metal_binding;

  void        initializeTextureBindings();
  void        setPreviewsMaximumSize(int size);
  static void setupDoubleSliderSpinBox(DoubleSliderSpinBox* slider_spin_box, double min_value, double max_value);
  void        setupConnections();

  void        updateTextureWidget(QComboBox* combo_box, TexturePreview* preview_label, Texture* texture);
  static void updateTexturePreview(Texture* texture, TexturePreview* preview_label);
};

#endif // GUI_WIDGETS_MATERIALSWIDGET_HPP
