#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <string>

#include "Qt/Controller/SceneController.hpp"
#include "Qt/View/SceneView.hpp"
#include "Scene/Scene.hpp"

SceneController::SceneController(SceneView* view, Scene* scene) : QObject(view), m_view(view), m_scene(scene) {
  connectSignals();
}

void SceneController::connectSignals() {
  m_scene->getObjectAddedNameObserver().add(
      [this](const std::string& name) { emit objectAdded(QString::fromStdString(name)); });
  m_scene->getLightAddedNameObserver().add(
      [this](const std::string& name) { emit lightAdded(QString::fromStdString(name)); });

  connect(this, &SceneController::objectAdded, m_view, &SceneView::onObjectAdded);
  connect(this, &SceneController::lightAdded, m_view, &SceneView::onLightAdded);
  connect(m_view, &SceneView::objectRenamed, this, &SceneController::onObjectRenamed);
  connect(m_view, &SceneView::lightRenamed, this, &SceneController::onLightRenamed);

  connect(this, &SceneController::cancelRenamingObject, m_view, &SceneView::onCancelRenamingObject);
  connect(this, &SceneController::cancelRenamingLight, m_view, &SceneView::onCancelRenamingLight);
}

void SceneController::onObjectRenamed(const QString& new_name, const QString& old_name) {
  if(m_scene->renameObject(old_name.toStdString(), new_name.toStdString())) {
    emit itemRenamed(new_name, old_name);
  } else {
    emit cancelRenamingObject(old_name, new_name);
  }
}

void SceneController::onLightRenamed(const QString& new_name, const QString& old_name) {
  if(m_scene->renameLight(old_name.toStdString(), new_name.toStdString())) {
    emit itemRenamed(new_name, old_name);
  } else {
    emit cancelRenamingLight(old_name, new_name);
  }
}