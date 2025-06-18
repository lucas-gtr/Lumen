#include <memory>
#include <qspinbox.h>
#include <qwidget.h>
#include <utility>

#include "Core/Config.hpp"
#include "Geometry/CubeMeshBuilder.hpp"
#include "Geometry/Mesh.hpp"
#include "Qt/MeshBuilderDialog/CubeMeshBuilderDialog.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
CubeMeshBuilderDialog::CubeMeshBuilderDialog(QWidget* parent) : MeshBuilderDialog(parent) {
  CubeMeshBuilderDialog::setupUI();
}

void CubeMeshBuilderDialog::setupUI() {
  setWindowTitle("Cube Parameters");

  m_size_spin = new QDoubleSpinBox(this);
  m_size_spin->setRange(MIN_CUBE_SIZE, MAX_CUBE_SIZE);
  m_size_spin->setValue(DEFAULT_CUBE_SIZE);

  addWidgetToLayout("Size ", m_size_spin);
  addButtonsRow();
}
// NOLINTEND(cppcoreguidelines-owning-memory)

std::unique_ptr<Mesh> CubeMeshBuilderDialog::buildMesh() const {
  const CubeMeshBuilder builder(m_size_spin->value());
  Mesh                  mesh = builder.build();
  return std::make_unique<Mesh>(std::move(mesh));
}