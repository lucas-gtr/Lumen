#include <memory>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <string>

#include "Core/Ray.hpp"
#include "GPU/OpenGL/EngineGL.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/View/RealtimeView.hpp"
#include "Rendering/RayIntersection.hpp"
#include "Scene/Scene.hpp"
#include "SceneObjects/Object3D.hpp"

RealtimeController::RealtimeController(RealtimeView* view, Scene* scene) : QObject(view), m_view(view), m_scene(scene) {
  connectSignals();
}

void RealtimeController::connectSignals() {
  connect(m_view, &RealtimeView::initializeRequested, this, &RealtimeController::onInitialize);
  connect(m_view, &RealtimeView::resized, this, &RealtimeController::onResize);
  connect(m_view, &RealtimeView::renderRequested, this, &RealtimeController::onRender);
  connect(m_view, &RealtimeView::wheelMoved, this, &RealtimeController::onWheel);
  connect(m_view, &RealtimeView::leftMouseClicked, this, &RealtimeController::onLeftMouseClick);
  connect(m_view, &RealtimeView::middleMouseDragged, this, &RealtimeController::onMouseDrag);
}

void RealtimeController::onInitialize() {
  m_engine = std::make_unique<EngineGL>(m_view->width(), m_view->height());
  m_engine->initialize(m_scene);
  m_engine->setupRendering();
}

void RealtimeController::onResize(int width, int height) { m_engine->setViewportSize(width, height); }

void RealtimeController::onRender(unsigned int framebuffer_id) { m_engine->renderFrame(framebuffer_id); }

void RealtimeController::onWheel(float delta) {
  auto* camera = m_engine->getResourceManager()->getCamera();
  camera->moveForward(delta * m_engine->cameraZoomSpeed());
}

void RealtimeController::onLeftMouseClick(int x, int y) {
  const Ray         selection_ray        = m_engine->getResourceManager()->getCamera()->getRayFromMousePosition(x, y);
  const std::string selected_object_name = RayIntersection::getObjectNameFromHit(selection_ray, m_scene);
  if(selected_object_name.empty()) {
    emit objectUnselected();
  } else {
    emit objectSelected(QString::fromStdString(selected_object_name));
  }
}

void RealtimeController::onMouseDrag(int dx, int dy, bool shift) {
  auto* camera = m_engine->getResourceManager()->getCamera();
  if(shift) {
    camera->moveRight(static_cast<float>(dx) * m_engine->cameraMoveSpeed());
    camera->moveUp(static_cast<float>(dy) * m_engine->cameraMoveSpeed());
  } else {
    camera->rotate(static_cast<float>(dx) * m_engine->cameraRotationSpeed(),
                   -static_cast<float>(dy) * m_engine->cameraRotationSpeed());
  }
}

void RealtimeController::selectObject(const QString& object_name) {
  Object3D* object = m_scene->getObject(object_name.toStdString());

  m_engine->getResourceManager()->setSelectedObject(object);
}

void RealtimeController::unselectObject() { m_engine->getResourceManager()->setSelectedObject(nullptr); }

void RealtimeController::bakeLights() {
  m_view->makeCurrent();
  m_engine->bakeLights();
  m_view->doneCurrent();
}

void RealtimeController::updateViewportCamera() { m_engine->getResourceManager()->getCamera()->updateViewportCamera(); }

void RealtimeController::updateRenderCamera() { m_engine->getResourceManager()->getCamera()->updateRenderCamera(); }

bool RealtimeController::shouldClose() const { return m_should_close; }

void RealtimeController::closeWindow() {
  m_should_close = true;
  m_view->close();
}
