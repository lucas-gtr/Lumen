#ifndef QT_VIEW_MENUVIEW_HPP
#define QT_VIEW_MENUVIEW_HPP

#include <qaction.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qobject.h>

class MenuView : public QMenuBar {
  Q_OBJECT

public:
  explicit MenuView(QWidget* parent);

signals:
  void addCubeRequested();
  void addPlaneRequested();
  void addSphereRequested();
  void loadObjRequested();

  void addDirectionalLightRequested();
  void addPointLightRequested();
  void addSpotLightRequested();

private:
  QMenu* m_geometry_menu = nullptr;
  QMenu* m_texture_menu  = nullptr;
  QMenu* m_lights_menu   = nullptr;

  QAction* m_add_cube   = nullptr;
  QAction* m_add_plane  = nullptr;
  QAction* m_add_sphere = nullptr;
  QAction* m_load_obj   = nullptr;

  QAction* m_add_texture = nullptr;

  QAction* m_add_directional_light = nullptr;
  QAction* m_add_point_light       = nullptr;
  QAction* m_add_spot_light        = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_MENUVIEW_HPP
