/**
 * @file CubeMeshBuilderDialog.hpp
 * @brief Header file for the CubeMeshBuilderDialog class, which provides a dialog for building cube meshes.
 */
#ifndef QT_MESHBUILDERDIALOG_CUBEMESHBUILDERDIALOG_HPP
#define QT_MESHBUILDERDIALOG_CUBEMESHBUILDERDIALOG_HPP

#include <qspinbox.h>

#include "Geometry/CubeMeshBuilder.hpp"
#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"

/**
 * @class CubeMeshBuilderDialog
 * @brief A dialog for building cube meshes.
 *
 * This dialog allows the user to specify the size of the cube mesh and build it.
 * It inherits from MeshBuilderDialog and provides a specific implementation for cube meshes.
 */
class CubeMeshBuilderDialog : public MeshBuilderDialog {
  Q_OBJECT
private:
  QDoubleSpinBox* m_size_spin = nullptr;

  void setupUI() override;

public:
  /**
   * @brief Constructor for CubeMeshBuilderDialog.
   * @param parent The parent widget for this dialog.
   */
  explicit CubeMeshBuilderDialog(QWidget* parent);

  CubeMeshBuilderDialog(const CubeMeshBuilderDialog&)            = delete;
  CubeMeshBuilderDialog& operator=(const CubeMeshBuilderDialog&) = delete;
  CubeMeshBuilderDialog(CubeMeshBuilderDialog&&)                 = delete;
  CubeMeshBuilderDialog& operator=(CubeMeshBuilderDialog&&)      = delete;

  /**
   * @brief Builds a cube mesh based on the specified size.
   * @return A unique pointer to the created Mesh object.
   */
  std::unique_ptr<Mesh> buildMesh() const override;

  /**
   * @brief Default destructor for CubeMeshBuilderDialog.
   */
  ~CubeMeshBuilderDialog() override = default;
};

#endif // QT_MESHBUILDERDIALOG_CUBEMESHBUILDERDIALOG_HPP