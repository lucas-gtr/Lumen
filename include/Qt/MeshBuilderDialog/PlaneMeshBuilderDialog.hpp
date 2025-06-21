/**
 * @file PlaneMeshBuilderDialog.hpp
 * @brief Header file for the PlaneMeshBuilderDialog class, which provides a dialog for building plane meshes.
 */
#ifndef QT_MESHBUILDERDIALOG_PLANEMESHBUILDERDIALOG_HPP
#define QT_MESHBUILDERDIALOG_PLANEMESHBUILDERDIALOG_HPP

#include <qspinbox.h>

#include "Geometry/PlaneMeshBuilder.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"

/**
 * @class PlaneMeshBuilderDialog
 * @brief A dialog for building plane meshes.
 */
class PlaneMeshBuilderDialog : public MeshBuilderDialog {
  Q_OBJECT
private:
  QDoubleSpinBox* m_width_spin  = nullptr;
  QDoubleSpinBox* m_length_spin = nullptr;

  void setupUI() override;

public:
  /**
   * @brief Constructor for PlaneMeshBuilderDialog.
   * @param parent The parent widget for this dialog.
   */
  explicit PlaneMeshBuilderDialog(QWidget* parent);

  PlaneMeshBuilderDialog(const PlaneMeshBuilderDialog&)            = delete;
  PlaneMeshBuilderDialog& operator=(const PlaneMeshBuilderDialog&) = delete;
  PlaneMeshBuilderDialog(PlaneMeshBuilderDialog&&)                 = delete;
  PlaneMeshBuilderDialog& operator=(PlaneMeshBuilderDialog&&)      = delete;

  /**
   * @brief Builds a mesh based on the dialog's input.
   * @return A unique pointer to the constructed Mesh object.
   */
  std::unique_ptr<Mesh> buildMesh() const override;

  /**
   * @brief Default destructor for PlaneMeshBuilderDialog.
   */
  ~PlaneMeshBuilderDialog() override = default;
};

#endif // QT_MESHBUILDERDIALOG_PLANEMESHBUILDERDIALOG_HPP