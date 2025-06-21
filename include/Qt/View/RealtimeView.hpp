#ifndef QT_VIEW_REALTIMEVIEW_HPP
#define QT_VIEW_REALTIMEVIEW_HPP

#include <qopenglfunctions_3_3_core.h>
#include <qopenglwidget.h>
#include <qpoint.h>
#include <qtimer.h>

class RealtimeView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
  Q_OBJECT

public:
  explicit RealtimeView(QWidget* parent);

signals:
  void initializeRequested();
  void renderRequested(unsigned int fbo);
  void resized(int width, int height);
  void wheelMoved(float delta);
  void leftMouseClicked(int x, int y);
  void middleMouseDragged(int dx, int dy, bool shiftPressed);

  void updateRequested();

protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

  void onUpdate();

private:
  QPoint m_lastMousePos = QPoint(0, 0);

  QTimer* m_refresh_timer = nullptr;

  void connectSignals();
};

#endif // QT_VIEW_REALTIMEVIEW_HPP
