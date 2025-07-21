/**
 * @file MaterialsListModel.hpp
 * @brief Header file for the MaterialsListModel class, which provides a model for displaying materials in a list.
 */
#ifndef GUI_WIDGETS_MATERIALSLISTMODEL_HPP
#define GUI_WIDGETS_MATERIALSLISTMODEL_HPP

#include <QAbstractListModel>
#include <QString>
#include <QVector>

class MaterialManager;

/**
 * @class MaterialsListModel
 * @brief A model for managing and displaying a list of materials.
 *
 * This class inherits from QAbstractListModel and provides functionality to add, rename, and remove materials.
 * It interacts with the MaterialManager to manage the materials and notifies views of changes.
 */
class MaterialsListModel : public QAbstractListModel {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the MaterialsListModel class.
   * @param material_manager The MaterialManager instance to manage materials.
   * @param parent The parent object for this model.
   */
  explicit MaterialsListModel(MaterialManager* material_manager, QObject* parent = nullptr);

  /**
   * @brief Returns the number of rows in the model.
   * @param parent The parent index (default is an invalid index).
   * @return The number of rows in the model.
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the data for the specified index and role.
   * @param index The index for which to retrieve data.
   * @param role The role of the data to retrieve (default is Qt::DisplayRole).
   * @return The data for the specified index and role.
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Sets the data for the specified index and role.
   * @param index The index for which to set data.
   * @param value The value to set.
   * @param role The role of the data (default is Qt::EditRole).
   * @return True if the data was set successfully, false otherwise.
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /**
   * @brief Returns the flags for the specified index.
   * @param index The index for which to retrieve flags.
   * @return The flags for the specified index.
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Adds a new material to the model.
   * @param item The name of the material to add.
   */
  void addItem(const QString& item);

  /**
   * @brief Renames a material in the model.
   * @param row The row of the material to rename.
   * @param new_name The new name for the material.
   * @return True if the material was renamed successfully, false otherwise.
   */
  bool renameItem(int row, const QString& new_name);

  /**
   * @brief Removes a material from the model.
   * @param row The row of the material to remove.
   */
  void removeItem(int row);

private:
  QVector<QString> m_items;
  MaterialManager* m_material_manager = nullptr;
};

#endif // GUI_WIDGETS_TEXTURESLISTMODEL_HPP