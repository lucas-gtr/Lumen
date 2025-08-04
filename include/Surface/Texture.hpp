/**
 * @file   Texture.hpp
 * @brief  Header file for the Texture class.
 */
#ifndef SURFACE_TEXTURE_HPP
#define SURFACE_TEXTURE_HPP

#include <cstdint>
#include <vector>

#include "Core/Color.hpp"
#include "Core/ImageTypes.hpp"
#include "Core/Observer.hpp"
#include "Surface/TextureFiltering.hpp"
#include "Surface/TextureWrapping.hpp"

/**
 * @enum ColorSpace
 * @brief Enumeration for color space modes.
 *
 * This enum defines the color space modes for textures, including RGB and sRGB.
 * The RGB mode is used for standard RGB color representation, while sRGB is used for
 * standard RGB color representation with gamma correction.
 */
enum class ColorSpace : std::uint8_t { LINEAR, S_RGB };

/**
 * @enum TextureType
 * @brief Enumeration for texture types.
 *
 * This enum defines the types of textures, including image textures and color textures.
 * Image textures are used for loading images, while color textures are used for solid colors.
 */
enum class TextureType : std::uint8_t { IMAGE_TEXTURE, COLOR_TEXTURE };

/**
 * @class Texture
 * @brief Class representing a texture for 3D rendering.
 *
 * This class encapsulates the properties of a texture, including its image data,
 * dimensions, color space, filtering mode, and wrapping mode. It provides methods
 * for setting and getting texture values, as well as sampling the texture using
 * different filtering techniques.
 */
class Texture {
private:
  std::vector<double>        m_image_data;
  std::vector<unsigned char> m_texture_preview;

  TextureType m_texture_type = TextureType::IMAGE_TEXTURE;

  ImageProperties m_texture_properties;
  ImageProperties m_preview_properties;

  std::string m_texture_path;
  bool        m_flipped_vertically = false;

  ColorSpace                        m_color_space    = ColorSpace::S_RGB;
  TextureSampling::TextureFiltering m_filtering_mode = TextureSampling::TextureFiltering::BILINEAR;
  TextureSampling::TextureWrapping  m_wrapping_mode  = TextureSampling::TextureWrapping::MIRRORED_REPEAT;

  ColorRGB m_border_color;

  Observer<> m_texture_data_observer;
  Observer<> m_texture_parameters_observer;

  std::pair<int, int> computePreviewSize() const;
  void                appendPixelFromSource(int src_x, int src_y, int channels, int original_width);
  void                generateRescaledImage(int new_width, int new_height);

  void flipVertically();

  void generateTexture(const ImageProperties& properties, const std::vector<double>& image_data);
  void finalizeTexture();

public:
  /**
   * @brief Default constructor for the Texture class.
   */
  Texture();

  Texture(const Texture& other)                = delete;
  Texture(Texture&& other) noexcept            = delete;
  Texture& operator=(const Texture& other)     = delete;
  Texture& operator=(Texture&& other) noexcept = delete;

  /**
   * @brief Gets the observer that notifies when texture data changes.
   * @return A reference to the observer that notifies about texture data changes.
   */
  Observer<>& getTextureDataObserver() { return m_texture_data_observer; }

  /**
   * @brief Gets the observer that notifies when texture parameters change.
   * @return A reference to the observer that notifies about texture parameter changes.
   */
  Observer<>& getTextureParametersObserver() { return m_texture_parameters_observer; }

  /**
   * @brief Sets the texture type.
   * @param type The new texture type to set.
   */
  void setTextureType(TextureType type);

  /**
   * @brief Gets the texture type.
   * @return The current texture type.
   */
  const TextureType& getTextureType() const { return m_texture_type; }

  /**
   * @brief Generates preview data for the texture.
   */
  void generatePreviewData();

  /**
   * @brief Loads texture data from a file.
   * @param filename The path to the texture file to load.
   */
  void loadFromFile(const char* filename);

  /**
   * @brief Gets the path of the texture file.
   * @return The path of the texture file.
   */
  const std::string& getTexturePath() const { return m_texture_path; }

