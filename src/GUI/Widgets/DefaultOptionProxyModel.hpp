/**
 * @file DefaultOptionProxyModel.hpp
 * @brief Header file for the DefaultOptionProxyModel class, which provides a proxy model for
 *        displaying a default option in a list.
 */
#ifndef GUI_WIDGETS_DEFAULTOPTIONPROXYMODEL_HPP
#define GUI_WIDGETS_DEFAULTOPTIONPROXYMODEL_HPP

#include <QAbstractProxyModel>

/**
 * @class DefaultOptionProxyModel
 * @brief A proxy model that adds a default option at the top of the list.
 */
class DefaultOptionProxyModel : public QAbstractProxyModel {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the DefaultOptionProxyModel class.
   * @param parent The parent object for this proxy model.
   */
  explicit DefaultOptionProxyModel(QObject* parent = nullptr);

  /**
   * @brief Sets the source model for this proxy model.
   * @param source_model The source model to set.
   */
  void setSourceModel(QAbstractItemModel* source_model) override;

  /**
   * @brief Creates an index for the specified row and column in the proxy model.
   * @param row The row number.
   * @param column The column number.
   * @param parent The parent index (default is an invalid index).
   * @return The created index.
   */
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the parent index of the specified child index.
   * @param child The child index for which to find the parent.
   * @return The parent index of the specified child.
   */
  QModelIndex parent(const QModelIndex& child) const override;

  /**
   * @brief Returns the number of rows in the proxy model.
   * @param parent The parent index (default is an invalid index).
   * @return The number of rows in the proxy model.
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the number of columns in the proxy model.
   * @param parent The parent index (default is an invalid index).
   * @return The number of columns in the proxy model.
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  /**
   * @brief Returns the data for the specified index and role.
   * @param index The index for which to retrieve data.
   * @param role The role of the data to retrieve (default is Qt::DisplayRole).
   * @return The data for the specified index and role.
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * @brief Returns the item flags for the specified index.
   * @param index The index for which to retrieve item flags.
   * @return The item flags for the specified index.
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * @brief Maps a proxy index to a source index.
   * @param proxy_index The index in the proxy model.
   * @return The corresponding index in the source model.
   */
  QModelIndex mapToSource(const QModelIndex& proxy_index) const override;

  /**
   * @brief Maps a source index to a proxy index.
   * @param source_index The index in the source model.
   * @return The corresponding index in the proxy model.
   */
  QModelIndex mapFromSource(const QModelIndex& source_index) const override;
};

#endif // GUI_WIDGETS_DEFAULTOPTIONPROXYMODEL_HPP