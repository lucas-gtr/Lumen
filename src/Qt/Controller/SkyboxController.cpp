#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <string>

#include "Core/Config.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/SkyboxController.hpp"
#include "Qt/Controller/TextureController.hpp"
#include "Qt/View/SkyboxView.hpp"
#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

SkyboxController::SkyboxController(SkyboxView* view, Skybox* skybox, TextureController* texture_controller,
                                   TextureManager* texture_manager, RealtimeController* m_realtime_controller)
    : QObject(view), m_view(view), m_skybox(skybox), m_texture_controller(texture_controller),
      m_texture_manager(texture_manager), m_realtime_controller(m_realtime_controller) {
  m_view->getTextureSelectorWidget()->connectToTextureManager(texture_manager);
  connectSignals();
  emit skyboxImageChanged(m_skybox->getTexture()->getPreviewData(), m_skybox->getTexture()->getPreviewProperties());
}

void SkyboxController::connectSignals() {
  connect(m_view, &SkyboxView::textureSelected, this, &SkyboxController::onTextureSelected);
  connect(m_view, &SkyboxView::displayInViewportToggled, this, &SkyboxController::onDisplayInViewportToggled);
  connect(m_view, &SkyboxView::skyboxTextureCreated, this, &SkyboxController::createSkyboxTextureFromFile);

  connect(this, &SkyboxController::skyboxImageChanged, m_view, &SkyboxView::onSkyboxPreviewImageChanged);
}

void SkyboxController::onTextureSelected(const QString& texture_name) {
  updateSkyboxTexture(texture_name.toStdString());
}

void SkyboxController::onDisplayInViewportToggled(bool enabled) {
  m_realtime_controller->getEngine()->setDrawSkybox(enabled);
}

void SkyboxController::createSkyboxTextureFromFile(const QString& file_name) {
  const std::string new_texture_name =
      m_texture_controller->createTextureFromFile(file_name.toStdString(), ColorSpace::LINEAR, false);

  m_view->setSkybox(QString::fromStdString(new_texture_name));
  updateSkyboxTexture(new_texture_name);
}

void SkyboxController::updateSkyboxTexture(const std::string& texture_name) {
  if(texture_name == DEFAULT_SKYBOX_NAME) {
    resetSkyboxTextureToDefault();
    return;
  }
  Texture* texture = m_texture_manager->getTexture(texture_name);
  m_skybox->setTexture(texture);

  emit skyboxImageChanged(m_skybox->getTexture()->getPreviewData(), m_skybox->getTexture()->getPreviewProperties());
}

void SkyboxController::resetSkyboxTextureToDefault() {
  m_skybox->setTexture(TextureManager::DefaultSkyboxTexture());

  emit skyboxImageChanged(m_skybox->getTexture()->getPreviewData(), m_skybox->getTexture()->getPreviewProperties());
}

void SkyboxController::updateViewFromModel() {
  m_view->setViewportDisplayEnabled(m_realtime_controller->getEngine()->isSkyboxEnabled());

  emit skyboxImageChanged(m_skybox->getTexture()->getPreviewData(), m_skybox->getTexture()->getPreviewProperties());
}
