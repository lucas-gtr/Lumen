/**
 * @file TransformWidget.hpp
 * @brief Header file for the TransformWidget class, which provides a user interface for editing
 *        transformation properties of 3D objects.
 */
#ifndef GUI_WIDGETS_TRANSFORMWIDGET_HPP
#define GUI_WIDGETS_TRANSFORMWIDGET_HPP

#include <QGroupBox>
#include <linalg/Vec3.hpp>

class Transform;
namespace Ui {
  class TransformWidget;
}

/**
 * @class TransformWidget
 * @brief A widget for displaying and editing transformation properties of a 3D object.
 *
 * This class provides a user interface for managing the position, rotation, and scale of a 3D object.
 * It allows users to modify these properties interactively and updates the associated Transform object accordingly.
 */
class TransformWidget : public QGroupBox {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the TransformWidget class.
   * @param parent The parent widget for this transform widget.
   */
  explicit TransformWidget(QWidget* parent = nullptr);
  
  /**
   * @brief Sets the Transform object for this widget.
   * @param transform The Transform instance to set.
   */
  void setTransform(Transform* transform);

  /**
   * @brief Updates the widget to reflect the current state of the Transform object.
   */
  void updateWidget();

  ~TransformWidget() override; ///< Default destructor for the TransformWidget class.

private slots:
  void onPositionChanged(const linalg::Vec3d& new_position);
  void onRotationChanged(const linalg::Vec3d& new_rotation);
  void onScaleChanged(const linalg::Vec3d& new_scale);

private:
  Ui::TransformWidget* ui;
  Transform*           m_transform = nullptr;
};

#endif // GUI_WIDGETS_TRANSFORMWIDGET_HPP
