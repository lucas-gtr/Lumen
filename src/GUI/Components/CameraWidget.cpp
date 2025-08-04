// GCOVR_EXCL_START
#include "CameraWidget.hpp"
#include "Core/Config.hpp"
#include "SceneObjects/Camera.hpp"
#include "ui_CameraWidget.h"

CameraWidget::CameraWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::CameraWidget) {
  ui->setupUi(this);

  setStyleSheet(QString::fromStdString(std::string(GROUP_BOX_STYLESHEET)));

  connect(ui->horizontalFovSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onHorizontalFovChanged);
  connect(ui->sensorWidthSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onSensorWidthChanged);
  connect(ui->focalLengthSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onFocalLengthChanged);
  connect(ui->apertureSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onApertureChanged);
  connect(ui->shutterSpeedSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onShutterSpeedChanged);
  connect(ui->focusDistanceSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onFocusDistanceChanged);
  connect(ui->nearPlaneSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onNearPlaneChanged);
  connect(ui->farPlaneSpinBox, &QDoubleSpinBox::valueChanged, this, &CameraWidget::onFarPlaneChanged);
  connect(ui->updateViewportCameraButton, &QPushButton::clicked, this, &CameraWidget::viewportCameraUpdated);
  connect(ui->updateRenderCameraButton, &QPushButton::clicked, this, &CameraWidget::renderCameraUpdated);
}

void CameraWidget::setCamera(Camera* camera) {
  m_camera = camera;
  if(m_camera == nullptr) {
    return;
  }

  ui->horizontalFovSpinBox->setValue(m_camera->getFovX());
  ui->sensorWidthSpinBox->setValue(m_camera->getSensorWidth() * M_TO_MM);
  ui->focalLengthSpinBox->setValue(m_camera->getFocalLength() * M_TO_MM);
  ui->apertureSpinBox->setValue(m_camera->getAperture());
  ui->shutterSpeedSpinBox->setValue(1.0 / m_camera->getShutterSpeed());
  ui->focusDistanceSpinBox->setValue(m_camera->getFocusDistance());
  ui->nearPlaneSpinBox->setValue(m_camera->getNearPlane());
  ui->farPlaneSpinBox->setValue(m_camera->getFarPlane());
}

void CameraWidget::onHorizontalFovChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setHorizontalFov(value);
    ui->sensorWidthSpinBox->setValue(m_camera->getSensorWidth() * M_TO_MM);
  }
}

void CameraWidget::onSensorWidthChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setSensorWidth(value);
    ui->horizontalFovSpinBox->setValue(m_camera->getFovX());
  }
}

void CameraWidget::onFocalLengthChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setFocalLength(value);
    ui->horizontalFovSpinBox->setValue(m_camera->getFovX());
  }
}

void CameraWidget::onApertureChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setAperture(value);
  }
}

void CameraWidget::onShutterSpeedChanged(double value) {
  if(m_camera != nullptr && value != 0.0) {
    m_camera->setShutterSpeed(1.0 / value);
  }
}

void CameraWidget::onFocusDistanceChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setFocusDistance(value);
  }
}

void CameraWidget::onNearPlaneChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setNearPlane(value);
    ui->farPlaneSpinBox->setMinimum(value + MIN_CAMERA_PLANE);
  }
}

void CameraWidget::onFarPlaneChanged(double value) {
  if(m_camera != nullptr) {
    m_camera->setFarPlane(value);
    ui->nearPlaneSpinBox->setMaximum(value - MIN_CAMERA_PLANE);
  }
}

CameraWidget::~CameraWidget() { delete ui; }
// GCOVR_EXCL_STOP