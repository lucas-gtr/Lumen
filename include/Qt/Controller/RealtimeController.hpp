#ifndef QT_CONTROLLER_REALTIMECONTROLLER_HPP
#define QT_CONTROLLER_REALTIMECONTROLLER_HPP

#include "GPU/OpenGL/EngineGL.hpp"
#include "Qt/View/RealtimeView.hpp"
#include "Scene/Scene.hpp"
#include <memory>
#include <qobject.h>

class RealtimeController : public QObject {
  Q_OBJECT

public:
  RealtimeController(RealtimeView* view, Scene* scene);

  EngineGL* getEngine() const { return m_engine.get(); }

  void bakeLights();
  void selectObject(const QString& object_name);
  void unselectObject();
  void updateViewportCamera();
  void updateRenderCamera();
  bool shouldClose() const;
  void closeWindow();

signals:
  void objectSelected(const QString& object_name);
  void objectUnselected();

private:
  // Slots
  void onInitialize();
  void onResize(int width, int height);
  void onRender(unsigned int framebuffer_id);
  void onWheel(float delta);
  void onLeftMouseClick(int x, int y);
  void onMouseDrag(int dx, int dy, bool shift);

  // Members
  RealtimeView* m_view = nullptr;

  Scene*                    m_scene;
  std::unique_ptr<EngineGL> m_engine;
  bool                      m_shouldClose = false;

  void connectSignals();
};

#endif // QT_CONTROLLER_REALTIMECONTROLLER_HPP