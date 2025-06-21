#ifndef QT_VIEW_CAMERAVIEW_HPP
#define QT_VIEW_CAMERAVIEW_HPP

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>

/**
 * @class CameraView
 * @brief Widget for viewing and editing camera parameters (MVC View).
 *
 * This widget does not depend on the Camera class directly.
 * It emits individual signals when parameters are modified.
 */
class CameraView : public QWidget {
  Q_OBJECT

public:
  explicit CameraView(QWidget* parent);

  void setHorizontalFov(double fov);
  void setSensorWidth(double width);
  void setFocalLength(double focal_length);
  void setAperture(double aperture);
  void setShutterSpeed(double shutter_speed);
  void setFocusDistance(double focus_distance);
  void setNearPlane(double near_plane);
  void setFarPlane(double far_plane);
  void setProjectionMode(const QString& mode);

signals:
  void projectionModeChanged(const QString& mode);
  void horizontalFovChanged(double fov);
  void sensorWidthChanged(double width);
  void focalLengthChanged(double focal_length);
  void apertureChanged(double aperture);
  void shutterSpeedChanged(double speed);
  void focusDistanceChanged(double distance);
  void nearPlaneChanged(double near_plane);
  void farPlaneChanged(double far_plane);
  void updateViewportCamera();
  void updateRenderCamera();

private:
  QComboBox*      m_projection_mode_combo         = nullptr;
  QDoubleSpinBox* m_fov_spin                      = nullptr;
  QDoubleSpinBox* m_sensor_width_spin             = nullptr;
  QDoubleSpinBox* m_focal_length_spin             = nullptr;
  QDoubleSpinBox* m_aperture_spin                 = nullptr;
  QDoubleSpinBox* m_shutter_speed_spin            = nullptr;
  QDoubleSpinBox* m_focus_distance_spin           = nullptr;
  QDoubleSpinBox* m_near_plane_spin               = nullptr;
  QDoubleSpinBox* m_far_plane_spin                = nullptr;
  QPushButton*    m_update_viewport_camera_button = nullptr;
  QPushButton*    m_update_render_camera_button   = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_CAMERAVIEW_HPP
