/**
 * @file   Texture.hpp
 * @brief  Header file for the Texture class.
 */
#ifndef SURFACE_TEXTURE_HPP
#define SURFACE_TEXTURE_HPP

#include <cstdint>

#include "Core/CommonTypes.hpp"
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
enum class ColorSpace : std::uint8_t { RGB, sRGB };

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
  double* m_image_data;

  ImageProperties m_texture_properties;

  ColorSpace                        m_color_space    = ColorSpace::sRGB;
  TextureSampling::TextureFiltering m_filtering_mode = TextureSampling::TextureFiltering::BILINEAR;
  TextureSampling::TextureWrapping  m_wrapping_mode  = TextureSampling::TextureWrapping::MIRRORED_REPEAT;

  ColorRGBA m_border_color;

public:
  /**
   * @brief Default constructor for the Texture class.
   * @param value The initial value for the texture.
   */
  explicit Texture(double value);

  /**
   * @brief Constructor for the Texture class with color.
   * @param color The initial color for the texture.
   */
  explicit Texture(const ColorRGB& color);

  /**
   * @brief Constructor for the Texture class with RGBA color.
   * @param color The initial RGBA color for the texture.
   */
  explicit Texture(const ColorRGBA& color);

  /**
   * @brief Constructor for the Texture class with image data (as unsigned char).
   * @param image_data Pointer to the image data.
   * @param texture_properties The properties of the image (width, height, and channel count).
   */
  Texture(const unsigned char* image_data, ImageProperties texture_properties);

  /**
   * @brief Constructor for the Texture class with image data (as double).
   * @param image_data Pointer to the image data.
   * @param texture_properties The properties of the image (width, height, and channel count).
   */
  Texture(const double* image_data, ImageProperties texture_properties);

  Texture(const Texture& other);                ///< Copy constructor.
  Texture(Texture&& other) noexcept;            ///< Move constructor.
  Texture& operator=(const Texture& other);     ///< Copy assignment operator.
  Texture& operator=(Texture&& other) noexcept; ///< Move assignment operator.

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
   * @param colorSpace The new color space for the texture.
   */
  void setColorSpace(ColorSpace colorSpace);

  /**
   * @brief Sets the filtering mode of the texture.
   * @param filteringMode The new filtering mode for the texture.
   */
  void setFilteringMode(TextureSampling::TextureFiltering filteringMode) { m_filtering_mode = filteringMode; }

  TextureSampling::TextureFiltering getFilteringMode() const { return m_filtering_mode; }

  /**
   * @brief Sets the wrapping mode of the texture.
   * @param wrappingMode The new wrapping mode for the texture.
   */
  void setWrappingMode(TextureSampling::TextureWrapping wrappingMode) { m_wrapping_mode = wrappingMode; }

  TextureSampling::TextureWrapping getWrappingMode() const { return m_wrapping_mode; }

  /**
   * @brief Sets the border color (RGBA) of the texture.
   * @param color The new border color for the texture.
   */
  void setBorderColor(const ColorRGBA& color) { m_border_color = color; }

  /**
   * @brief Sets the border color (RGB) of the texture.
   * @param color The new border color for the texture.
   */
  void setBorderColor(const ColorRGB& color) { m_border_color = ColorRGBA(color, 1.0); }

  /**
   * @brief Sets the border color (grayscale) of the texture.
   * @param value The new grayscale value for the border color.
   */
  void setBorderColor(double value) { m_border_color = {value, value, value, 1.0}; }

  ColorRGBA getBorderColor() const { return m_border_color; }

  /**
   * @brief Sets the image data of the texture.
   * @param image_data Pointer to the new image data.
   * @param texture_properties The properties of the image (width, height, and channel count).
   */
  void setImageData(const double* image_data, ImageProperties texture_properties);

  /**
   * @brief Flips the texture vertically.
   */
  void flipVertically();

  /**
   * @brief Gets the properties of the texture.
   * @return The properties of the texture.
   */
  const ImageProperties& getProperties() const { return m_texture_properties; }

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

  double* getImageData() const { return m_image_data; }

  ~Texture(); ///< Destructor.
};

#endif // SURFACE_TEXTURE_HPP