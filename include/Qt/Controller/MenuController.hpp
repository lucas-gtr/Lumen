#ifndef QT_CONTROLLER_MENUCONTROLLER_HPP
#define QT_CONTROLLER_MENUCONTROLLER_HPP

#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>

#include "Qt/View/MenuView.hpp"
#include "Scene/Scene.hpp"

class MenuController : public QObject {
  Q_OBJECT
public:
  explicit MenuController(MenuView* view, Scene* scene);

private:
  // Slots
  void onAddCubeRequested();
  void onAddPlaneRequested();
  void onAddSphereRequested();
  void onLoadObjRequested();

  void onAddDirectionalLightRequested();
  void onAddPointLightRequested();
  void onAddSpotLightRequested();

  // Members
  MenuView* m_view;
  Scene*    m_scene;

  void connectSignals();
};

#endif // QT_CONTROLLER_MENUCONTROLLER_HPP