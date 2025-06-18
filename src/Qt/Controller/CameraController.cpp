#include <iostream>
#include <qobject.h>

#include "Core/MathConstants.hpp"
#include "Qt/Controller/CameraController.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/TransformController.hpp"
#include "Qt/View/CameraView.hpp"
#include "SceneObjects/Camera.hpp"

CameraController::CameraController(CameraView* view, Camera* camera, RealtimeController* viewport,
                                   TransformController* transform_controller)
    : QObject(view), m_view(view), m_camera(camera), m_realtime_controller(viewport),
      m_transform_controller(transform_controller) {
  connectSignals();
}

void CameraController::connectSignals() {
  connect(m_view, &CameraView::projectionModeChanged, this, &CameraController::onProjectionModeChanged);
  connect(m_view, &CameraView::horizontalFovChanged, this, &CameraController::onHorizontalFovChanged);
  connect(m_view, &CameraView::sensorWidthChanged, this, &CameraController::onSensorWidthChanged);
  connect(m_view, &CameraView::focalLengthChanged, this, &CameraController::onFocalLengthChanged);
  connect(m_view, &CameraView::apertureChanged, this, &CameraController::onApertureChanged);
  connect(m_view, &CameraView::shutterSpeedChanged, this, &CameraController::onShutterSpeedChanged);
  connect(m_view, &CameraView::focusDistanceChanged, this, &CameraController::onFocusDistanceChanged);
  connect(m_view, &CameraView::nearPlaneChanged, this, &CameraController::onNearPlaneChanged);
  connect(m_view, &CameraView::farPlaneChanged, this, &CameraController::onFarPlaneChanged);
  connect(m_view, &CameraView::updateViewportCamera, this, &CameraController::onUpdateViewportCamera);
  connect(m_view, &CameraView::updateRenderCamera, this, &CameraController::onUpdateRenderCamera);
}

void CameraController::updateViewFromModel() {
  m_view->setHorizontalFov(m_camera->getFovX());
  m_view->setSensorWidth(m_camera->getSensorWidth() * M_TO_MM);
  m_view->setFocalLength(m_camera->getFocalLength() * M_TO_MM);
  m_view->setAperture(m_camera->getAperture());
  m_view->setShutterSpeed(1.0 / m_camera->getShutterSpeed());
  m_view->setFocusDistance(m_camera->getFocusDistance());
  m_view->setNearPlane(m_camera->getNearPlane());
  m_view->setFarPlane(m_camera->getFarPlane());
}

void CameraController::onProjectionModeChanged(const QString& mode) {}

void CameraController::onHorizontalFovChanged(double fov) {
  m_camera->setHorizontalFov(fov);
  updateViewFromModel();
}

void CameraController::onSensorWidthChanged(double width) {
  m_camera->setSensorWidth(width);
  updateViewFromModel();
}

void CameraController::onFocalLengthChanged(double focal_length) {
  m_camera->setFocalLength(focal_length);
  updateViewFromModel();
}

void CameraController::onApertureChanged(double aperture) { m_camera->setAperture(aperture); }

void CameraController::onShutterSpeedChanged(double shutter_speed) { m_camera->setShutterSpeed(shutter_speed); }

void CameraController::onFocusDistanceChanged(double distance) { m_camera->setFocusDistance(distance); }

void CameraController::onNearPlaneChanged(double near_plane) {
  m_camera->setNearPlane(near_plane);
  updateViewFromModel();
}

void CameraController::onFarPlaneChanged(double far_plane) {
  m_camera->setFarPlane(far_plane);
  updateViewFromModel();
}

void CameraController::onUpdateViewportCamera() {
  std::cout << "Render camera updated." << '\n';

  if(m_realtime_controller != nullptr) {
    m_realtime_controller->updateViewportCamera();
  }
}

void CameraController::onUpdateRenderCamera() {
  std::cout << "Render camera updated." << '\n';
  if(m_realtime_controller != nullptr) {
    m_realtime_controller->updateRenderCamera();
  }
  m_transform_controller->updateViewFromModel();
}
