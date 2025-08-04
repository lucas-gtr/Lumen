// GCOVR_EXCL_START
#include "TexturePreview.hpp"
#include "Core/Color.hpp"

TexturePreview::TexturePreview(QWidget* parent) : QLabel(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setAttribute(Qt::WA_OpaquePaintEvent);
}

void TexturePreview::setImage(const unsigned char* image_data, ImageProperties preview_properties) {
  const QSize max_size(maximumSize().width(), maximumSize().height());
  QPixmap     pixmap;

  if(isColorImage(preview_properties)) {
    const QColor color       = getSolidColor(image_data, preview_properties.channels);
    const QImage solid_image = createSolidImage(color, max_size);
    pixmap                   = QPixmap::fromImage(solid_image);
  } else {
    const QImage image = createImageFromData(image_data, preview_properties);
    pixmap             = QPixmap::fromImage(image).scaled(max_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  setFixedSize(pixmap.size());
  setPixmap(pixmap);
}

bool TexturePreview::isColorImage(const ImageProperties& properties) {
  return properties.width == 1 && properties.height == 1;
}

QColor TexturePreview::getSolidColor(const unsigned char* data, int channels) {
  switch(channels) {
  case 1:
    return {data[0], data[0], data[0]};
  case 3:
  case 4:
    return {data[0], data[1], data[2]};
  default:
    return {COLOR8_MAX_VALUE, 0, COLOR8_MAX_VALUE, COLOR8_MAX_VALUE};
  }
}

QImage TexturePreview::createSolidImage(const QColor& color, const QSize& size) {
  QImage image(size, QImage::Format_RGBA8888);
  image.fill(color);
  return image;
}

QImage::Format TexturePreview::getImageFormat(int channels) {
  switch(channels) {
  case 1:
    return QImage::Format_Grayscale8;
  case 3:
    return QImage::Format_RGB888;
  case 4:
    return QImage::Format_RGBA8888;
  default:
    return QImage::Format_Invalid;
  }
}

QImage TexturePreview::createImageFromData(const unsigned char* data, const ImageProperties& properties) {
  const QImage::Format format = getImageFormat(properties.channels);
  QImage               image(properties.width, properties.height, format);
  image.fill(Qt::black);

  std::memcpy(image.bits(), data, image.sizeInBytes());

  if(format == QImage::Format_RGBA8888) {
    uchar*    bits        = image.bits();
    const int pixel_count = properties.width * properties.height;
    for(int i = 0; i < pixel_count; ++i) {
      bits[4 * i + 3] = COLOR8_MAX_VALUE;
    }
  }

  return image;
}
// GCOVR_EXCL_STOP