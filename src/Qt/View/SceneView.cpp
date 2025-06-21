#include <qabstractitemview.h>
#include <qboxlayout.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qtreewidget.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/View/SceneView.hpp"

SceneView::SceneView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void SceneView::setupUI() {
  m_scene_tree = new QTreeWidget(this);
  m_scene_tree->setHeaderHidden(true);
  m_scene_tree->setSelectionMode(QAbstractItemView::SingleSelection);
  m_scene_tree->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
  m_scene_tree->setIndentation(HIERARCHY_TREE_INDENTATION);

  m_camera_item = new QTreeWidgetItem(m_scene_tree);
  m_camera_item->setText(0, "Camera");
  m_camera_item->setData(0, Qt::UserRole, "Camera");

  m_objects_category = new QTreeWidgetItem(m_scene_tree);
  m_objects_category->setText(0, "Objects");
  m_objects_category->setFlags(Qt::ItemIsEnabled);
  m_objects_category->setExpanded(true);

  m_lights_category = new QTreeWidgetItem(m_scene_tree);
  m_lights_category->setText(0, "Lights");
  m_lights_category->setFlags(Qt::ItemIsEnabled);
  m_lights_category->setExpanded(true);

  auto* layout = new QVBoxLayout;
  layout->addWidget(m_scene_tree);
  setLayout(layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void SceneView::connectSignals() {
  connect(m_scene_tree, &QTreeWidget::itemSelectionChanged, this, &SceneView::onItemSelectionChanged);
  connect(m_scene_tree, &QTreeWidget::itemChanged, this, &SceneView::onItemChanged);
}

void SceneView::onItemSelectionChanged() {
  auto selected_items = m_scene_tree->selectedItems();
  if(selected_items.isEmpty()) {
    emit selectionCleared();
    return;
  }

  QTreeWidgetItem* item      = selected_items.first();
  const QString    item_type = item->data(0, Qt::UserRole).toString();

  if(item_type == "Camera") {
    emit cameraSelected();
  } else if(item_type == "Light") {
    emit lightSelected(item->text(0));
  } else if(item_type == "Object") {
    emit objectSelected(item->text(0));
  }
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void SceneView::onObjectAdded(const QString& name) {
  auto* item = new QTreeWidgetItem(m_objects_category);
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
  item->setText(0, name);
  const QSignalBlocker blocker(m_scene_tree);
  item->setData(0, Qt::UserRole, "Object");
  m_objects_category->addChild(item);
}

void SceneView::onLightAdded(const QString& name) {
  auto* item = new QTreeWidgetItem(m_lights_category);
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
  item->setText(0, name);
  const QSignalBlocker blocker(m_scene_tree);
  item->setData(0, Qt::UserRole, "Light");
  m_lights_category->addChild(item);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void SceneView::onItemChanged(QTreeWidgetItem* item, int column) {
  if(column != 0) {
    return;
  }
  const QSignalBlocker blocker(m_scene_tree);
  const QString        old_name = item->data(0, Qt::UserRole + 1).toString();
  const QString        new_name = item->text(0);
  item->setData(0, Qt::UserRole + 1, new_name);

  if(item->data(0, Qt::UserRole).toString() == "Object") {
    emit objectRenamed(new_name, old_name);
  } else if(item->data(0, Qt::UserRole).toString() == "Light") {
    emit lightRenamed(new_name, old_name);
  }
}

void SceneView::onCancelRenamingObject(const QString& old_name, const QString& new_name) {
  const QSignalBlocker blocker(m_scene_tree);
  auto                 items = m_scene_tree->findItems(new_name, Qt::MatchExactly | Qt::MatchRecursive);
  if(!items.isEmpty()) {
    QTreeWidgetItem* item = items.first();
    item->setText(0, old_name);
    item->setData(0, Qt::UserRole + 1, old_name);
  }
}

void SceneView::onCancelRenamingLight(const QString& old_name, const QString& new_name) {
  const QSignalBlocker blocker(m_scene_tree);
  auto                 items = m_scene_tree->findItems(new_name, Qt::MatchExactly | Qt::MatchRecursive);
  if(!items.isEmpty()) {
    QTreeWidgetItem* item = items.first();
    item->setText(0, old_name);
    item->setData(0, Qt::UserRole + 1, old_name);
  }
}

void SceneView::setSelectedObject(const QString& object_name) {
  const QSignalBlocker blocker(m_scene_tree);
  auto                 items = m_scene_tree->findItems(object_name, Qt::MatchExactly | Qt::MatchRecursive);
  if(!items.isEmpty()) {
    m_scene_tree->clearSelection();
    items.first()->setSelected(true);
    m_scene_tree->scrollToItem(items.first());
  }
}

void SceneView::clearSelection() {
  const QSignalBlocker blocker(m_scene_tree);
  m_scene_tree->clearSelection();
}