#include <iostream>
#include <qfileinfo.h>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Qt/Controller/TextureController.hpp"
#include "Qt/View/TextureView.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureManager.hpp"
#include "Surface/TextureWrapping.hpp"

TextureController::TextureController(TextureView* view, TextureManager* texture_manager)
    : QObject(view), m_view(view), m_texture_manager(texture_manager) {
  connectSignals();
}

void TextureController::connectSignals() {
  connect(m_view, &TextureView::textureSelected, this, &TextureController::onTextureSelected);
  connect(m_view, &TextureView::textureUnselected, this, &TextureController::onTextureUnselected);
  connect(m_view, &TextureView::textureNameEdited, this, &TextureController::onTextureNameEdited);
  connect(m_view, &TextureView::textureAdded, this, &TextureController::onTextureAdded);
  connect(m_view, &TextureView::textureRemoved, this, &TextureController::onTextureRemoved);
  connect(m_view, &TextureView::textureTypeChanged, this, &TextureController::onTextureTypeChanged);
  connect(m_view, &TextureView::textureFormatChanged, this, &TextureController::onTextureFormatChanged);
  connect(m_view, &TextureView::textureColorChanged, this, &TextureController::onTextureColorChanged);
  connect(m_view, &TextureView::texturePathChanged, this, &TextureController::onTexturePathChanged);
  connect(m_view, &TextureView::colorSpaceChanged, this, &TextureController::onColorSpaceChanged);
  connect(m_view, &TextureView::filteringChanged, this, &TextureController::onFilteringChanged);
  connect(m_view, &TextureView::wrappingModeChanged, this, &TextureController::onWrappingModeChanged);
  connect(m_view, &TextureView::borderColorChanged, this, &TextureController::onBorderColorChanged);
  connect(m_view, &TextureView::flipVerticallyChanged, this, &TextureController::onFlipVerticallyChanged);

  connect(this, &TextureController::textureAdded, m_view, &TextureView::onTextureAdded);
  connect(this, &TextureController::textureRenamed, m_view, &TextureView::onTextureRenamed);
  connect(this, &TextureController::textureRemoved, m_view, &TextureView::onTextureRemoved);
  connect(this, &TextureController::textureImageChanged, m_view, &TextureView::onTextureImageChanged);
}

void TextureController::onTextureSelected(const QString& name) {
  m_current_texture_name = name.toStdString();
  m_current_texture      = m_texture_manager->getTexture(m_current_texture_name);
  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());

  updateViewFromModel();
}

void TextureController::onTextureUnselected() {
  m_current_texture = nullptr;
  m_current_texture_name.clear();
}

void TextureController::onTextureNameEdited(const QString& new_name) {
  if(m_texture_manager->renameTexture(m_current_texture, new_name.toStdString())) {
    emit textureRenamed(QString::fromStdString(new_name.toStdString()), QString::fromStdString(m_current_texture_name));
    m_current_texture_name = new_name.toStdString();
  } else {
    emit textureRenamed(QString::fromStdString(m_current_texture_name), QString::fromStdString(m_current_texture_name));
  }
}

std::string TextureController::createTextureFromFile(const std::string& file_name, ColorSpace color_space,
                                                     bool flip_vertically) {
  std::string texture_name =
      m_texture_manager->getAvailableTextureName(QFileInfo(QString::fromStdString(file_name)).baseName().toStdString());
  m_texture_manager->addTexture(texture_name);
  m_current_texture_name = texture_name;
  m_current_texture      = m_texture_manager->getTexture(texture_name);
  m_current_texture->loadFromFile(file_name.c_str());
  m_current_texture->setColorSpace(color_space);
  m_current_texture->setFlippedVertically(flip_vertically);

  emit textureAdded(QString::fromStdString(texture_name));

  return texture_name;
}

