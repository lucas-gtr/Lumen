#ifndef QT_CONTROLLER_OBJECTCONTROLLER_HPP
#define QT_CONTROLLER_OBJECTCONTROLLER_HPP

#include <qobject.h>
#include <qstring.h>

#include "Qt/Controller/MaterialController.hpp"
#include "Qt/View/ObjectView.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/MaterialManager.hpp"

class ObjectController : public QObject {
  Q_OBJECT
public:
  explicit ObjectController(ObjectView* view, MaterialManager* material_manager);

  void setObject(Object3D* object);
  void updateViewFromModel();

private:
  // Slots
  void onMaterialSelected(const QString& material_name);

  // Members
  Object3D*   m_object = nullptr;
  ObjectView* m_view;

  MaterialManager* m_material_manager;

  void connectSignals();
};

#endif // QT_CONTROLLER_OBJECTCONTROLLER_HPP
