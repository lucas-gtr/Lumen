#include <qboxlayout.h>
#include <qcombobox.h>
#include <qobject.h>
#include <string>
#include <string_view>

#include <qstring.h>
#include <qwidget.h>

#include "Qt/MaterialSelectorWidget.hpp"
#include "Surface/MaterialManager.hpp"

MaterialSelectorWidget::MaterialSelectorWidget(const std::string_view& default_material_name, QWidget* parent)
    : QWidget(parent), m_default_material_name(default_material_name) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void MaterialSelectorWidget::setupUI() {
  m_material_selector = new QComboBox(this);
  m_material_selector->addItem(QString::fromStdString(m_default_material_name));

  auto* layout = new QHBoxLayout;

  layout->addWidget(m_material_selector);

  layout->setContentsMargins(0, 0, 0, 0);
  setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void MaterialSelectorWidget::connectSignals() {
  connect(m_material_selector, &QComboBox::currentTextChanged, this, &MaterialSelectorWidget::materialSelected);
}

void MaterialSelectorWidget::connectToMaterialManager(MaterialManager* material_manager) {
  material_manager->getMaterialAddedObserver().add(
      [this](const std::string& material_name) { onMaterialAdded(material_name); });
  material_manager->getMaterialRemovedObserver().add(
      [this](const std::string& material_name) { onMaterialRemoved(material_name); });
  material_manager->getMaterialRenamedObserver().add(
      [this](const std::string& old_name, const std::string& new_name) { onMaterialRenamed(old_name, new_name); });
}

void MaterialSelectorWidget::onMaterialAdded(const std::string& material_name) {
  const QSignalBlocker blocker(m_material_selector);
  m_material_selector->addItem(QString::fromStdString(material_name));
}

void MaterialSelectorWidget::onMaterialRemoved(const std::string& material_name) {
  m_material_selector->blockSignals(true);
  const int index = m_material_selector->findText(QString::fromStdString(material_name));
  if(index != -1) {
    const bool was_selected = (m_material_selector->currentIndex() == index);
    m_material_selector->removeItem(index);
    if(was_selected && m_material_selector->count() > 0) {
      m_material_selector->blockSignals(false);
      m_material_selector->setCurrentIndex(0);
    }
  }
}

void MaterialSelectorWidget::onMaterialRenamed(const std::string& old_name, const std::string& new_name) {
  const QSignalBlocker blocker(m_material_selector);
  const int            index = m_material_selector->findText(QString::fromStdString(old_name));
  if(index != -1) {
    const bool was_selected = (m_material_selector->currentIndex() == index);
    m_material_selector->setItemText(index, QString::fromStdString(new_name));
    if(was_selected) {
      m_material_selector->setCurrentIndex(index);
    }
  }
}

void MaterialSelectorWidget::setCurrentMaterial(const QString& material_name) {
  const QSignalBlocker blocker(m_material_selector);
  const int            index = m_material_selector->findText(material_name);
  if(index != -1) {
    m_material_selector->setCurrentIndex(index);
  } else {
    m_material_selector->setCurrentIndex(0); // No match found
  }
}