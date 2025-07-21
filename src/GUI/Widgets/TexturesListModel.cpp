#include "Surface/TextureManager.hpp"
#include "TexturesListModel.hpp"

TexturesListModel::TexturesListModel(TextureManager* texture_manager, QObject* parent)
    : QAbstractListModel(parent), m_texture_manager(texture_manager) {}

int TexturesListModel::rowCount(const QModelIndex& parent) const {
  if(parent.isValid()) {
    return 0;
  }
  return static_cast<int>(m_items.size());
}

QVariant TexturesListModel::data(const QModelIndex& index, int role) const {
  if(!index.isValid() || index.row() >= m_items.size()) {
    return {};
  }
  if(role == Qt::DisplayRole || role == Qt::EditRole) {
    return m_items[index.row()];
  }

  return {};
}

bool TexturesListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  if(!index.isValid() || index.row() >= m_items.size() || role != Qt::EditRole) {
    return false;
  }
  const QString new_name = value.toString().trimmed();

  if(new_name.isEmpty() || new_name == m_items[index.row()]) {
    return false;
  }
  if(m_texture_manager->getAvailableTextureName(new_name.toStdString()) != new_name.toStdString()) {
    return false;
  }

  m_texture_manager->renameTexture(m_items[index.row()].toStdString(), new_name.toStdString());
  m_items[index.row()] = new_name;
  emit dataChanged(index, index, {role});
  return true;
}

Qt::ItemFlags TexturesListModel::flags(const QModelIndex& index) const {
  if(!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void TexturesListModel::addItem(const QString& item) {
  const std::string new_texture_name = m_texture_manager->getAvailableTextureName(item.toStdString());
  m_texture_manager->addTexture(new_texture_name);
  const int insert_row = static_cast<int>(m_items.size());
  beginInsertRows(QModelIndex(), insert_row, insert_row);
  m_items.append(QString::fromStdString(new_texture_name));
  endInsertRows();
}

bool TexturesListModel::renameItem(int row, const QString& new_name) {
  if(row < 0 || row >= m_items.size()) {
    return false;
  }
  if(m_texture_manager->getAvailableTextureName(new_name.toStdString()) != new_name.toStdString()) {
    return false;
  }

  m_texture_manager->renameTexture(m_items[row].toStdString(), new_name.toStdString());
  m_items[row] = new_name;
  emit dataChanged(index(row, 0), index(row, 0), {Qt::DisplayRole});
  return true;
}

void TexturesListModel::removeItem(int row) {
  if(row < 0 || row >= m_items.size()) {
    return;
  }
  beginRemoveRows(QModelIndex(), row, row);
  m_texture_manager->removeTexture(m_items[row].toStdString());
  m_items.removeAt(row);
  endRemoveRows();
}