void TextureController::onTextureAdded(const QString& name) {
  const std::string texture_name = m_texture_manager->getAvailableTextureName(name.toStdString());
  m_texture_manager->addTexture(texture_name);
  m_current_texture_name = texture_name;
  m_current_texture      = m_texture_manager->getTexture(texture_name);

  emit textureAdded(QString::fromStdString(texture_name));
}

void TextureController::onTextureRemoved() {
  if(m_current_texture == nullptr) {
    return;
  }

  m_texture_manager->removeTexture(m_current_texture_name);
  m_current_texture = nullptr;
  emit textureRemoved(QString::fromStdString(m_current_texture_name));
}

void TextureController::onTextureTypeChanged(const QString& type) {
  if(m_current_texture == nullptr) {
    return;
  }

  if(type == "Color Texture") {
    m_current_texture->setTextureType(TextureType::COLOR_TEXTURE);
  } else if(type == "Image Texture") {
    m_current_texture->setTextureType(TextureType::IMAGE_TEXTURE);
  } else {
    std::cerr << "Unknown texture type: " << type.toStdString() << '\n';
    return;
  }
  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
}

void TextureController::onTextureFormatChanged(const QString& format) {
  const int     channel    = m_current_texture->getProperties().channels;
  const double* image_data = m_current_texture->getImageData();
  ColorRGBA     color{0.0, 0.0, 0.0, 1.0};
  switch(channel) {
  case 1:
    color.r = color.g = color.b = image_data[0];
    color.a                     = 1.0;
    break;
  case 3:
    color.r = image_data[0];
    color.g = image_data[1];
    color.b = image_data[2];
    color.a = 1.0;
    break;
  case 4:
    color.r = image_data[0];
    color.g = image_data[1];
    color.b = image_data[2];
    color.a = image_data[3];
    break;
  default:
    std::cerr << "Unknown texture channel count: " << channel << '\n';
    return;
  }

  if(format == "Grayscale") {
    m_current_texture->setValue(toGrayscale(color));
  } else if(format == "RGB") {
    m_current_texture->setValue(ColorRGB{color.r, color.g, color.b});
  } else if(format == "RGBA") {
    m_current_texture->setValue(color);
  } else {
    std::cerr << "Unknown texture format: " << format.toStdString() << '\n';
    return;
  }
  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
}

void TextureController::onTextureColorChanged(const QColor& color) {
  if(m_current_texture == nullptr) {
    return;
  }

  switch(m_current_texture->getProperties().channels) {
  case 1:
    m_current_texture->setValue(toGrayscale(ColorRGB{color.redF(), color.greenF(), color.blueF()}));
    break;
  case 3:
    m_current_texture->setValue(ColorRGB{color.redF(), color.greenF(), color.blueF()});
    break;
  case 4:
    m_current_texture->setValue(ColorRGBA{color.redF(), color.greenF(), color.blueF(), color.alphaF()});
    break;
  default:
    return;
  }
  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
}

void TextureController::onTexturePathChanged(const QString& path) {
  if(m_current_texture == nullptr) {
    return;
  }

  m_current_texture->loadFromFile(path.toStdString().c_str());
  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
}

void TextureController::onFlipVerticallyChanged(bool flip) {
  if(m_current_texture == nullptr) {
    return;
  }
  m_current_texture->setFlippedVertically(flip);
}

void TextureController::onColorSpaceChanged(const QString& color_space) {
  if(m_current_texture == nullptr) {
    return;
  }
  if(color_space == "Linear") {
    m_current_texture->setColorSpace(ColorSpace::Linear);
  } else if(color_space == "sRGB") {
    m_current_texture->setColorSpace(ColorSpace::sRGB);
  } else {
    return;
  }
}

void TextureController::onFilteringChanged(const QString& filtering) {
  if(m_current_texture == nullptr) {
    return;
  }
  if(filtering == "Bilinear") {
    m_current_texture->setFilteringMode(TextureSampling::TextureFiltering::BILINEAR);
  } else if(filtering == "Nearest") {
    m_current_texture->setFilteringMode(TextureSampling::TextureFiltering::NEAREST);
  } else {
    return;
  }
}

