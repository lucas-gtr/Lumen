#include <memory>
#include <qspinbox.h>
#include <qwidget.h>
#include <utility>

#include "Core/Config.hpp"
#include "Geometry/Mesh.hpp"
#include "Geometry/SphereMeshBuilder.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"
#include "Qt/MeshBuilderDialog/SphereMeshBuilderDialog.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
SphereMeshBuilderDialog::SphereMeshBuilderDialog(QWidget* parent) : MeshBuilderDialog(parent) {
  SphereMeshBuilderDialog::setupUI();
}

void SphereMeshBuilderDialog::setupUI() {
  setWindowTitle("Sphere Parameters");

  m_radius_spin = new QDoubleSpinBox(this);
  m_radius_spin->setRange(MIN_SPHERE_RADIUS, MAX_SPHERE_RADIUS);
  m_radius_spin->setValue(DEFAULT_SPHERE_RADIUS);

  m_segments_spin = new QSpinBox(this);
  m_segments_spin->setRange(MIN_SPHERE_SEGMENTS, MAX_SPHERE_SEGMENTS);
  m_segments_spin->setValue(DEFAULT_SPHERE_SEGMENTS);

  m_rings_spin = new QSpinBox(this);
  m_rings_spin->setRange(MIN_SPHERE_RINGS, MAX_SPHERE_RINGS);
  m_rings_spin->setValue(DEFAULT_SPHERE_RINGS);

  addWidgetToLayout("Radius ", m_radius_spin);
  addWidgetToLayout("Segments ", m_segments_spin);
  addWidgetToLayout("Rings ", m_rings_spin);
  addButtonsRow();
}

std::unique_ptr<Mesh> SphereMeshBuilderDialog::buildMesh() const {
  const SphereMeshBuilder builder(m_radius_spin->value(), m_segments_spin->value(), m_rings_spin->value());
  Mesh                    mesh = builder.build();
  return std::make_unique<Mesh>(std::move(mesh));
}
// NOLINTEND(cppcoreguidelines-owning-memory)