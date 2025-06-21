#ifndef QT_VIEW_SCENEVIEW_HPP
#define QT_VIEW_SCENEVIEW_HPP

#include <qboxlayout.h>
#include <qtreewidget.h>
#include <qwidget.h>

class SceneView : public QWidget {
  Q_OBJECT
public:
  explicit SceneView(QWidget* parent);

  void setSelectedObject(const QString& object_name);
  void clearSelection();

  // Slots
  void onObjectAdded(const QString& name);
  void onLightAdded(const QString& name);

  void onCancelRenamingObject(const QString& old_name, const QString& new_name);
  void onCancelRenamingLight(const QString& old_name, const QString& new_name);

signals:
  void cameraSelected();
  void lightSelected(const QString& light_name);
  void objectSelected(const QString& object_name);
  void selectionCleared();
  void objectRenamed(const QString& new_name, const QString& old_name);
  void lightRenamed(const QString& new_name, const QString& old_name);

private:
  // Slots
  void onItemSelectionChanged();
  void onItemChanged(QTreeWidgetItem* item, int column);

  // Members
  QTreeWidget* m_scene_tree = nullptr;

  QTreeWidgetItem* m_camera_item      = nullptr;
  QTreeWidgetItem* m_objects_category = nullptr;
  QTreeWidgetItem* m_lights_category  = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_SCENEVIEW_HPP