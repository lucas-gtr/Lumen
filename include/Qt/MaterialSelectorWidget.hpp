/**
 * @file MaterialSelectorWidget.hpp
 * @brief Header file for the MaterialSelectorWidget class, which provides a UI for selecting materials.
 */
#ifndef QT_MATERIALSELECTORWIDGET_HPP
#define QT_MATERIALSELECTORWIDGET_HPP

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>

#include "Surface/MaterialManager.hpp"

/**
 * @class MaterialSelectorWidget
 * @brief A widget for selecting materials from a dropdown list.
 *
 * This widget allows users to select a material from a list, add new materials,
 * and remove existing ones. It connects to a MaterialManager to manage the materials.
 */
class MaterialSelectorWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for MaterialSelectorWidget.
   * @param default_material_name The name of the default material to select initially.
   * @param parent The parent widget.
   */
  explicit MaterialSelectorWidget(const std::string_view& default_material_name, QWidget* parent);

  /**
   * @brief Connects the material manager to this widget.
   * @param material_manager Pointer to the MaterialManager instance.
   */
  void connectToMaterialManager(MaterialManager* material_manager);

  /**
   * @brief Sets the current material in the selector.
   * @param material_name The name of the material to set as current.
   */
  void setCurrentMaterial(const QString& material_name);

signals:
  void materialSelected(const QString& material_name);

private:
  // Slots
  void onMaterialAdded(const std::string& material_name);
  void onMaterialRemoved(const std::string& material_name);
  void onMaterialRenamed(const std::string& old_name, const std::string& new_name);

  // Members
  QComboBox*   m_material_selector      = nullptr;
  QPushButton* m_browse_material_button = nullptr;

  std::string m_default_material_name;

  void setupUI();
  void connectSignals();
};

#endif // QT_MATERIALSELECTORWIDGET_HPP