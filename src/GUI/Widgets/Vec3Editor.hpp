/**
 * @file Vec3Editor.hpp
 * @brief Header file for the Vec3Editor class, which provides a widget for editing
 *        3D vectors (Vec3).
 */
#ifndef GUI_WIDGETS_VEC3EDITOR_HPP
#define GUI_WIDGETS_VEC3EDITOR_HPP

#include <QWidget>
#include <linalg/Vec3.hpp>

namespace Ui {
class Vec3Editor;
}

/**
 * @class Vec3Editor
 * @brief A widget for editing 3D vectors (Vec3).
 *
 * This class provides a user interface for editing 3D vectors, allowing users to input
 * values for the x, y, and z components of the vector. It emits a signal when the value changes.
 */
class Vec3Editor : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the Vec3Editor class.
   * @param parent The parent widget for this Vec3Editor.
   */
  explicit Vec3Editor(QWidget* parent = nullptr);

  /**
   * @brief Sets the value of the Vec3Editor.
   * @param value The Vec3d value to set.
   */
  void setValue(const linalg::Vec3d& value);

  ~Vec3Editor() override; ///< Default destructor for the Vec3Editor class.

signals:
  void valueChanged(const linalg::Vec3d& new_value);

private slots:
  void onValueChanged();

private:
  Ui::Vec3Editor* ui;

  linalg::Vec3d getValue() const;
};

#endif // GUI_WIDGETS_VEC3EDITOR_HPP
