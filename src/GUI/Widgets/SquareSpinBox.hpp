/**
 * @file SquareSpinBox.hpp
 * @brief Header file for the SquareSpinBox class, which provides a spin box for selecting square numbers.
 */
#ifndef GUI_WIDGETS_SQUARESPINBOX_HPP
#define GUI_WIDGETS_SQUARESPINBOX_HPP

#include <QSpinBox>

/**
 * @class SquareSpinBox
 * @brief A spin box that allows selection of square numbers.
 *
 * This class inherits from QSpinBox and overrides methods to ensure that the values
 * are always square numbers. It provides functionality to step through square numbers
 * when the user interacts with the spin box.
 */
class SquareSpinBox : public QSpinBox {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the SquareSpinBox class.
   * @param parent The parent widget for this spin box.
   */
  explicit SquareSpinBox(QWidget* parent = nullptr);

protected:
  int     valueFromText(const QString& text) const override;
  QString textFromValue(int value) const override;

private:
  static int nextSquare(int current);
  static int previousSquare(int current);

  void stepBy(int steps) override;
};

#endif // GUI_WIDGETS_SQUARESPINBOX_HPP
