/**
 * @file Framebuffer.hpp
 * @brief Header file for the Framebuffer class.
 */
#ifndef CORE_FRAMEBUFFER_HPP
#define CORE_FRAMEBUFFER_HPP

#include <cstddef>
#include <vector>

#include "Core/CommonTypes.hpp"

/**
 * @class Framebuffer
 * @brief A class representing a framebuffer for storing image data.
 *
 * This class provides methods for creating, managing, and manipulating a framebuffer
 * with support for setting pixel colors and generating image output.
 */
class Framebuffer {
private:
  double*                          m_framebuffer = nullptr;
  std::vector<std::vector<double>> m_thread_buffers;

  ImageProperties m_framebuffer_properties;

  static thread_local int m_thread_id;

public:
  /**
   * @brief Constructor for the Framebuffer class.
   * @param framebuffer_properties The properties of the image (width, height, and channel count).
   */
  explicit Framebuffer(ImageProperties framebuffer_properties);

  Framebuffer(const Framebuffer&)            = delete;
  Framebuffer& operator=(const Framebuffer&) = delete;
  Framebuffer(Framebuffer&&)                 = delete;
  Framebuffer& operator=(Framebuffer&&)      = delete;

  /**
   * @brief Gets the width of the framebuffer.
   * @return The width of the framebuffer.
   */
  int getWidth() const { return m_framebuffer_properties.width; }

  /**
   * @brief Gets the height of the framebuffer.
   * @return The height of the framebuffer.
   */
  int getHeight() const { return m_framebuffer_properties.height; }

  /**
   * @brief Gets the number of channels per pixel.
   * @return The number of channels per pixel.
   */
  int getChannelCount() const { return m_framebuffer_properties.channels; }

  /**
   * @brief Gets the size of the framebuffer.
   * @return The size of the framebuffer.
   */
  size_t getSize() const { return m_framebuffer_properties.bufferSize(); }

  /**
   * @brief Sets the properties of the framebuffer (width, height, and channel count).
   * @param framebuffer_properties The properties of the image to be set.
   * @note This method will clear the framebuffer and reallocate memory if the properties change.
   */
  void setFramebufferProperties(ImageProperties framebuffer_properties);

  /**
   * @brief Converts the framebuffer data to sRGB color space.
   */
  void convertToSRGBColorSpace();

  /**
   * @brief Gets the framebuffer data.
   * @return A pointer to the framebuffer data.
   */
  const double* getFramebuffer() const { return m_framebuffer; };

  /**
   * @brief Changes the framebuffer properties.
   */
  void updateFrameBuffer();

  /**
   * @brief Initializes thread buffers for multi-threaded rendering.
   * @param num_threads The number of threads to initialize buffers for.
   */
  void initThreadBuffers(unsigned int num_threads);

  /**
   * @brief Reduces the thread buffers into the main framebuffer.
   * This method aggregates the pixel data from all thread buffers into the main framebuffer.
   */
  void reduceThreadBuffers();

  /**
   * @brief Scales the values in the framebuffer by a given factor.
   * @param factor The factor by which to scale the framebuffer values.
   */
  void scaleBufferValues(double factor);

  /**
   * @brief Clears the thread buffers.
   */
  void clearThreadBuffers();

  /**
   * @brief Sets the color of a specific pixel in the framebuffer.
   * @param pixel_coord The coordinates of the pixel to set.
   * @param color The color to set the pixel to.
   * @param weight The weight of the color to apply.
   */
  void setPixelColor(const PixelCoord& pixel_coord, const ColorRGBA& color, double weight);

  /**
   * @brief Sets the thread ID for the current thread.
   * @param thread_id The ID of the thread.
   */
  static void setThreadId(int thread_id) { m_thread_id = thread_id; }

  ~Framebuffer(); ///< Destructor to clean up the framebuffer resources.
};

#endif // CORE_FRAMEBUFFER_HPP