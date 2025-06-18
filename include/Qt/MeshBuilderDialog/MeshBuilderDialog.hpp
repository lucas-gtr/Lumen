/**
 * @file MeshBuilderDialog.hpp
 * @brief Header file for the MeshBuilderDialog class, which provides a dialog for building meshes.
 */
#ifndef QT_MESHBUILDERDIALOG_MESHBUILDERDIALOG_HPP
#define QT_MESHBUILDERDIALOG_MESHBUILDERDIALOG_HPP

#include <memory>
#include <qdialog.h>
#include <qformlayout.h>

#include "Geometry/Mesh.hpp"
#include "Surface/Material.hpp"

/**
 * @class MeshBuilderDialog
 * @brief A dialog for building meshes with a form layout.
 *
 * This class provides a base dialog for creating meshes, allowing derived classes to implement specific mesh building
 * logic.
 */
class MeshBuilderDialog : public QDialog {
  Q_OBJECT
private:
  QFormLayout* m_layout = nullptr;

  virtual void setupUI() = 0;

protected:
  void addWidgetToLayout(const QString& label, QWidget* widget) {
    if(m_layout != nullptr) {
      m_layout->addRow(label, widget);
    }
  }

public:
  /**
   * @brief Constructor for MeshBuilderDialog.
   * @param parent The parent widget for this dialog.
   */
  explicit MeshBuilderDialog(QWidget* parent) : QDialog(parent), m_layout(new QFormLayout(this)) {}

  MeshBuilderDialog(const MeshBuilderDialog&)            = delete;
  MeshBuilderDialog(MeshBuilderDialog&&)                 = delete;
  MeshBuilderDialog& operator=(const MeshBuilderDialog&) = delete;
  MeshBuilderDialog& operator=(MeshBuilderDialog&&)      = delete;

  /**
   * @brief Adds a row of buttons to the dialog.
   */
  void addButtonsRow();

  /**
   * @brief Builds a mesh based on the dialog's input.
   * @return A unique pointer to the constructed Mesh object.
   */
  virtual std::unique_ptr<Mesh> buildMesh() const = 0;

  /**
   * @brief Default destructor for MeshBuilderDialog.
   */
  ~MeshBuilderDialog() override = default;
};

#endif // QT_MESHBUILDERDIALOG_MESHBUILDERDIALOG_HPP