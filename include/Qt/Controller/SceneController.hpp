#ifndef QT_CONTROLLER_SCENECONTROLLER_HPP
#define QT_CONTROLLER_SCENECONTROLLER_HPP

#include <qobject.h>
#include <qstring.h>

#include "Qt/View/SceneView.hpp"
#include "Scene/Scene.hpp"

class SceneController : public QObject {
  Q_OBJECT
public:
  explicit SceneController(SceneView* view, Scene* scene);

signals:
  void objectAdded(const QString& name);
  void lightAdded(const QString& name);

  void itemRenamed(const QString& new_name, const QString& old_name);

  void cancelRenamingObject(const QString& old_name, const QString& new_name);
  void cancelRenamingLight(const QString& old_name, const QString& new_name);

private:
  // Slots
  void onObjectRenamed(const QString& new_name, const QString& old_name);
  void onLightRenamed(const QString& new_name, const QString& old_name);

  // Members
  SceneView* m_view;
  Scene*     m_scene;

  void connectSignals();
};

#endif // QT_CONTROLLER_SCENECONTROLLER_HPP