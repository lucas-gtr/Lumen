#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/View/ViewportView.hpp"

ViewportView::ViewportView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void ViewportView::setupUI() {
  auto* group_box = new QGroupBox("Viewport settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");

  auto* form_layout = new QFormLayout(group_box);

  m_camera_rotation_speed_spin = new QDoubleSpinBox(group_box);
  m_camera_rotation_speed_spin->setRange(MIN_CAMERA_ROTATION_SPEED, MAX_CAMERA_ROTATION_SPEED);
  m_camera_rotation_speed_spin->setSingleStep(VIEWPORT_CAMERA_MOUSE_DRAG_STEP);
  m_camera_rotation_speed_spin->setDecimals(VIEWPORT_CAMERA_SETTINGS_DECIMALS);
  form_layout->addRow("Camera rotation speed", m_camera_rotation_speed_spin);

  m_camera_zoom_speed_spin = new QDoubleSpinBox(group_box);
  m_camera_zoom_speed_spin->setRange(MIN_CAMERA_ZOOM_SPEED, MAX_CAMERA_ZOOM_SPEED);
  m_camera_zoom_speed_spin->setSingleStep(VIEWPORT_CAMERA_MOUSE_WHEEL_STEP);
  form_layout->addRow("Camera zoom speed", m_camera_zoom_speed_spin);

  m_camera_move_speed_spin = new QDoubleSpinBox(group_box);
  m_camera_move_speed_spin->setRange(MIN_CAMERA_MOVE_SPEED, MAX_CAMERA_MOVE_SPEED);
  m_camera_move_speed_spin->setSingleStep(VIEWPORT_CAMERA_MOUSE_DRAG_STEP);
  m_camera_move_speed_spin->setDecimals(VIEWPORT_CAMERA_SETTINGS_DECIMALS);
  form_layout->addRow("Camera move speed", m_camera_move_speed_spin);

  m_shadow_map_size_spin = new QSpinBox(group_box);
  m_shadow_map_size_spin->setRange(MIN_SHADOW_MAP_SIZE, MAX_SHADOW_MAP_SIZE);
  m_shadow_map_size_spin->setSingleStep(SHADOW_MAP_SIZE_STEP);
  m_shadow_map_size_spin->setSuffix(" px");
  form_layout->addRow("Shadow map size", m_shadow_map_size_spin);

  m_dynamic_light_check = new QCheckBox(group_box);
  form_layout->addRow("Dynamic light", m_dynamic_light_check);

  m_dynamic_shadow_map_check = new QCheckBox(group_box);
  m_dynamic_shadow_map_check->setEnabled(false);
  form_layout->addRow("Dynamic shadow map", m_dynamic_shadow_map_check);

  m_bake_lights_button = new QPushButton("Bake lights", group_box);
  form_layout->addRow(m_bake_lights_button);

  m_tone_mapping_combo = new QComboBox(group_box);
  m_tone_mapping_combo->addItems({"None", "Reinhard", "Exposure"});
  form_layout->addRow("Tone mapping", m_tone_mapping_combo);

  m_exposure_spin = new QDoubleSpinBox(group_box);
  m_exposure_spin->setRange(MIN_TONE_MAPPING_EXPOSURE, MAX_TONE_MAPPING_EXPOSURE);
  m_exposure_spin->setSingleStep(EXPOSURE_STEP);
  m_exposure_spin->setVisible(false);

  m_exposure_label = new QLabel("Exposure", group_box);
  m_exposure_label->setVisible(false);
  form_layout->addRow(m_exposure_label, m_exposure_spin);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(group_box);
  main_layout->addStretch();
  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void ViewportView::connectSignals() {
  connect(m_camera_rotation_speed_spin, &QDoubleSpinBox::valueChanged, this, &ViewportView::cameraRotationSpeedChanged);
  connect(m_camera_zoom_speed_spin, &QDoubleSpinBox::valueChanged, this, &ViewportView::cameraZoomSpeedChanged);
  connect(m_camera_move_speed_spin, &QDoubleSpinBox::valueChanged, this, &ViewportView::cameraMoveSpeedChanged);
  connect(m_shadow_map_size_spin, &QSpinBox::valueChanged, this, &ViewportView::shadowMapSizeChanged);
  connect(m_dynamic_light_check, &QCheckBox::toggled, this, &ViewportView::onDynamicLightToggled);
  connect(m_dynamic_shadow_map_check, &QCheckBox::toggled, this, &ViewportView::dynamicShadowMapChanged);
  connect(m_bake_lights_button, &QPushButton::clicked, this, &ViewportView::bakeLights);
  connect(m_tone_mapping_combo, &QComboBox::currentTextChanged, this, &ViewportView::onToneMappingChanged);
  connect(m_exposure_spin, &QDoubleSpinBox::valueChanged, this, &ViewportView::exposureChanged);
}

void ViewportView::onToneMappingChanged(const QString& mode) {
  m_exposure_label->setVisible(mode == "Exposure");
  m_exposure_spin->setVisible(mode == "Exposure");
  emit toneMappingChanged(mode);
}

void ViewportView::onDynamicLightToggled(bool enabled) {
  m_dynamic_shadow_map_check->setEnabled(enabled);
  emit dynamicLightChanged(enabled);
}

void ViewportView::setCameraRotationSpeed(double speed) {
  const QSignalBlocker blocker(m_camera_rotation_speed_spin);
  m_camera_rotation_speed_spin->setValue(speed);
}

void ViewportView::setCameraZoomSpeed(double speed) {
  const QSignalBlocker blocker(m_camera_zoom_speed_spin);
  m_camera_zoom_speed_spin->setValue(speed);
}

void ViewportView::setCameraMoveSpeed(double speed) {
  const QSignalBlocker blocker(m_camera_move_speed_spin);
  m_camera_move_speed_spin->setValue(speed);
}

void ViewportView::setShadowMapSize(int size) {
  const QSignalBlocker blocker(m_shadow_map_size_spin);
  m_shadow_map_size_spin->setValue(size);
}

void ViewportView::setDynamicLighting(bool enabled) {
  const QSignalBlocker blocker(m_dynamic_light_check);
  m_dynamic_light_check->setChecked(enabled);
  m_dynamic_shadow_map_check->setEnabled(enabled);
}

void ViewportView::setDynamicShadowMap(bool enabled) {
  const QSignalBlocker blocker(m_dynamic_shadow_map_check);
  m_dynamic_shadow_map_check->setChecked(enabled);
}

void ViewportView::setToneMapping(const QString& mode) {
  const QSignalBlocker blocker(m_tone_mapping_combo);
  m_tone_mapping_combo->setCurrentText(mode);
  m_exposure_label->setVisible(mode == "Exposure");
  m_exposure_spin->setVisible(mode == "Exposure");
}

void ViewportView::setExposure(double exposure) {
  const QSignalBlocker blocker(m_exposure_spin);
  m_exposure_spin->setValue(exposure);
}
