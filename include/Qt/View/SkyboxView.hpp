#ifndef QT_VIEW_SKYBOXVIEW_HPP
#define QT_VIEW_SKYBOXVIEW_HPP

#include <qcheckbox.h>
#include <qlabel.h>
#include <qstring.h>

#include "Core/CommonTypes.hpp"
#include "Qt/TexturePreviewWidget.hpp"
#include "Qt/TextureSelectorWidget.hpp"

class SkyboxView : public QWidget {
  Q_OBJECT

public:
  explicit SkyboxView(QWidget* parent);

  void setSkybox(const QString& texture_name);
  void setViewportDisplayEnabled(bool enabled);

  TextureSelectorWidget* getTextureSelectorWidget() const { return m_texture_selector; }

signals:
  void textureSelected(const QString& texture_name);
  void displayInViewportToggled(bool enabled);
  void skyboxTextureCreated(const QString& file_path);

public slots:
  void onSkyboxPreviewImageChanged(const unsigned char* preview_data, const ImageProperties& preview_properties);

private:
  TexturePreviewWidget*  m_preview_image_label = nullptr;
  TextureSelectorWidget* m_texture_selector    = nullptr;
  QCheckBox*             m_display_checkbox    = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_SKYBOXVIEW_HPP
