// GCOVR_EXCL_START
#include "MaterialsListModel.hpp"
#include "Surface/MaterialManager.hpp"

MaterialsListModel::MaterialsListModel(MaterialManager* material_manager, QObject* parent)
    : QAbstractListModel(parent), m_material_manager(material_manager) {
  const auto materials = m_material_manager->getAllMaterialsName();
  m_items.reserve(materials.size());
  for(const auto& material_name : materials) {
    m_items.push_back(QString::fromStdString(material_name));
  }
}

int MaterialsListModel::rowCount(const QModelIndex& parent) const {
  if(parent.isValid()) {
    return 0;
  }
  return static_cast<int>(m_items.size());
}

QVariant MaterialsListModel::data(const QModelIndex& index, int role) const {
  if(!index.isValid() || index.row() >= m_items.size()) {
    return {};
  }
  if(role == Qt::DisplayRole || role == Qt::EditRole) {
    return m_items[index.row()];
  }

  return {};
}

bool MaterialsListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  if(!index.isValid() || index.row() >= m_items.size() || role != Qt::EditRole) {
    return false;
  }
  const QString new_name = value.toString().trimmed();

  if(new_name.isEmpty() || new_name == m_items[index.row()]) {
    return false;
  }
  if(m_material_manager->getAvailableMaterialName(new_name.toStdString()) != new_name.toStdString()) {
    return false;
  }

  m_material_manager->renameMaterial(m_items[index.row()].toStdString(), new_name.toStdString());
  m_items[index.row()] = new_name;
  emit dataChanged(index, index, {role});
  return true;
}

Qt::ItemFlags MaterialsListModel::flags(const QModelIndex& index) const {
  if(!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void MaterialsListModel::addItem(const QString& item) {
  const std::string new_material_name = m_material_manager->getAvailableMaterialName(item.toStdString());
  m_material_manager->addMaterial(new_material_name);
  const int insert_row = static_cast<int>(m_items.size());
  beginInsertRows(QModelIndex(), insert_row, insert_row);
  m_items.push_back(QString::fromStdString(new_material_name));
  endInsertRows();
}

bool MaterialsListModel::renameItem(int row, const QString& new_name) {
  if(row < 0 || row >= m_items.size()) {
    return false;
  }
  if(m_material_manager->getAvailableMaterialName(new_name.toStdString()) != new_name.toStdString()) {
    return false;
  }

  m_material_manager->renameMaterial(m_items[row].toStdString(), new_name.toStdString());
  m_items[row] = new_name;
  emit dataChanged(index(row, 0), index(row, 0), {Qt::DisplayRole});
  return true;
}

void MaterialsListModel::removeItem(int row) {
  if(row < 0 || row >= m_items.size()) {
    return;
  }
  beginRemoveRows(QModelIndex(), row, row);
  m_material_manager->removeMaterial(m_items[row].toStdString());
  m_items.erase(m_items.begin() + row);
  endRemoveRows();
}
// GCOVR_EXCL_STOP