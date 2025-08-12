// GCOVR_EXCL_START
#include <QGuiApplication>
#include <memory>

#include "GPU/OpenGL/EngineGL.hpp"
#include "OpenGLWidget.hpp"
#include "Rendering/PathTracer/RayIntersection.hpp"
#include "Scene/Scene.hpp"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), m_scene(nullptr), m_engine(nullptr), m_refresh_timer(new QTimer(this)) {
  connect(m_refresh_timer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&QWidget::update));

  m_refresh_timer->start(OPENGL_REFRESH_RATE);
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  m_engine = std::make_unique<EngineGL>(width(), height());
  m_engine->initialize(m_scene);
  m_engine->setupRendering();

  emit engineInitialized(m_engine.get());
}

void OpenGLWidget::resizeGL(int w, int h) {
  if(m_scene != nullptr) {
    m_engine->setViewportSize(w, h);
  }
}

void OpenGLWidget::paintGL() {
  if(m_scene != nullptr) {
    m_engine->renderFrame(defaultFramebufferObject());
  }
}

void OpenGLWidget::setScene(Scene* scene) { m_scene = scene; }

void OpenGLWidget::updateViewportCamera() {
  if(m_scene != nullptr) {
    m_engine->getResourceManager()->getCamera()->updateViewportCamera();
  }
}

void OpenGLWidget::updateRenderCamera() {
  if(m_scene != nullptr) {
    m_engine->getResourceManager()->getCamera()->updateRenderCamera();
  }
}

void OpenGLWidget::selectObject(const QString& object_name) {
  if(m_scene != nullptr) {
    Object3D* object = m_scene->getObject(object_name.toStdString());

    m_engine->getResourceManager()->setSelectedObject(object);
  }
}

void OpenGLWidget::unselectObject() {
  if(m_scene != nullptr) {
    m_engine->getResourceManager()->setSelectedObject(nullptr);
  }
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
  const float delta = static_cast<float>(event->angleDelta().y()) / WHEEL_TICK_ANGLE;

  auto* camera = m_engine->getResourceManager()->getCamera();
  camera->moveForward(delta * m_engine->cameraZoomSpeed());
}

void OpenGLWidget::selectObjectFromClick(int x, int y) {
  const Ray         selection_ray        = m_engine->getResourceManager()->getCamera()->getRayFromMousePosition(x, y);
  const std::string selected_object_name = RayIntersection::getObjectNameFromHit(selection_ray, m_scene);
  if(selected_object_name.empty()) {
    emit objectUnselected();
  } else {
    emit objectSelected(QString::fromStdString(selected_object_name));
  }
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::MiddleButton) {
    m_last_mouse_pos = event->pos();
  } else if(event->button() == Qt::LeftButton) {
    selectObjectFromClick(event->pos().x(), event->pos().y());
  }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if((event->buttons() & Qt::MiddleButton) != 0U) {
    const int  dx    = m_last_mouse_pos.x() - event->pos().x();
    const int  dy    = event->pos().y() - m_last_mouse_pos.y();
    const bool shift = (QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) != 0U;

    auto* camera = m_engine->getResourceManager()->getCamera();
    if(shift) {
      camera->moveRight(static_cast<float>(dx) * m_engine->cameraMoveSpeed());
      camera->moveUp(static_cast<float>(dy) * m_engine->cameraMoveSpeed());
    } else {
      camera->rotate(static_cast<float>(dx) * m_engine->cameraRotationSpeed(),
                     -static_cast<float>(dy) * m_engine->cameraRotationSpeed());
    }
  }
  m_last_mouse_pos = event->pos();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event) {
  if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
    emit objectDeleted();
  } else {
    QOpenGLWidget::keyPressEvent(event);
  }
}
// GCOVR_EXCL_STOP