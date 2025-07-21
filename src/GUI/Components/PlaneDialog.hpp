/**
 * @file PlaneDialog.hpp
 * @brief Header file for the PlaneDialog class, which provides a user interface for creating and editing a plane
 * object.
 */
#ifndef GUI_COMPONENTS_PLANEDIALOG_HPP
#define GUI_COMPONENTS_PLANEDIALOG_HPP

#include <QDialog>

namespace Ui {
class PlaneDialog;
}

/**
 * @class PlaneDialog
 * @brief A dialog for creating and editing a plane object.
 *
 * This class provides a user interface for specifying the dimensions of a plane object,
 * including its width and length. It allows users to input these values and create a plane
 * object with the specified dimensions.
 */
class PlaneDialog : public QDialog {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the PlaneDialog class.
   * @param parent The parent widget for this dialog.
   */
  explicit PlaneDialog(QWidget* parent = nullptr);

  /**
   * @brief Gets the width of the plane specified in the dialog.
   * @return The width of the plane as a double.
   */
  double getWidth() const;

  /**
   * @brief Gets the length of the plane specified in the dialog.
   * @return The length of the plane as a double.
   */
  double getLength() const;

  ~PlaneDialog() override; ///< Default destructor for the PlaneDialog class.

private:
  Ui::PlaneDialog* ui;
};

#endif // GUI_COMPONENTS_PLANEDIALOG_HPP
