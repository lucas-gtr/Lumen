#include <qboxlayout.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/MaterialSelectorWidget.hpp"
#include "Qt/View/ObjectView.hpp"

ObjectView::ObjectView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void ObjectView::setupUI() {
  auto* group_box = new QGroupBox("Object Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight: bold; font-size: 14px;}");

  m_material_selector = new MaterialSelectorWidget(DEFAULT_MATERIAL_NAME, group_box);

  auto* form_layout = new QFormLayout(group_box);
  form_layout->addRow("Material ", m_material_selector);
  group_box->setLayout(form_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(group_box);
  main_layout->addStretch();

  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void ObjectView::connectSignals() {
  connect(m_material_selector, &MaterialSelectorWidget::materialSelected, this, &ObjectView::materialSelected);
}

void ObjectView::setCurrentMaterial(const QString& material_name) {
  m_material_selector->setCurrentMaterial(material_name);
}
