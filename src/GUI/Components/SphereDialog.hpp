/**
 * @file SphereDialog.hpp
 * @brief Header file for the SphereDialog class, which provides a user interface for creating and
 *        configuring spheres in a 3D scene.
 */
#ifndef GUI_COMPONENTS_SPHEREDIALOG_HPP
#define GUI_COMPONENTS_SPHEREDIALOG_HPP

#include <QDialog>

namespace Ui {
class SphereDialog;
}

/**
 * @class SphereDialog
 * @brief A dialog for creating and configuring spheres in a 3D scene.
 *
 * This class provides a user interface for specifying the radius, segments, and rings of a sphere.
 * It allows users to create spheres with customizable parameters.
 */
class SphereDialog : public QDialog {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the SphereDialog class.
   * @param parent The parent widget for this dialog.
   */
  explicit SphereDialog(QWidget* parent = nullptr);

  /**
   * @brief Gets the radius of the sphere.
   * @return The radius of the sphere.
   */
  double getRadius() const;

  /**
   * @brief Gets the number of segments of the sphere.
   * @return The number of segments of the sphere.
   */
  int getSegments() const;

  /**
   * @brief Gets the number of rings of the sphere.
   * @return The number of rings of the sphere.
   */
  int getRings() const;

  ~SphereDialog(); ///< Default destructor for the SphereDialog class.

private:
  Ui::SphereDialog* ui;
};

#endif // GUI_COMPONENTS_SPHEREDIALOG_HPP
