#include "LightWidget.hpp"
#include "Lighting/DirectionalLight.hpp"
#include "Lighting/Light.hpp"
#include "Lighting/SpotLight.hpp"
#include "ui_LightWidget.h"

#include "Core/Config.hpp"

LightWidget::LightWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::LightWidget) {
  ui->setupUi(this);

  setStyleSheet(QString::fromStdString(std::string(GROUP_BOX_STYLESHEET)));

  connect(ui->colorButton, &ColorButton::colorChanged, this, &LightWidget::onColorChanged);
  connect(ui->intensitySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &LightWidget::onIntensityChanged);

  connect(ui->directionEditor, &Vec3Editor::valueChanged, this, &LightWidget::onDirectionChanged);

  connect(ui->spotDirectionEditor, &Vec3Editor::valueChanged, this, &LightWidget::onSpotDirectionChanged);
  connect(ui->innerAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &LightWidget::onInnerAngleChanged);
  connect(ui->outerAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &LightWidget::onOuterAngleChanged);
}

void LightWidget::setLight(Light* light) {
  m_light = light;
  if(m_light == nullptr) {
    return;
  }
  updateWidget();
}

void LightWidget::updateWidget() {
  if(m_light == nullptr) {
    return;
  }

  switch(m_light->getType()) {
  case LightType::POINT: {
    ui->lightTypeWidget->setCurrentIndex(0);
    break;
  }
  case LightType::SPOT: {
    ui->lightTypeWidget->setCurrentIndex(1);
    auto* spot_light = dynamic_cast<SpotLight*>(m_light);
    if(spot_light == nullptr) {
      std::cerr << "LightWidget::updateWidget: Light is not a SpotLight.\n";
      return;
    }
    ui->spotDirectionEditor->setValue(spot_light->getDirection());
    ui->innerAngleSpinBox->setValue(spot_light->getInnerAngleDeg());
    ui->outerAngleSpinBox->setValue(spot_light->getOuterAngleDeg());
    break;
  }
  case LightType::DIRECTIONAL: {
    ui->lightTypeWidget->setCurrentIndex(2);
    auto* directional_light = dynamic_cast<DirectionalLight*>(m_light);
    if(directional_light == nullptr) {
      std::cerr << "LightWidget::updateWidget: Light is not a DirectionalLight.\n";
      return;
    }
    ui->directionEditor->setValue(directional_light->getDirection());
    break;
  }
  default:
    std::cerr << "Unknown light type: " << static_cast<int>(m_light->getType()) << '\n';
    return;
  }

  QWidget* current = ui->lightTypeWidget->currentWidget();
  ui->lightTypeWidget->setFixedHeight(current->sizeHint().height());

  ui->colorButton->setColor(m_light->getColor());
  ui->intensitySpinBox->setValue(m_light->getIntensity());
}

void LightWidget::onColorChanged(const ColorRGB& new_color) {
  if(m_light != nullptr) {
    m_light->setColor(new_color);
  }
}

void LightWidget::onIntensityChanged(double value) {
  if(m_light != nullptr) {
    m_light->setIntensity(value);
  }
}

void LightWidget::onDirectionChanged(const linalg::Vec3d& new_direction) {
  if(m_light != nullptr) {
    auto* directional_light = dynamic_cast<DirectionalLight*>(m_light);
    if(directional_light != nullptr) {
      directional_light->setDirection(new_direction);
    } else {
      std::cerr << "LightWidget::onDirectionChanged: Light is not a DirectionalLight.\n";
    }
  }
}

void LightWidget::onSpotDirectionChanged(const linalg::Vec3d& new_direction) {
  if(m_light != nullptr) {
    auto* spot_light = dynamic_cast<SpotLight*>(m_light);
    if(spot_light != nullptr) {
      spot_light->setDirection(new_direction);
    } else {
      std::cerr << "LightWidget::onSpotDirectionChanged: Light is not a SpotLight.\n";
    }
  }
}

void LightWidget::onInnerAngleChanged(double value) {
  if(m_light != nullptr) {
    auto* spot_light = dynamic_cast<SpotLight*>(m_light);
    if(spot_light != nullptr) {
      spot_light->setInnerAngle(value);
    } else {
      std::cerr << "LightWidget::onInnerAngleChanged: Light is not a SpotLight.\n";
    }
  }
}

void LightWidget::onOuterAngleChanged(double value) {
  if(m_light != nullptr) {
    auto* spot_light = dynamic_cast<SpotLight*>(m_light);
    if(spot_light != nullptr) {
      spot_light->setOuterAngle(value);
    } else {
      std::cerr << "LightWidget::onOuterAngleChanged: Light is not a SpotLight.\n";
    }
  }
}

LightWidget::~LightWidget() { delete ui; }