void TextureController::onWrappingModeChanged(const QString& mode) {
  if(m_current_texture == nullptr) {
    return;
  }
  if(mode == "Repeat") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::REPEAT);
  } else if(mode == "Mirrored Repeat") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::MIRRORED_REPEAT);
  } else if(mode == "Clamp to edge") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_EDGE);
  } else if(mode == "Clamp to border") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  } else {
    return;
  }
}

void TextureController::onBorderColorChanged(const QColor& color) {
  if(m_current_texture == nullptr) {
    return;
  }

  const ColorRGBA border_color = {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
  m_current_texture->setBorderColor(border_color);
}

void TextureController::updateViewFromModel() {
  if(m_current_texture == nullptr) {
    return;
  }

  const ImageProperties properties = m_current_texture->getProperties();
  std::string           texture_type;
  switch(m_current_texture->getTextureType()) {
  case TextureType::IMAGE_TEXTURE:
    texture_type = "Image Texture";
    break;
  case TextureType::COLOR_TEXTURE:
    texture_type = "Color Texture";
    break;
  default:
    texture_type = "Image Texture";
  }
  m_view->setTextureType(QString::fromStdString(texture_type));

  std::string texture_format;
  switch(properties.channels) {
  case 1:
    texture_format = "Grayscale";
    break;
  case 3:
    texture_format = "RGB";
    break;
  case 4:
    texture_format = "RGBA";
    break;
  default:
    texture_format = "RGB";
  }
  m_view->setTextureFormat(QString::fromStdString(texture_format));

  m_view->setTextureColorButton(QColor::fromRgbF(static_cast<float>(m_current_texture->getValue3d({0, 0}).r),
                                                 static_cast<float>(m_current_texture->getValue3d({0, 0}).g),
                                                 static_cast<float>(m_current_texture->getValue3d({0, 0}).b)));
  m_view->setTexturePath(QString::fromStdString(m_current_texture->getTexturePath()));

  m_view->setFlipVertically(m_current_texture->isFlippedVertically());

  const std::string color_space = m_current_texture->getColorSpace() == ColorSpace::Linear ? "Linear" : "sRGB";
  m_view->setColorSpace(QString::fromStdString(color_space));

  std::string texture_filtering;
  switch(m_current_texture->getFilteringMode()) {
  case TextureSampling::TextureFiltering::BILINEAR:
    texture_filtering = "Bilinear";
    break;
  case TextureSampling::TextureFiltering::NEAREST:
    texture_filtering = "Nearest";
    break;
  default:
    texture_filtering = "Nearest";
  }
  m_view->setTextureFiltering(QString::fromStdString(texture_filtering));

  std::string texture_wrapping;
  switch(m_current_texture->getWrappingMode()) {
  case TextureSampling::TextureWrapping::REPEAT:
    texture_wrapping = "Repeat";
    break;
  case TextureSampling::TextureWrapping::MIRRORED_REPEAT:
    texture_wrapping = "Mirrored repeat";
    break;
  case TextureSampling::TextureWrapping::CLAMP_TO_EDGE:
    texture_wrapping = "Clamp to edge";
    break;
  case TextureSampling::TextureWrapping::CLAMP_TO_BORDER:
    texture_wrapping = "Clamp to border";
    break;
  default:
    texture_wrapping = "Repeat";
  }
  m_view->setTextureWrappingMode(QString::fromStdString(texture_wrapping));

  const QColor border_color = QColor::fromRgbF(static_cast<float>(m_current_texture->getBorderColor().r),
                                               static_cast<float>(m_current_texture->getBorderColor().g),
                                               static_cast<float>(m_current_texture->getBorderColor().b),
                                               static_cast<float>(m_current_texture->getBorderColor().a));
  m_view->setTextureBorderColor(border_color);

  emit textureImageChanged(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
}