  /**
   * @brief Sets the value of the texture.
   * @param value The new value for the texture.
   */
  void setValue(double value);

  /**
   * @brief Sets the value of the texture with color.
   * @param color The new RGB color for the texture.
   */
  void setValue(const ColorRGB& color);

  /**
   * @brief Sets the value of the texture with RGBA color.
   * @param color The new RGBA color for the texture.
   */
  void setValue(const ColorRGBA& color);

  /**
   * @brief Sets the color space of the texture.
   * @param color_space The new color space for the texture.
   */
  void setColorSpace(ColorSpace color_space);

  /**
   * @brief Gets the color space of the texture.
   * @return The color space of the texture.
   */
  const ColorSpace& getColorSpace() const { return m_color_space; }

  /**
   * @brief Sets the filtering mode of the texture.
   * @param filtering_mode The new filtering mode for the texture.
   */
  void setFilteringMode(TextureSampling::TextureFiltering filtering_mode);

  /**
   * @brief Gets the filtering mode of the texture.
   * @return The filtering mode of the texture.
   */
  const TextureSampling::TextureFiltering& getFilteringMode() const { return m_filtering_mode; }

  /**
   * @brief Sets the wrapping mode of the texture.
   * @param wrapping_mode The new wrapping mode for the texture.
   */
  void setWrappingMode(TextureSampling::TextureWrapping wrapping_mode);

  /**
   * @brief Gets the wrapping mode of the texture.
   * @return The wrapping mode of the texture.
   */
  const TextureSampling::TextureWrapping& getWrappingMode() const { return m_wrapping_mode; }

  /**
   * @brief Sets the border color (RGBA) of the texture.
   * @param color The new border color for the texture.
   */
  void setBorderColor(const ColorRGBA& color);

  /**
   * @brief Sets the border color (RGB) of the texture.
   * @param color The new border color for the texture.
   */
  void setBorderColor(const ColorRGB& color);

  /**
   * @brief Sets the border color (grayscale) of the texture.
   * @param value The new grayscale value for the border color.
   */
  void setBorderColor(double value);

  /**
   * @brief Gets the border color of the texture.
   * @return The border color of the texture.
   */
  const ColorRGB& getBorderColor() const { return m_border_color; }

  /**
   * @brief Sets whether the texture is flipped vertically.
   * @param flipped True if the texture should be flipped vertically, false otherwise.
   */
  void setFlippedVertically(bool flipped);

  /**
   * @brief Checks if the texture is flipped vertically.
   * @return True if the texture is flipped vertically, false otherwise.
   */
  bool isFlippedVertically() const { return m_flipped_vertically; }

  /**
   * @brief Gets the properties of the texture.
   * @return The properties of the texture.
   */
  const ImageProperties& getProperties() const { return m_texture_properties; }

  /**
   * @brief Gets the properties of the texture preview.
   * @return The properties of the texture preview.
   */
  const ImageProperties& getPreviewProperties() const { return m_preview_properties; }

  /**
   * @brief Gets the value of the texture.
   * @param uv_coord The texture coordinates (u, v).
   *
   * @return The value of the texture.
   */
  double getValue1d(TextureUV uv_coord) const;

  /**
   * @brief Gets the color of the texture.
   * @param uv_coord The texture coordinates (u, v).
   *
   * @return The color RGB of the texture.
   */
  ColorRGB getValue3d(TextureUV uv_coord) const;

  /**
   * @brief Gets the color of the texture with alpha.
   * @param uv_coord The texture coordinates (u, v).
   *
   * @return The color RGBA of the texture.
   */
  ColorRGBA getValue4d(TextureUV uv_coord) const;

  /**
   * @brief Gets the image data of the texture.
   * @return A pointer to the image data of the texture.
   */
  const double* getImageData() const { return m_image_data.data(); }

  /**
   * @brief Gets the preview data of the texture.
   * @return A pointer to the preview data of the texture.
   */
  const unsigned char* getPreviewData() const { return m_texture_preview.data(); }

  ~Texture() = default; ///< Default destructor for the Texture class.
};

#endif // SURFACE_TEXTURE_HPP