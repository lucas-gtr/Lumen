/**
 * @file CubeDialog.hpp
 * @brief Header file for the CubeDialog class, which provides a dialog for creating cubes.
 */
#ifndef GUI_COMPONENTS_CUBEDIALOG_HPP
#define GUI_COMPONENTS_CUBEDIALOG_HPP

#include <QDialog>

namespace Ui {
  class CubeDialog;
}

/**
 * @class CubeDialog
 * @brief A dialog for creating cubes with a specified size.
 *
 * This class provides a user interface for specifying the size of a cube.
 * It inherits from QDialog and allows users to input the size of the cube.
 */
class CubeDialog : public QDialog {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the CubeDialog class.
   * @param parent The parent widget for this dialog.
   */
  explicit CubeDialog(QWidget* parent = nullptr);
  
  /**
   * @brief Gets the size of the cube specified in the dialog.
   * @return The size of the cube as a double.
   */
  double getSize() const;

  ~CubeDialog(); ///< Default destructor for the CubeDialog class.

private:
  Ui::CubeDialog* ui;
};

#endif // GUI_COMPONENTS_CUBEDIALOG_HPP
