#include <memory>
#include <qspinbox.h>
#include <qwidget.h>
#include <utility>

#include "Core/Config.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/PlaneMeshBuilder.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"
#include "Qt/MeshBuilderDialog/PlaneMeshBuilderDialog.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
PlaneMeshBuilderDialog::PlaneMeshBuilderDialog(QWidget* parent) : MeshBuilderDialog(parent) {
  PlaneMeshBuilderDialog::setupUI();
}

void PlaneMeshBuilderDialog::setupUI() {
  setWindowTitle("Plane Parameters");

  m_width_spin = new QDoubleSpinBox(this);
  m_width_spin->setRange(MIN_PLANE_WIDTH, MAX_PLANE_WIDTH);
  m_width_spin->setValue(DEFAULT_PLANE_WIDTH);

  m_length_spin = new QDoubleSpinBox(this);
  m_length_spin->setRange(MIN_PLANE_LENGTH, MAX_PLANE_LENGTH);
  m_length_spin->setValue(DEFAULT_PLANE_LENGTH);

  addWidgetToLayout("Width ", m_width_spin);
  addWidgetToLayout("Length ", m_length_spin);
  addButtonsRow();
}

std::unique_ptr<Mesh> PlaneMeshBuilderDialog::buildMesh() const {
  const PlaneMeshBuilder builder(m_width_spin->value(), m_length_spin->value());
  Mesh                   mesh = builder.build();
  return std::make_unique<Mesh>(std::move(mesh));
}
// NOLINTEND(cppcoreguidelines-owning-memory)