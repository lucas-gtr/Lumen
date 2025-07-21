/**
 * @file CameraWidget.hpp
 * @brief Header file for the CameraWidget class, which provides a user interface for camera settings.
 */
#ifndef GUI_WIDGETS_CAMERAWIDGET_HPP
#define GUI_WIDGETS_CAMERAWIDGET_HPP

#include <QGroupBox>
#include <linalg/Vec3.hpp>

class Camera;

namespace Ui {
class CameraWidget;
}

/**
 * @class CameraWidget
 * @brief A widget for displaying and editing camera settings.
 *
 * This class provides a user interface for adjusting camera parameters such as field of view,
 * sensor width, focal length, aperture, shutter speed, focus distance, near plane, and far plane.
 * It emits signals when the camera settings are updated.
 */
class CameraWidget : public QGroupBox {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the CameraWidget class.
   * @param parent The parent widget for this camera widget.
   */
  explicit CameraWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the camera for this widget.
   * @param camera The Camera object to set.
   */
  void setCamera(Camera* camera);

  ~CameraWidget() override; ///< Default destructor for the CameraWidget class.

signals:
  void viewportCameraUpdated();
  void renderCameraUpdated();

private slots:
  void onHorizontalFovChanged(double value);
  void onSensorWidthChanged(double value);
  void onFocalLengthChanged(double value);
  void onApertureChanged(double value);
  void onShutterSpeedChanged(double value);
  void onFocusDistanceChanged(double value);
  void onNearPlaneChanged(double value);
  void onFarPlaneChanged(double value);

private:
  Ui::CameraWidget* ui;
  Camera*           m_camera = nullptr;
};

#endif // GUI_WIDGETS_CAMERAWIDGET_HPP
