#include "DefaultOptionProxyModel.hpp"

DefaultOptionProxyModel::DefaultOptionProxyModel(QObject* parent) : QAbstractProxyModel(parent) {}

void DefaultOptionProxyModel::setSourceModel(QAbstractItemModel* source_model) {
  beginResetModel();
  QAbstractProxyModel::setSourceModel(source_model);

  if(source_model != nullptr) {
    connect(source_model, &QAbstractItemModel::dataChanged, this, &QAbstractItemModel::dataChanged);
    connect(source_model, &QAbstractItemModel::rowsInserted, this, &QAbstractItemModel::rowsInserted);
    connect(source_model, &QAbstractItemModel::rowsRemoved, this, &QAbstractItemModel::rowsRemoved);
    connect(source_model, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);
    connect(source_model, &QAbstractItemModel::layoutChanged, this, &QAbstractItemModel::layoutChanged);
  }
  endResetModel();
}

QModelIndex DefaultOptionProxyModel::index(int row, int column, const QModelIndex& parent) const {
  if(parent.isValid() || row < 0 || row >= rowCount() || column != 0) {
    return {};
  }
  return createIndex(row, column);
}

QModelIndex DefaultOptionProxyModel::parent(const QModelIndex& /*child*/) const { return {}; }

int DefaultOptionProxyModel::rowCount(const QModelIndex& parent) const {
  if(parent.isValid() || sourceModel() == nullptr) {
    return 0;
  }
  return sourceModel()->rowCount() + 1;
}

int DefaultOptionProxyModel::columnCount(const QModelIndex& parent) const {
  return (sourceModel() != nullptr) ? sourceModel()->columnCount(parent) : 1;
}

QVariant DefaultOptionProxyModel::data(const QModelIndex& index, int role) const {
  if(!index.isValid() || index.column() != 0) {
    return {};
  }
  if(index.row() == 0) {
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
      return QStringLiteral("Default");
    }
    return {};
  }

  const QModelIndex source_index = sourceModel()->index(index.row() - 1, index.column());
  return sourceModel()->data(source_index, role);
}

Qt::ItemFlags DefaultOptionProxyModel::flags(const QModelIndex& index) const {
  if(!index.isValid()) {
    return Qt::NoItemFlags;
  }
  if(index.row() == 0) {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  }
  const QModelIndex source_index = sourceModel()->index(index.row() - 1, index.column());
  return sourceModel()->flags(source_index);
}

QModelIndex DefaultOptionProxyModel::mapToSource(const QModelIndex& proxy_index) const {
  if(!proxy_index.isValid() || proxy_index.row() == 0) {
    return {};
  }
  return sourceModel()->index(proxy_index.row() - 1, proxy_index.column());
}

QModelIndex DefaultOptionProxyModel::mapFromSource(const QModelIndex& source_index) const {
  if(!source_index.isValid()) {
    return {};
  }
  return index(source_index.row() + 1, source_index.column());
}
