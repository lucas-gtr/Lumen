#include <GL/gl.h>
#include <iostream>
#include <qevent.h>
#include <qguiapplication.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qopenglwidget.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/OpenGLContext.hpp"
#include "Qt/View/RealtimeView.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
RealtimeView::RealtimeView(QWidget* parent) : QOpenGLWidget(parent), m_refresh_timer(new QTimer(this)) {
  m_refresh_timer->start(OPENGL_REFRESH_RATE);

  connectSignals();
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void RealtimeView::connectSignals() {
  connect(m_refresh_timer, &QTimer::timeout, this, static_cast<void (QWidget::*)()>(&QWidget::update));
}

void RealtimeView::initializeGL() {
  initializeOpenGLFunctions();
  std::cout << "OpenGL initialized. Version: " << glGetString(GL_VERSION) << '\n';
  emit initializeRequested();

  OpenGLContext::Instance().setWidget(this);
}

void RealtimeView::resizeGL(int width, int height) { emit resized(width, height); }

void RealtimeView::paintGL() { emit renderRequested(defaultFramebufferObject()); }

void RealtimeView::wheelEvent(QWheelEvent* event) {
  const float delta = static_cast<float>(event->angleDelta().y()) / WHEEL_TICK_ANGLE;
  emit        wheelMoved(delta);
}

void RealtimeView::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::MiddleButton) {
    m_last_mouse_pos = event->pos();
  } else if(event->button() == Qt::LeftButton) {
    emit leftMouseClicked(event->pos().x(), event->pos().y());
  }
}

void RealtimeView::mouseMoveEvent(QMouseEvent* event) {
  if((event->buttons() & Qt::MiddleButton) != 0U) {
    const int  dx    = m_last_mouse_pos.x() - event->pos().x();
    const int  dy    = event->pos().y() - m_last_mouse_pos.y();
    const bool shift = (QGuiApplication::keyboardModifiers() & Qt::ShiftModifier) != 0U;
    emit       middleMouseDragged(dx, dy, shift);
  }
  m_last_mouse_pos = event->pos();
}

void RealtimeView::keyPressEvent(QKeyEvent* event) {}
