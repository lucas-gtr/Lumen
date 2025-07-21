/**
 * @file TexturesListModel.hpp
 * @brief Header file for the TexturesListModel class, which provides a model for managing
 *        a list of textures in a Qt application.
 */
#ifndef GUI_WIDGETS_TEXTURESLISTMODEL_HPP
#define GUI_WIDGETS_TEXTURESLISTMODEL_HPP

#include <QAbstractListModel>
#include <QString>
#include <QVector>

class TextureManager;

/**
 * @class TexturesListModel
 * @brief A model for managing a list of textures in a Qt application.
 *
 * This class inherits from QAbstractListModel and provides functionality to manage
 * a list of textures, including adding, renaming, and removing textures.
 */
class TexturesListModel : public QAbstractListModel {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the TexturesListModel class.
   * @param texture_manager The TextureManager instance to manage textures.
   * @param parent The parent object for this model.
   */
  explicit TexturesListModel(TextureManager* texture_manager, QObject* parent = nullptr);

  /**
   * @brief Returns the number of rows in the model.
   * @param parent The parent index (default is an invalid index).
   * @return The number of rows in the model.
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the data for the specified index and role.
   * @param index The index for which to retrieve data.
   * @param role The role for which to retrieve data (default is Qt::DisplayRole).
   * @return The data for the specified index and role.
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Sets the data for the specified index and role.
   * @param index The index for which to set data.
   * @param value The value to set.
   * @param role The role for which to set data (default is Qt::EditRole).
   * @return True if the data was set successfully, false otherwise.
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /**
   * @brief Returns the flags for the specified index.
   * @param index The index for which to retrieve flags.
   * @return The item flags for the specified index.
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Adds a new texture item to the model.
   * @param item The name of the texture to add.
   */
  void addItem(const QString& item);

  /**
   * @brief Renames an existing texture item in the model.
   * @param row The row index of the item to rename.
   * @param new_name The new name for the texture.
   * @return True if the item was renamed successfully, false otherwise.
   */
  bool renameItem(int row, const QString& new_name);

  /**
   * @brief Removes an item from the model.
   * @param row The row index of the item to remove.
   */
  void removeItem(int row);

private:
  QVector<QString> m_items;
  TextureManager*  m_texture_manager = nullptr;
};

#endif // GUI_WIDGETS_TEXTURESLISTMODEL_HPP