#include <qobject.h>
#include <qstring.h>
#include <string>

#include "Core/Config.hpp"
#include "Qt/Controller/ObjectController.hpp"
#include "Qt/View/ObjectView.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/Material.hpp"
#include "Surface/MaterialManager.hpp"

ObjectController::ObjectController(ObjectView* view, MaterialManager* material_manager)
    : QObject(view), m_view(view), m_material_manager(material_manager) {
  m_view->getMaterialSelectorWidget()->connectToMaterialManager(material_manager);
  connectSignals();
  updateViewFromModel();
}

void ObjectController::connectSignals() {
  connect(m_view, &ObjectView::materialSelected, this, &ObjectController::onMaterialSelected);
}

void ObjectController::setObject(Object3D* object) {
  m_object = object;
  updateViewFromModel();
}

void ObjectController::onMaterialSelected(const QString& material_name) {
  if(material_name.toStdString() == DEFAULT_MATERIAL_NAME) {
    m_object->setMaterial(MaterialManager::DefaultMaterial());
  } else {
    Material* material = m_material_manager->getMaterial(material_name.toStdString());
    if(material != nullptr) {
      m_object->setMaterial(material);
    } else {
    }
  }
}

void ObjectController::updateViewFromModel() {
  if(m_object == nullptr) {
    return;
  }
  const Material* material = m_object->getMaterial();
  if(material != nullptr) {
    const std::string& material_name = m_material_manager->getMaterialName(material);
    m_view->setCurrentMaterial(QString::fromStdString(material_name));
  } else {
    m_view->setCurrentMaterial(QString::fromStdString(std::string(DEFAULT_MATERIAL_NAME)));
  }
}