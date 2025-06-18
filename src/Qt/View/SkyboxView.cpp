#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Qt/TexturePreviewWidget.hpp"
#include "Qt/TextureSelectorWidget.hpp"
#include "Qt/View/SkyboxView.hpp"

SkyboxView::SkyboxView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void SkyboxView::setupUI() {
  auto* group_box = new QGroupBox("Skybox Settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight: bold; font-size: 14px;}");

  m_preview_image_label =
      new TexturePreviewWidget(SKYBOX_VIEW_TEXTURE_PREVIEW_MAX_SIZE, SKYBOX_VIEW_TEXTURE_PREVIEW_MAX_SIZE, group_box);

  auto* image_layout = new QVBoxLayout;
  image_layout->addWidget(m_preview_image_label, 0, Qt::AlignHCenter);

  m_texture_selector = new TextureSelectorWidget(DEFAULT_SKYBOX_NAME, group_box);

  m_display_checkbox = new QCheckBox("Display in viewport", group_box);

  auto* group_layout = new QVBoxLayout;
  group_layout->addLayout(image_layout);
  group_layout->addWidget(m_texture_selector);
  group_layout->addWidget(m_display_checkbox);

  group_box->setLayout(group_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(group_box);
  main_layout->addStretch();

  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void SkyboxView::connectSignals() {
  connect(m_texture_selector, &TextureSelectorWidget::textureSelected, this, &SkyboxView::textureSelected);
  connect(m_texture_selector, &TextureSelectorWidget::textureCreationRequested, this,
          &SkyboxView::skyboxTextureCreated);
  connect(m_display_checkbox, &QCheckBox::toggled, this, &SkyboxView::displayInViewportToggled);
}

void SkyboxView::onSkyboxPreviewImageChanged(const unsigned char*   preview_data,
                                             const ImageProperties& preview_properties) {
  m_preview_image_label->setTexturePreview(preview_data, preview_properties);
}

void SkyboxView::setSkybox(const QString& texture_name) { m_texture_selector->setCurrentTexture(texture_name); }

void SkyboxView::setViewportDisplayEnabled(bool enabled) {
  const QSignalBlocker blocker(m_display_checkbox);
  m_display_checkbox->setChecked(enabled);
}