#include <qboxlayout.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <string_view>

#include "Core/Config.hpp"
#include "Qt/TextureSelectorWidget.hpp"
#include "Surface/TextureManager.hpp"

TextureSelectorWidget::TextureSelectorWidget(const std::string_view& default_texture_name, QWidget* parent)
    : QWidget(parent), m_default_texture_name(default_texture_name) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void TextureSelectorWidget::setupUI() {
  m_texture_selector = new QComboBox(this);
  m_texture_selector->addItem(QString::fromStdString(m_default_texture_name));
  m_texture_selector->setMinimumWidth(TEXTURE_MATERIAL_SELECTOR_MINIMUM_WIDTH);

  m_browse_texture_button = new QPushButton("Browse", this);
  m_browse_texture_button->setFixedWidth(BROWSE_BUTTON_WIDTH);
  auto* layout = new QHBoxLayout;

  layout->addWidget(m_texture_selector);
  layout->addWidget(m_browse_texture_button);
  setLayout(layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void TextureSelectorWidget::connectSignals() {
  connect(m_texture_selector, &QComboBox::currentTextChanged, this, &TextureSelectorWidget::textureSelected);
  connect(m_browse_texture_button, &QPushButton::clicked, this, &TextureSelectorWidget::onBrowseTextureButtonClicked);
}

void TextureSelectorWidget::connectToTextureManager(TextureManager* texture_manager) {
  texture_manager->getTextureAddedObserver().add(
      [this](const std::string& texture_name) { onTextureAdded(texture_name); });
  texture_manager->getTextureRemovedObserver().add(
      [this](const std::string& texture_name) { onTextureRemoved(texture_name); });
  texture_manager->getTextureRenamedObserver().add(
      [this](const std::string& old_name, const std::string& new_name) { onTextureRenamed(old_name, new_name); });
}

void TextureSelectorWidget::onBrowseTextureButtonClicked() {
  const QString file_path =
      QFileDialog::getOpenFileName(this, "Select Texture File", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga)");
  if(!file_path.isEmpty()) {
    emit textureCreationRequested(file_path);
  }
}

void TextureSelectorWidget::onTextureAdded(const std::string& texture_name) {
  const QSignalBlocker blocker(m_texture_selector);
  m_texture_selector->addItem(QString::fromStdString(texture_name));
}

void TextureSelectorWidget::onTextureRemoved(const std::string& texture_name) {
  m_texture_selector->blockSignals(true);
  const int index = m_texture_selector->findText(QString::fromStdString(texture_name));
  if(index != -1) {
    const bool was_selected = (m_texture_selector->currentIndex() == index);
    m_texture_selector->removeItem(index);
    if(was_selected && m_texture_selector->count() > 0) {
      m_texture_selector->blockSignals(false);
      m_texture_selector->setCurrentIndex(0);
    }
  }
}

void TextureSelectorWidget::onTextureRenamed(const std::string& old_name, const std::string& new_name) {
  const QSignalBlocker blocker(m_texture_selector);
  const int            index = m_texture_selector->findText(QString::fromStdString(old_name));
  if(index != -1) {
    const bool was_selected = (m_texture_selector->currentIndex() == index);
    m_texture_selector->setItemText(index, QString::fromStdString(new_name));
    if(was_selected) {
      m_texture_selector->setCurrentIndex(index);
    }
  }
}

void TextureSelectorWidget::setCurrentTexture(const QString& texture_name) {
  const QSignalBlocker blocker(m_texture_selector);
  const int            index = m_texture_selector->findText(texture_name);
  if(index != -1) {
    m_texture_selector->setCurrentIndex(index);
  } else {
    m_texture_selector->setCurrentIndex(0); // No match found
  }
}