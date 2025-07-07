/**
 * @file OpenGLContext.hpp
 * @brief Header file for the OpenGLContext class, which manages the OpenGL context in a Qt application.
 */
#ifndef QT_OPENGLCONTEXT_HPP
#define QT_OPENGLCONTEXT_HPP

#include <qopenglwidget.h>

/**
 * @class OpenGLContext
 * @brief Singleton class to manage the OpenGL context in a Qt application.
 *
 * This class provides methods to set the current OpenGL widget, make the context current, and release it.
 */
class OpenGLContext {
public:
  OpenGLContext(const OpenGLContext&)            = delete;
  OpenGLContext& operator=(const OpenGLContext&) = delete;
  OpenGLContext(OpenGLContext&&)                 = delete;
  OpenGLContext& operator=(OpenGLContext&&)      = delete;

  /**
   * @brief Gets the singleton instance of OpenGLContext.
   * @return Reference to the singleton instance of OpenGLContext.
   */
  static OpenGLContext& Instance() {
    static OpenGLContext instance;
    return instance;
  }

  /**
   * @brief Sets the OpenGL widget for the context.
   * @param widget Pointer to the QOpenGLWidget to set.
   */
  void setWidget(QOpenGLWidget* widget) {
    if(m_opengl_widget == nullptr && widget != nullptr) {
      m_opengl_widget = widget;
    }
  }

  /**
   * @brief Makes the OpenGL context current.
   */
  void makeContextCurrent() {
    if(m_opengl_widget != nullptr) {
      m_opengl_widget->makeCurrent();
    }
  }

  /**
   * @brief Releases the current OpenGL context.
   */
  void doneContext() {
    if(m_opengl_widget != nullptr) {
      m_opengl_widget->doneCurrent();
    }
  }

private:
  QOpenGLWidget* m_opengl_widget = nullptr;

  OpenGLContext() = default;

  ~OpenGLContext() = default;
};

#endif // QT_OPENGLCONTEXT_HPP
