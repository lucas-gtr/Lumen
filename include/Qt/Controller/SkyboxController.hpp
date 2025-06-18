#ifndef QT_CONTROLLER_SKYBOXCONTROLLER_HPP
#define QT_CONTROLLER_SKYBOXCONTROLLER_HPP

#include <qobject.h>
#include <qstring.h>

#include "GPU/OpenGL/EngineGL.hpp"
#include "Qt/Controller/RealtimeController.hpp"
#include "Qt/Controller/TextureController.hpp"
#include "Qt/View/SkyboxView.hpp"
#include "Scene/Skybox.hpp"
#include "Surface/TextureManager.hpp"

class SkyboxController : public QObject {
  Q_OBJECT
public:
  explicit SkyboxController(SkyboxView* view, Skybox* skybox, TextureController* texture_controller,
                            TextureManager* texture_manager, RealtimeController* m_realtime_controller);

  void updateViewFromModel();

signals:
  void skyboxImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);

private:
  // Slots
  void onTextureSelected(const QString& texture_name);
  void onDisplayInViewportToggled(bool enabled);
  void createSkyboxTextureFromFile(const QString& file_name);

  // Members
  SkyboxView* m_view;
  Skybox*     m_skybox;

  TextureController* m_texture_controller;

  const TextureManager* m_texture_manager;
  RealtimeController*   m_realtime_controller;

  void updateSkyboxTexture(const std::string& texture_name);
  void resetSkyboxTextureToDefault();

  void connectSignals();
};

#endif // QT_CONTROLLER_SKYBOXCONTROLLER_HPP