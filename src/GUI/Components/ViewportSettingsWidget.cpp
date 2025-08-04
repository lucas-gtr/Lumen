// GCOVR_EXCL_START
#include "ViewportSettingsWidget.hpp"
#include "GPU/OpenGL/EngineGL.hpp"
#include "ui_ViewportSettingsWidget.h"

ViewportSettingsWidget::ViewportSettingsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::ViewportSettingsWidget) {
  ui->setupUi(this);

  connect(ui->cameraRotationSpeedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ViewportSettingsWidget::onCameraRotationSpeedChanged);
  connect(ui->cameraMoveSpeedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ViewportSettingsWidget::onCameraMovementSpeedChanged);
  connect(ui->cameraZoomSpeedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ViewportSettingsWidget::onCameraZoomSpeedChanged);
  connect(ui->shadowMapSizeSpinBox, &QSpinBox::valueChanged, this, &ViewportSettingsWidget::onShadowMapSizeChanged);
  connect(ui->dynamicShadowMapCheckBox, &QCheckBox::toggled, this, &ViewportSettingsWidget::onDynamicShadowsChanged);
  connect(ui->toneMappingComboBox, &QComboBox::currentTextChanged, this, &ViewportSettingsWidget::onToneMappingChanged);
  connect(ui->exposureSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ViewportSettingsWidget::onExposureChanged);
  connect(ui->bakeLightButton, &QPushButton::clicked, this, &ViewportSettingsWidget::lightBakeRequested);
}

void ViewportSettingsWidget::updateWidget() {
  if(m_engine == nullptr) {
    return;
  }

  ui->cameraRotationSpeedSpinBox->setValue(m_engine->cameraRotationSpeed());
  ui->cameraMoveSpeedSpinBox->setValue(m_engine->cameraMoveSpeed());
  ui->cameraZoomSpeedSpinBox->setValue(m_engine->cameraZoomSpeed());
  ui->shadowMapSizeSpinBox->setValue(m_engine->getShadowMapSize());
  ui->dynamicShadowMapCheckBox->setChecked(m_engine->isDynamicShadowMapEnabled());
  ui->exposureSpinBox->setValue(m_engine->getExposure());

  QString tone_mapping;
  switch(m_engine->getToneMapping()) {
  case ToneMapping::NONE:
    tone_mapping = "None";
    break;
  case ToneMapping::LUMINANCE_REINHARD:
    tone_mapping = "Reinhard";
    break;
  case ToneMapping::EXPOSURE:
    tone_mapping = "Exposure";
    break;
  default:
    tone_mapping = "None";
  }
  ui->toneMappingComboBox->setCurrentText(tone_mapping);
  ui->exposureSpinBox->setVisible(tone_mapping == "Exposure");
  ui->exposureLabel->setVisible(tone_mapping == "Exposure");
}

void ViewportSettingsWidget::setEngine(EngineGL* engine) {
  m_engine = engine;
  updateWidget();
}

void ViewportSettingsWidget::onCameraRotationSpeedChanged(double value) {
  if(m_engine != nullptr) {
    m_engine->setCameraRotationSpeed(static_cast<float>(value));
  }
}

void ViewportSettingsWidget::onCameraMovementSpeedChanged(double value) {
  if(m_engine != nullptr) {
    m_engine->setCameraMoveSpeed(static_cast<float>(value));
  }
}

void ViewportSettingsWidget::onCameraZoomSpeedChanged(double value) {
  if(m_engine != nullptr) {
    m_engine->setCameraZoomSpeed(static_cast<float>(value));
  }
}

void ViewportSettingsWidget::onShadowMapSizeChanged(int value) {
  if(m_engine != nullptr) {
    m_engine->setShadowMapSize(value);
  }
}

void ViewportSettingsWidget::onDynamicShadowsChanged(bool checked) {
  if(m_engine != nullptr) {
    m_engine->setDynamicShadowMap(checked);
  }
}

void ViewportSettingsWidget::onToneMappingChanged(const QString& tone_mapping) {
  if(m_engine != nullptr) {
    if(tone_mapping == "None") {
      m_engine->setToneMapping(ToneMapping::NONE);
    } else if(tone_mapping == "Reinhard") {
      m_engine->setToneMapping(ToneMapping::LUMINANCE_REINHARD);
    } else if(tone_mapping == "Exposure") {
      m_engine->setToneMapping(ToneMapping::EXPOSURE);
    }
    ui->exposureSpinBox->setVisible(tone_mapping == "Exposure");
    ui->exposureLabel->setVisible(tone_mapping == "Exposure");
  }
}

void ViewportSettingsWidget::onExposureChanged(double value) {
  if(m_engine != nullptr) {
    m_engine->setExposure(static_cast<float>(value));
  }
}

ViewportSettingsWidget::~ViewportSettingsWidget() { delete ui; }
// GCOVR_EXCL_STOP