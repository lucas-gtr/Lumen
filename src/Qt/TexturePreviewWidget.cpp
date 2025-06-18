#include <cstring>
#include <qboxlayout.h>
#include <qcolor.h>
#include <qframe.h>
#include <qglobal.h>
#include <qimage.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qsize.h>
#include <qwidget.h>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Qt/TexturePreviewWidget.hpp"

TexturePreviewWidget::TexturePreviewWidget(int max_width, int max_height, QWidget* parent)
    : QWidget(parent), m_max_width(max_width), m_max_height(max_height) {
  setupUI();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void TexturePreviewWidget::setupUI() {
  m_preview_image_label = new QLabel(this);
  m_preview_image_label->setAlignment(Qt::AlignCenter);
  m_preview_image_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_preview_image_label->setAttribute(Qt::WA_OpaquePaintEvent);
  m_preview_image_label->setFrameStyle(QFrame::Box);

  auto* layout = new QVBoxLayout;
  layout->addWidget(m_preview_image_label);

  setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void TexturePreviewWidget::setTexturePreview(const unsigned char*   preview_data,
                                             const ImageProperties& preview_properties) {
  m_preview_image_label->clear();
  m_preview_image_label->setPixmap(QPixmap());

  QPixmap     pixmap;
  const QSize max_size(m_max_width, m_max_height);

  if(preview_properties.width == 1 && preview_properties.height == 1) {
    QColor color;
    switch(preview_properties.channels) {
    case 1:
      color = QColor(preview_data[0], preview_data[0], preview_data[0]);
      break;
    case 3:
    case 4:
      color = QColor(preview_data[0], preview_data[1], preview_data[2]);
      break;
    default:
      color = QColor(COLOR8_MAX_VALUE, 0, 0, COLOR8_MAX_VALUE);
      break;
    }

    QImage solid_image(max_size, QImage::Format_RGBA8888);
    solid_image.fill(color);
    pixmap = QPixmap::fromImage(solid_image);
  } else {
    QImage::Format format = QImage::Format_Invalid;
    switch(preview_properties.channels) {
    case 1:
      format = QImage::Format_Grayscale8;
      break;
    case 3:
      format = QImage::Format_RGB888;
      break;
    case 4:
      format = QImage::Format_RGBA8888;
      break;
    default:
      break;
    }

    QImage image(preview_properties.width, preview_properties.height, format);
    image.fill(Qt::black);
    std::memcpy(image.bits(), preview_data, image.sizeInBytes());
    uchar*    bits        = image.bits();
    const int pixel_count = preview_properties.width * preview_properties.height;
    if(format == QImage::Format_RGBA8888) {
      for(int i = 0; i < pixel_count; ++i) {
        bits[4 * i + 3] = COLOR8_MAX_VALUE;
      }
    }
    pixmap = QPixmap::fromImage(image).scaled(max_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  m_preview_image_label->setFixedSize(pixmap.size());
  m_preview_image_label->setPixmap(pixmap);
}