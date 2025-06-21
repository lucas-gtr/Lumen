#include <qaction.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qwidget.h>

#include "Qt/View/MenuView.hpp"

MenuView::MenuView(QWidget* parent) : QMenuBar(parent) {
  setupUI();
  connectSignals();
}

void MenuView::setupUI() {
  QMenu* geometry_menu = addMenu("Geometry");
  m_add_cube           = geometry_menu->addAction("Add Cube");
  m_add_plane          = geometry_menu->addAction("Add Plane");
  m_add_sphere         = geometry_menu->addAction("Add Sphere");
  geometry_menu->addSeparator();
  m_load_obj = geometry_menu->addAction("Load .obj");

  QMenu* lights_menu      = addMenu("Lights");
  m_add_directional_light = lights_menu->addAction("Add Directional Light");
  m_add_point_light       = lights_menu->addAction("Add Point Light");
  m_add_spot_light        = lights_menu->addAction("Add Spotlight");
}

void MenuView::connectSignals() {
  connect(m_add_cube, &QAction::triggered, this, &MenuView::addCubeRequested);
  connect(m_add_plane, &QAction::triggered, this, &MenuView::addPlaneRequested);
  connect(m_add_sphere, &QAction::triggered, this, &MenuView::addSphereRequested);
  connect(m_load_obj, &QAction::triggered, this, &MenuView::loadObjRequested);

  connect(m_add_directional_light, &QAction::triggered, this, &MenuView::addDirectionalLightRequested);
  connect(m_add_point_light, &QAction::triggered, this, &MenuView::addPointLightRequested);
  connect(m_add_spot_light, &QAction::triggered, this, &MenuView::addSpotLightRequested);
}
