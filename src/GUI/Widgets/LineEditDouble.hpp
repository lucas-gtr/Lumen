/**
 * @file LineEditDouble.hpp
 * @brief Header file for the LineEditDouble class, which provides a line edit for double values with validation.
 */
#ifndef GUI_WIDGETS_LINEEDITDOUBLE_HPP
#define GUI_WIDGETS_LINEEDITDOUBLE_HPP

#include <QDoubleValidator>
#include <QLineEdit>

/**
 * @class LineEditDouble
 * @brief A line edit widget for entering double values with validation.
 *
 * This class inherits from QLineEdit and provides functionality to set bounds for the double value,
 * validate the input, and emit a signal when the value changes.
 */
class LineEditDouble : public QLineEdit {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the LineEditDouble class.
   * @param parent The parent widget for this line edit.
   */
  explicit LineEditDouble(QWidget* parent = nullptr);

  /**
   * @brief Sets the bounds for the double value.
   * @param minimum The minimum value allowed.
   * @param maximum The maximum value allowed.
   */
  void setBounds(double minimum, double maximum);

  /**
   * @brief Gets the current value of the line edit.
   * @return The current double value as a double.
   */
  double value() const;

  /**
   * @brief Sets the value of the line edit.
   * @param new_value The new double value to set.
   */
  void setValue(double new_value);

signals:
  void valueChanged(double new_value);

private slots:
  void onTextChanged(const QString& text);

private:
  QDoubleValidator* m_validator;
  double            m_minimum;
  double            m_maximum;
  bool              m_hasBounds;
};

#endif // GUI_WIDGETS_LINEEDITDOUBLE_HPP
