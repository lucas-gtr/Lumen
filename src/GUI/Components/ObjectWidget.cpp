#include "Core/Config.hpp"
#include "ObjectWidget.hpp"
#include "SceneObjects/Object3D.hpp"
#include "Surface/MaterialManager.hpp"
#include "ui_ObjectWidget.h"

ObjectWidget::ObjectWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::ObjectWidget) {
  ui->setupUi(this);

  setStyleSheet(QString::fromStdString(std::string(GROUP_BOX_STYLESHEET)));

  connect(ui->materialComboBox, &QComboBox::currentTextChanged, this, &ObjectWidget::onMaterialChanged);
}

void ObjectWidget::setObject3D(Object3D* object) {
  m_object = object;
  if(m_object == nullptr) {
    return;
  }
  updateWidget();
}

void ObjectWidget::setMaterialManager(MaterialManager* material_manager) { m_material_manager = material_manager; }

void ObjectWidget::setMaterialsListModel(DefaultOptionProxyModel* model) {
  m_materials_list_model = model;
  ui->materialComboBox->setModel(m_materials_list_model);
}

void ObjectWidget::setCurrentMaterial(const QString& material_name) {
  if(m_object == nullptr) {
    return;
  }

  const int index = ui->materialComboBox->findText(material_name);
  if(index != -1) {
    ui->materialComboBox->setCurrentIndex(index);
  }
}

void ObjectWidget::updateWidget() {
  if(m_object == nullptr) {
    return;
  }

  Material*   material      = m_object->getMaterial();
  std::string material_name = m_material_manager->getMaterialName(material);
  if(material_name.empty()) {
    material_name = "Default";
  }
  ui->materialComboBox->setCurrentText(QString::fromStdString(material_name));
}

void ObjectWidget::onMaterialChanged(const QString& new_material) {
  if(m_object == nullptr) {
    return;
  }
  emit materialChanged(m_object, new_material);
}

ObjectWidget::~ObjectWidget() { delete ui; }
