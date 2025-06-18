/**
 * @file TexturePreviewWidget.hpp
 * @brief Header file for the TexturePreviewWidget class, which displays a texture preview.
 */
#ifndef QT_TEXTUREPREVIEWWIDGET_HPP
#define QT_TEXTUREPREVIEWWIDGET_HPP

#include <qlabel.h>

#include "Core/CommonTypes.hpp"

/**
 * @class TexturePreviewWidget
 * @brief A widget that displays a texture preview image.
 */
class TexturePreviewWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for TexturePreviewWidget.
   * @param max_width Maximum width of the preview image.
   * @param max_height Maximum height of the preview image.
   * @param parent Parent widget.
   */
  explicit TexturePreviewWidget(int max_width, int max_height, QWidget* parent);

  /**
   * @brief Sets the texture preview image.
   * @param preview_data Pointer to the image data (unsigned char array).
   * @param preview_properties Properties of the image, including width, height, and channels.
   */
  void setTexturePreview(const unsigned char* preview_data, const ImageProperties& preview_properties);

private:
  QLabel* m_preview_image_label = nullptr;

  int m_max_width;
  int m_max_height;

  void setupUI();
};

#endif // QT_TEXTUREPREVIEWWIDGET_HPP