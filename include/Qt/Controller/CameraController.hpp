#ifndef QT_CONTROLLER_CAMERACONTROLLER_HPP
#define QT_CONTROLLER_CAMERACONTROLLER_HPP

#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/TransformController.hpp"
#include "Qt/View/CameraView.hpp"
#include "SceneObjects/Camera.hpp"

class CameraController : public QObject {
  Q_OBJECT
public:
  explicit CameraController(CameraView* view, Camera* camera, RealtimeController* viewport,
                            TransformController* transform_controller);

  void updateViewFromModel();

private:
  // Slots
  void onProjectionModeChanged(const QString& mode);
  void onHorizontalFovChanged(double fov);
  void onSensorWidthChanged(double width);
  void onFocalLengthChanged(double focal_length);
  void onApertureChanged(double aperture);
  void onShutterSpeedChanged(double shutter_speed);
  void onFocusDistanceChanged(double distance);
  void onNearPlaneChanged(double near_plane);
  void onFarPlaneChanged(double far_plane);
  void onUpdateViewportCamera();
  void onUpdateRenderCamera();

  // Members
  CameraView* m_view;
  Camera*     m_camera;

  RealtimeController*  m_realtime_controller;
  TransformController* m_transform_controller;

  void connectSignals();
};

#endif // QT_CONTROLLER_CAMERACONTROLLER_HPP
