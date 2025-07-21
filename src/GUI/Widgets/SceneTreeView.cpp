#include <QKeyEvent>
#include <QMessageBox>
#include <QSignalBlocker>

#include "Scene/Scene.hpp"
#include "SceneTreeView.hpp"

SceneTreeView::SceneTreeView(QWidget* parent) : QTreeView(parent), m_model(new QStandardItemModel(this)) {
  m_model->setHorizontalHeaderLabels({"Scene"});

  setModel(m_model);
  setHeaderHidden(false);
  setSelectionMode(QAbstractItemView::SingleSelection);

  connect(selectionModel(), &QItemSelectionModel::selectionChanged, this, &SceneTreeView::onSelectionChanged);
  connect(m_model, &QStandardItemModel::itemChanged, this, &SceneTreeView::onItemRenamed);
}

void SceneTreeView::initializeFromScene(Scene* scene) {
  m_scene = scene;
  m_model->clear();
  m_model->setHorizontalHeaderLabels({"Scene"});

  QStandardItem* root = m_model->invisibleRootItem();

  m_cameraItem = new QStandardItem("Camera");
  m_cameraItem->setFlags(m_cameraItem->flags() & ~Qt::ItemIsEditable);
  m_cameraItem->setData(static_cast<int>(ItemType::Camera), ItemRole::TypeRole);
  root->appendRow(m_cameraItem);

  m_objectsRoot = new QStandardItem("3D Objects");
  m_objectsRoot->setData(static_cast<int>(ItemType::Root), ItemRole::TypeRole);
  m_objectsRoot->setFlags(m_objectsRoot->flags() & ~Qt::ItemIsEditable);
  root->appendRow(m_objectsRoot);

  if(m_scene != nullptr) {
    for(const Object3D* obj : m_scene->getObjectList()) {
      const std::string name = m_scene->getObjectName(obj);
      addObject(QString::fromStdString(name));
    }
  }

  m_lightsRoot = new QStandardItem("Lights");
  m_lightsRoot->setData(static_cast<int>(ItemType::Root), ItemRole::TypeRole);
  m_lightsRoot->setFlags(m_lightsRoot->flags() & ~Qt::ItemIsEditable);
  root->appendRow(m_lightsRoot);

  if(m_scene != nullptr) {
    for(const Light* light : m_scene->getLightList()) {
      const std::string name = m_scene->getLightName(light);
      addLight(QString::fromStdString(name));
    }
  }

  expandAll();
}

QStandardItem* SceneTreeView::findItemByName(QStandardItem* parent, const QString& name) {
  for(int i = 0; i < parent->rowCount(); ++i) {
    QStandardItem* child = parent->child(i);
    if(child->data(ItemRole::NameRole).toString() == name) {
      return child;
    }
  }
  return nullptr;
}

void SceneTreeView::addObject(const QString& name) {
  if(m_objectsRoot == nullptr) {
    return;
  }
  auto* item = new QStandardItem(name);
  item->setData(static_cast<int>(ItemType::Object), ItemRole::TypeRole);
  item->setData(name, ItemRole::NameRole);
  m_objectsRoot->appendRow(item);
}

void SceneTreeView::removeObject(const QString& name) {
  if(m_objectsRoot == nullptr) {
    return;
  }
  QStandardItem* item = findItemByName(m_objectsRoot, name);
  if(item != nullptr) {
    m_objectsRoot->removeRow(item->row());
  }
}

void SceneTreeView::addLight(const QString& name) {
  if(m_lightsRoot == nullptr) {
    return;
  }
  auto* item = new QStandardItem(name);
  item->setData(static_cast<int>(ItemType::Light), ItemRole::TypeRole);
  item->setData(name, ItemRole::NameRole);
  m_lightsRoot->appendRow(item);
}

void SceneTreeView::removeLight(const QString& name) {
  if(m_lightsRoot == nullptr) {
    return;
  }
  QStandardItem* item = findItemByName(m_lightsRoot, name);
  if(item != nullptr) {
    m_lightsRoot->removeRow(item->row());
  }
}

void SceneTreeView::selectObject(const QString& name) {
  const QSignalBlocker blocker(this);
  if(m_objectsRoot == nullptr) {
    return;
  }
  QStandardItem* item = findItemByName(m_objectsRoot, name);
  if(item == nullptr) {
    return;
  }
  const QModelIndex index = item->index();
  if(!index.isValid()) {
    return;
  }
  selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
  setCurrentIndex(index);
}

void SceneTreeView::clearSelection() {
  const QSignalBlocker blocker(this);
  selectionModel()->clearSelection();
  clearFocus();
}

void SceneTreeView::keyPressEvent(QKeyEvent* event) {
  if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
    deleteSelectedItem();
  } else {
    QTreeView::keyPressEvent(event);
  }
}

void SceneTreeView::deleteSelectedItem() {
  const QModelIndex index = selectionModel()->currentIndex();
  if(!index.isValid() || selectionModel()->selectedIndexes().isEmpty()) {
    return;
  }
  const int     type = index.data(ItemRole::TypeRole).toInt();
  const QString name = index.data(ItemRole::NameRole).toString();

  if(type == static_cast<int>(ItemType::Object)) {
    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion", QString("Are you sure you want to delete object \"%1\"?").arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
      m_scene->removeObject(name.toStdString());
      removeObject(name);
    }
    return;
  }
  if(type == static_cast<int>(ItemType::Light)) {
    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion", QString("Are you sure you want to delete light \"%1\"?").arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
      m_scene->removeLight(name.toStdString());
      removeLight(name);
    }
    return;
  }
}

void SceneTreeView::onSelectionChanged(const QItemSelection& selected, const QItemSelection& /*unused*/) {
  if(selected.isEmpty()) {
    emit selectionCleared();
    return;
  }

  const QModelIndex index = selected.indexes().first();
  if(!index.isValid()) {
    emit selectionCleared();
    return;
  }

  const int     type = index.data(ItemRole::TypeRole).toInt();
  const QString name = index.data(ItemRole::NameRole).toString();

  switch(static_cast<ItemType>(type)) {
  case ItemType::Camera:
    emit cameraSelected();
    break;
  case ItemType::Object:
    emit objectSelected(name);
    break;
  case ItemType::Light:
    emit lightSelected(name);
    break;
  default:
    emit selectionCleared();
    break;
  }
}

void SceneTreeView::onItemRenamed(QStandardItem* item) {
  const int type = item->data(ItemRole::TypeRole).toInt();

  const QString new_name = item->text();
  const QString old_name = item->data(ItemRole::NameRole).toString();

  if(new_name == old_name) {
    return;
  }
  switch(static_cast<ItemType>(type)) {
  case ItemType::Object:
    if(m_scene->renameObject(old_name.toStdString(), new_name.toStdString())) {
      item->setData(new_name, ItemRole::NameRole);
    } else {
      item->setText(old_name);
    }
    break;
  case ItemType::Light:
    if(m_scene->renameLight(old_name.toStdString(), new_name.toStdString())) {
      item->setData(new_name, ItemRole::NameRole);
    } else {
      item->setText(old_name);
    }
    break;
  default:
    break;
  }
}
