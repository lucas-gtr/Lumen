/**
 * @file ObjectWidget.hpp
 * @brief Header file for the ObjectWidget class, which provides a user interface for editing 3D object properties.
 */
#ifndef GUI_WIDGETS_OBJECTWIDGET_HPP
#define GUI_WIDGETS_OBJECTWIDGET_HPP

#include <QGroupBox>

#include "Core/CommonTypes.hpp"
#include "DefaultOptionProxyModel.hpp"

class Object3D;
class MaterialManager;
namespace Ui {
class ObjectWidget;
}

/**
 * @class ObjectWidget
 * @brief A widget for displaying and editing properties of a 3D object.
 *
 * This class provides a user interface for managing a 3D object, including its mesh, material,
 * and other properties. It allows users to select materials from a list and update the object's
 * properties accordingly.
 */
class ObjectWidget : public QGroupBox {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the ObjectWidget class.
   * @param parent The parent widget for this object widget.
   */
  explicit ObjectWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the 3D object for this widget.
   * @param object The Object3D instance to set.
   */
  void setObject3D(Object3D* object);

  /**
   * @brief Sets the materials list model for this widget.
   * @param model The DefaultOptionProxyModel instance to set as the materials list model.
   */
  void setMaterialsListModel(DefaultOptionProxyModel* model);

  /**
   * @brief Sets the material manager for this widget.
   * @param material_manager The MaterialManager instance to set.
   */
  void setMaterialManager(MaterialManager* material_manager);

  /**
   * @brief Updates the widget to reflect the current state of the object and materials.
   */
  void updateWidget();

  /**
   * @brief Sets the current material for the object.
   * @param material_name The name of the material to set as the current material.
   */
  void setCurrentMaterial(const QString& material_name);

  ~ObjectWidget() override; ///< Default destructor for the ObjectWidget class.

signals:
  void materialChanged(Object3D* object, const QString& new_material);

private slots:
  void onMaterialChanged(const QString& new_material);

private:
  Ui::ObjectWidget* ui;
  Object3D*         m_object           = nullptr;
  MaterialManager*  m_material_manager = nullptr;

  DefaultOptionProxyModel* m_materials_list_model = nullptr;
};

#endif // GUI_WIDGETS_OBJECTWIDGET_HPP
