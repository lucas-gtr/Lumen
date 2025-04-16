/**
 * @file Framebuffer.hpp
 * @brief Header file for the Framebuffer class.
 */
#ifndef CORE_FRAMEBUFFER_HPP
#define CORE_FRAMEBUFFER_HPP

#include <Eigen/Core>
#include <cstddef>
#include <vector>

/**
 * @struct ImageProperties
 * @brief A structure representing the properties of an image.
 *
 * This structure contains the width, height, and number of channels of an image.
 */
struct ImageProperties {
  size_t width;
  size_t height;
  size_t channel_count;
};

/**
 * @class Framebuffer
 * @brief A class representing a framebuffer for storing image data.
 *
 * This class provides methods for creating, managing, and manipulating a framebuffer
 * with support for setting pixel colors and generating image output.
 */
class Framebuffer {
private:
  std::vector<double>        m_framebuffer;
  std::vector<unsigned char> m_image;

  size_t m_width;
  size_t m_height;
  size_t m_channel_count;

public:
  /**
   * @brief Constructor for the Framebuffer class.
   * @param properties The properties of the image (width, height, and channel count).
   */
  explicit Framebuffer(ImageProperties properties);
  Framebuffer(const Framebuffer&)            = delete; ///< Deleted copy constructor.
  Framebuffer& operator=(const Framebuffer&) = delete; ///< Deleted copy assignment operator.
  Framebuffer(Framebuffer&&)                 = delete; ///< Deleted move constructor.
  Framebuffer& operator=(Framebuffer&&)      = delete; ///< Deleted move assignment operator.

  /**
   * @brief Gets the width of the framebuffer.
   * @return The width of the framebuffer.
   */
  size_t getWidth() const { return m_width; }

  /**
   * @brief Gets the height of the framebuffer.
   * @return The height of the framebuffer.
   */
  size_t getHeight() const { return m_height; }

  /**
   * @brief Gets the number of channels per pixel.
   * @return The number of channels per pixel.
   */
  size_t getChannelCount() const { return m_channel_count; }

  /**
   * @brief Sets the properties of the framebuffer (width, height, and channel count).
   * @param properties The properties of the image to be set.
   * @note This method will clear the framebuffer and reallocate memory if the properties change.
   */
  void setFramebufferProperties(ImageProperties properties);

  /**
   * @brief Generates the image from the framebuffer data.
   */
  void generateImage();

  /**
   * @brief Gets the image data as a constant pointer.
   * @return A constant pointer to the image data.
   */
  const unsigned char* getImage() const { return m_image.data(); };

  /**
   * @brief Changes the framebuffer properties.
   */
  void updateFrameBuffer();

  /**
   * @brief Sets the color of a specific pixel in the framebuffer.
   * @param x The x-coordinate of the pixel.
   * @param y The y-coordinate of the pixel.
   * @param color The color to set the pixel to.
   * @param weight The weight of the color to apply.
   */
  void setPixelColor(size_t x, size_t y, const Eigen::Vector3d& color, double weight);

  ~Framebuffer() = default; /// Default destructor.
};

#endif // CORE_FRAMEBUFFER_HPP