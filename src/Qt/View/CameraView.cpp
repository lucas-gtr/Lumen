#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qglobal.h>
#include <qgroupbox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/View/CameraView.hpp"

CameraView::CameraView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void CameraView::setupUI() {
  auto* projection_mode_combo = new QComboBox(this);
  projection_mode_combo->addItem("Perspective");
  projection_mode_combo->addItem("Orthographic");
  projection_mode_combo->setItemData(1, false, Qt::UserRole - 1);
  m_projection_mode_combo = projection_mode_combo;

  auto* fov_spin = new QDoubleSpinBox(this);
  fov_spin->setRange(MIN_HORIZONTAL_FOV, MAX_HORIZONTAL_FOV);
  fov_spin->setSuffix(" °");
  m_fov_spin = fov_spin;

  auto* sensor_width_spin = new QDoubleSpinBox(this);
  sensor_width_spin->setRange(1.0, 100.0);
  sensor_width_spin->setSuffix(" mm");
  m_sensor_width_spin = sensor_width_spin;

  auto* focal_length_spin = new QDoubleSpinBox(this);
  focal_length_spin->setRange(MIN_FOCAL_LENGTH, MAX_FOCAL_LENGTH);
  focal_length_spin->setSuffix(" mm");
  m_focal_length_spin = focal_length_spin;

  auto* aperture_spin = new QDoubleSpinBox(this);
  aperture_spin->setRange(MIN_APERTURE, MAX_APERTURE);
  aperture_spin->setSingleStep(APERTURE_STEP);
  m_aperture_spin = aperture_spin;

  auto* shutter_speed_spin = new QDoubleSpinBox(this);
  shutter_speed_spin->setRange(1.0 / MAX_SHUTTER_SPEED, 1.0 / MIN_SHUTTER_SPEED);
  shutter_speed_spin->setPrefix("1/");
  shutter_speed_spin->setSuffix(" s");
  m_shutter_speed_spin = shutter_speed_spin;

  auto* focus_distance_spin = new QDoubleSpinBox(this);
  focus_distance_spin->setRange(MIN_FOCUS_DISTANCE, MAX_FOCUS_DISTANCE);
  focus_distance_spin->setSuffix(" m");
  m_focus_distance_spin = focus_distance_spin;

  auto* near_plane_spin = new QDoubleSpinBox(this);
  near_plane_spin->setRange(MIN_CAMERA_PLANE, MAX_CAMERA_PLANE);
  near_plane_spin->setSuffix(" m");
  m_near_plane_spin = near_plane_spin;

  auto* far_plane_spin = new QDoubleSpinBox(this);
  far_plane_spin->setRange(MIN_CAMERA_PLANE, MAX_CAMERA_PLANE);
  far_plane_spin->setSuffix(" m");
  m_far_plane_spin = far_plane_spin;

  auto* update_viewport_camera_button = new QPushButton("Update Viewport Camera from Render Camera", this);
  update_viewport_camera_button->setToolTip("Update the viewport camera with the current render camera position");
  m_update_viewport_camera_button = update_viewport_camera_button;

  auto* update_render_camera_button = new QPushButton("Update Render Camera from Viewport Camera", this);
  update_render_camera_button->setToolTip("Update the render camera with the current viewport camera position");
  m_update_render_camera_button = update_render_camera_button;

  auto* layout = new QFormLayout;
  layout->addRow("Projection Mode", projection_mode_combo);
  layout->addRow("Horizontal FOV", fov_spin);
  layout->addRow("Sensor Width", sensor_width_spin);
  layout->addRow("Focal Length", focal_length_spin);
  layout->addRow("Aperture", aperture_spin);
  layout->addRow("Shutter Speed", shutter_speed_spin);
  layout->addRow("Focus Distance", focus_distance_spin);
  layout->addRow("Near Plane", near_plane_spin);
  layout->addRow("Far Plane", far_plane_spin);
  layout->addRow(update_viewport_camera_button);
  layout->addRow(update_render_camera_button);

  auto* group_box = new QGroupBox("Camera Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");
  group_box->setLayout(layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(group_box);
  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void CameraView::connectSignals() {
  connect(m_projection_mode_combo, &QComboBox::currentTextChanged, this, &CameraView::projectionModeChanged);
  connect(m_fov_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CameraView::horizontalFovChanged);
  connect(m_sensor_width_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &CameraView::sensorWidthChanged);
  connect(m_focal_length_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &CameraView::focalLengthChanged);
  connect(m_aperture_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CameraView::apertureChanged);
  connect(m_shutter_speed_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &CameraView::shutterSpeedChanged);
  connect(m_focus_distance_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &CameraView::focusDistanceChanged);
  connect(m_near_plane_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CameraView::nearPlaneChanged);
  connect(m_far_plane_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CameraView::farPlaneChanged);
  connect(m_update_viewport_camera_button, &QPushButton::clicked, this, &CameraView::updateViewportCamera);
  connect(m_update_render_camera_button, &QPushButton::clicked, this, &CameraView::updateRenderCamera);
}

void CameraView::setProjectionMode(const QString& mode) {
  const int index = m_projection_mode_combo->findText(mode);
  if(index != -1) {
    m_projection_mode_combo->setCurrentIndex(index);
  }
}

void CameraView::setHorizontalFov(double fov) {
  const QSignalBlocker blocker(m_fov_spin);
  m_fov_spin->setValue(fov);
}

void CameraView::setSensorWidth(double width) {
  const QSignalBlocker blocker(m_sensor_width_spin);
  m_sensor_width_spin->setValue(width);
}

void CameraView::setFocalLength(double focal_length) {
  const QSignalBlocker blocker(m_focal_length_spin);
  m_focal_length_spin->setValue(focal_length);
}

void CameraView::setAperture(double aperture) {
  const QSignalBlocker blocker(m_aperture_spin);
  m_aperture_spin->setValue(aperture);
}

void CameraView::setShutterSpeed(double shutter_speed) {
  const QSignalBlocker blocker(m_shutter_speed_spin);
  m_shutter_speed_spin->setValue(shutter_speed);
}

void CameraView::setFocusDistance(double focus_distance) {
  const QSignalBlocker blocker(m_focus_distance_spin);
  m_focus_distance_spin->setValue(focus_distance);
}

void CameraView::setNearPlane(double near_plane) {
  const QSignalBlocker blocker(m_near_plane_spin);
  m_near_plane_spin->setValue(near_plane);
}

void CameraView::setFarPlane(double far_plane) {
  const QSignalBlocker blocker(m_far_plane_spin);
  m_far_plane_spin->setValue(far_plane);
}
