/**
 * @file OpenGLWidget.hpp
 * @brief Header file for the OpenGLWidget class, which provides an OpenGL rendering context
 *        for rendering a scene.
 */
#ifndef GUI_WIDGETS_OPENGLWIDGET_HPP
#define GUI_WIDGETS_OPENGLWIDGET_HPP

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPoint>
#include <QTimer>
#include <QWheelEvent>
#include <memory>

class Scene;
class EngineGL;

/**
 * @class OpenGLWidget
 * @brief A widget that provides an OpenGL rendering context for rendering a scene.
 *
 * This class inherits from QOpenGLWidget and provides functionality to initialize the OpenGL context,
 * render the scene, handle mouse and keyboard events, and manage the rendering engine.
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the OpenGLWidget class.
   * @param parent The parent widget for this OpenGL widget.
   */
  explicit OpenGLWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the scene to be rendered by this OpenGL widget.
   * @param scene The Scene object to be rendered.
   */
  void setScene(Scene* scene);

  /**
   * @brief Selects an object in the scene by its name.
   * @param object_name The name of the object to select.
   */
  void selectObject(const QString& object_name);

  /**
   * @brief Unselects the currently selected object in the scene.
   */
  void unselectObject();

  /**
   * @brief Updates the viewport camera based on the render camera.
   */
  void updateViewportCamera();

  /**
   * @brief Updates the render camera based on the viewport camera.
   */
  void updateRenderCamera();

  /**
   * @brief Gets the OpenGL rendering engine used by this widget.
   * @return A pointer to the EngineGL instance managing OpenGL rendering.
   */
  EngineGL* getEngine() const { return m_engine.get(); }

signals:
  void engineInitialized(EngineGL* engine);

  void objectSelected(const QString& object_name);
  void objectUnselected();
  void objectDeleted();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

private:
  Scene*                    m_scene;
  std::unique_ptr<EngineGL> m_engine;

  QPoint  m_last_mouse_pos = QPoint(0, 0);
  QTimer* m_refresh_timer  = nullptr;

  void selectObjectFromClick(int x, int y);
};

#endif // GUI_WIDGETS_OPENGLWIDGET_HPP
