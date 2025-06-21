/**
 * @file SphereMeshBuilderDialog.hpp
 * @brief Header file for the SphereMeshBuilderDialog class, which provides a dialog for building sphere meshes.
 */
#ifndef QT_MESHBUILDERDIALOG_SPHEREMESHBUILDERDIALOG_HPP
#define QT_MESHBUILDERDIALOG_SPHEREMESHBUILDERDIALOG_HPP

#include <qspinbox.h>

#include "Geometry/SphereMeshBuilder.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"

/**
 * @class SphereMeshBuilderDialog
 * @brief A dialog for building sphere meshes.
 */
class SphereMeshBuilderDialog : public MeshBuilderDialog {
  Q_OBJECT
private:
  QDoubleSpinBox* m_radius_spin   = nullptr;
  QSpinBox*       m_segments_spin = nullptr;
  QSpinBox*       m_rings_spin    = nullptr;

  void setupUI() override;

public:
  /**
   * @brief Constructor for SphereMeshBuilderDialog.
   * @param parent The parent widget for this dialog.
   */
  explicit SphereMeshBuilderDialog(QWidget* parent);

  SphereMeshBuilderDialog(const SphereMeshBuilderDialog&)            = delete;
  SphereMeshBuilderDialog& operator=(const SphereMeshBuilderDialog&) = delete;
  SphereMeshBuilderDialog(SphereMeshBuilderDialog&&)                 = delete;
  SphereMeshBuilderDialog& operator=(SphereMeshBuilderDialog&&)      = delete;

  /**
   * @brief Builds a sphere mesh based on the specified radius, segments, and rings.
   * @return A unique pointer to the created Mesh object.
   */
  std::unique_ptr<Mesh> buildMesh() const override;

  /**
   * @brief Default destructor for SphereMeshBuilderDialog.
   */
  ~SphereMeshBuilderDialog() override = default;
};

#endif // QT_MESHBUILDERDIALOG_SPHEREMESHBUILDERDIALOG_HPP