#include <qobject.h>
#include <qstring.h>
#include <string>

#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/ViewportController.hpp"
#include "Qt/View/ViewportView.hpp"

ViewportController::ViewportController(ViewportView* view, RealtimeController* realtime_controller)
    : m_view(view), m_realtime_controller(realtime_controller) {
  connectSignals();
}

void ViewportController::connectSignals() {
  connect(m_view, &ViewportView::cameraRotationSpeedChanged, this, &ViewportController::onRotationSpeedChanged);
  connect(m_view, &ViewportView::cameraZoomSpeedChanged, this, &ViewportController::onZoomSpeedChanged);
  connect(m_view, &ViewportView::cameraMoveSpeedChanged, this, &ViewportController::onMoveSpeedChanged);
  connect(m_view, &ViewportView::shadowMapSizeChanged, this, &ViewportController::onShadowMapSizeChanged);
  connect(m_view, &ViewportView::dynamicLightChanged, this, &ViewportController::onDynamicLightingChanged);
  connect(m_view, &ViewportView::dynamicShadowMapChanged, this, &ViewportController::onDynamicShadowChanged);
  connect(m_view, &ViewportView::bakeLights, this, &ViewportController::onBakeLights);
  connect(m_view, &ViewportView::toneMappingChanged, this, &ViewportController::onToneMappingChanged);
  connect(m_view, &ViewportView::exposureChanged, this, &ViewportController::onExposureChanged);
}

void ViewportController::onRotationSpeedChanged(double speed) {
  m_realtime_controller->getEngine()->setCameraRotationSpeed(static_cast<float>(speed));
}

void ViewportController::onZoomSpeedChanged(double speed) {
  m_realtime_controller->getEngine()->setCameraZoomSpeed(static_cast<float>(speed));
}

void ViewportController::onMoveSpeedChanged(double speed) {
  m_realtime_controller->getEngine()->setCameraMoveSpeed(static_cast<float>(speed));
}

void ViewportController::onShadowMapSizeChanged(int size) {
  m_realtime_controller->getEngine()->setShadowMapSize(size);
}

void ViewportController::onDynamicLightingChanged(bool enabled) {
  m_realtime_controller->getEngine()->setDynamicLighting(enabled);
}

void ViewportController::onDynamicShadowChanged(bool enabled) {
  m_realtime_controller->getEngine()->setDynamicShadowMap(enabled);
}

void ViewportController::onToneMappingChanged(const QString& mode) {
  ToneMapping tone_mapping = ToneMapping::NONE;
  if(mode == "Reinhard") {
    tone_mapping = ToneMapping::REINHARD;
  } else if(mode == "Exposure") {
    tone_mapping = ToneMapping::EXPOSURE;
  } else {
    tone_mapping = ToneMapping::NONE;
  }
  m_realtime_controller->getEngine()->setToneMapping(tone_mapping);
}

void ViewportController::onBakeLights() { m_realtime_controller->bakeLights(); }

void ViewportController::onExposureChanged(double exposure) {
  m_realtime_controller->getEngine()->setExposure(static_cast<float>(exposure));
}

void ViewportController::updateViewFromModel() {
  auto* engine = m_realtime_controller->getEngine();
  m_view->setCameraRotationSpeed(engine->cameraRotationSpeed());
  m_view->setCameraZoomSpeed(engine->cameraZoomSpeed());
  m_view->setCameraMoveSpeed(engine->cameraMoveSpeed());
  m_view->setShadowMapSize(engine->getShadowMapSize());
  m_view->setDynamicLighting(engine->isDynamicLightingEnabled());
  m_view->setDynamicShadowMap(engine->isDynamicShadowMapEnabled());

  const ToneMapping tone_mapping = engine->getToneMapping();
  std::string       toneMappingStr;
  switch(tone_mapping) {
  case ToneMapping::REINHARD:
    toneMappingStr = "Reinhard";
    break;
  case ToneMapping::EXPOSURE:
    toneMappingStr = "Exposure";
    break;
  default:
    toneMappingStr = "None";
    break;
  }
  m_view->setToneMapping(QString::fromStdString(toneMappingStr));

  m_view->setExposure(engine->getExposure());
}
