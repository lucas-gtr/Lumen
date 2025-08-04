/**
 * @file TexturePreview.hpp
 * @brief Header file for the TexturePreview class, which provides a preview of textures.
 */
#ifndef GUI_WIDGETS_TEXTUREPREVIEW_HPP
#define GUI_WIDGETS_TEXTUREPREVIEW_HPP

#include <QColor>
#include <QImage>
#include <QLabel>

#include "Core/ImageTypes.hpp"

/**
 * @class TexturePreview
 * @brief A widget that displays a preview of a texture.
 *
 * This class inherits from QLabel and provides functionality to set an image preview
 * based on the provided image data and properties. It can handle both color images and
 * solid colors, and it can create a QImage from the raw image data.
 */
class TexturePreview : public QLabel {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the TexturePreview class.
   * @param parent The parent widget for this texture preview.
   */
  explicit TexturePreview(QWidget* parent = nullptr);

  /**
   * @brief Sets the texture preview image.
   * @param image_data The raw image data to set as the texture preview.
   * @param properties The properties of the image, such as width, height, and channels.
   */
  void setImage(const unsigned char* image_data, ImageProperties properties);

private:
  static bool           isColorImage(const ImageProperties& properties);
  static QColor         getSolidColor(const unsigned char* image_data, int channels);
  static QImage::Format getImageFormat(int channels);

  static QImage createSolidImage(const QColor& color, const QSize& size);
  static QImage createImageFromData(const unsigned char* image_data, const ImageProperties& properties);
};

#endif // GUI_WIDGETS_TEXTUREPREVIEW_HPP