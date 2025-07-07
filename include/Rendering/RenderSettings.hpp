/**
 * @file RenderSettings.hpp
 * @brief Header file for the RenderSettings class.
 */
#ifndef RENDERING_RENDERSETTINGS_HPP
#define RENDERING_RENDERSETTINGS_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"

enum class RenderMode : std::uint8_t { SINGLE_THREADED, MULTI_THREADED_CPU, GPU_CUDA };

/**
 * @class RenderSettings
 * @brief Class to manage the settings for rendering, including resolution, clipping planes, and other related
 * parameters.
 *
 * This class provides getter and setter methods for each rendering setting, with bounds checking
 * to ensure valid values are used. The class supports image resolution, number of color channels,
 * maximum bounces for ray tracing, samples per pixel, and background color.
 */
class RenderSettings {
private:
  ImageProperties m_properties = {DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_CHANNEL_COUNT};

  int m_max_bounces        = DEFAULT_MAX_BOUNCES;
  int m_samples_per_pixels = DEFAULT_SAMPLES_PER_PIXEL;

  RenderMode m_render_mode = RenderMode::SINGLE_THREADED;

  int          m_chunk_size   = DEFAULT_CHUNK_SIZE;
  unsigned int m_thread_count = std::max(1U, std::thread::hardware_concurrency() - 4);

  double m_dx = 1.0 / static_cast<double>(m_properties.width);
  double m_dy = 1.0 / static_cast<double>(m_properties.height);

public:
  RenderSettings() = default; ///< Default constructor initializes with default settings.

  RenderSettings(const RenderSettings&)            = delete;
  RenderSettings& operator=(const RenderSettings&) = delete;
  RenderSettings(RenderSettings&&)                 = delete;
  RenderSettings& operator=(RenderSettings&&)      = delete;

  ImageProperties getImageProperties() const { return m_properties; }

  /**
   * @brief Get the width of the rendered image.
   * @return The width of the image in pixels.
   */
  int getWidth() const { return m_properties.width; }

  /**
   * @brief Get the horizontal step size for pixel coordinates.
   * @return The horizontal step size (dx).
   */
  double getDx() const { return m_dx; }

  /**
   * @brief Set the width of the rendered image.
   * The width will be clamped to a valid range between MIN_WIDTH and MAX_IMAGE_WIDTH.
   * @param width The width of the image in pixels.
   */
  void setWidth(int width) {
    m_properties.width = std::clamp(width, MIN_WIDTH, MAX_IMAGE_WIDTH);
    m_dx               = 1.0 / static_cast<double>(width);
  }

  /**
   * @brief Get the height of the rendered image.
   * @return The height of the image in pixels.
   */
  int getHeight() const { return m_properties.height; }

  /**
   * @brief Get the vertical step size for pixel coordinates.
   * @return The vertical step size (dy).
   */
  double getDy() const { return m_dy; }

  /**
   * @brief Set the height of the rendered image.
   * The height will be clamped to a valid range between MIN_HEIGHT and MAX_IMAGE_HEIGHT.
   * @param height The height of the image in pixels.
   */
  void setHeight(int height) {
    m_properties.height = std::clamp(height, MIN_HEIGHT, MAX_IMAGE_HEIGHT);
    m_dy                = 1.0 / static_cast<double>(height);
  }

  /**
   * @brief Get the number of color channels.
   * @return The number of color channels (e.g., 3 for RGB, 4 for RGBA).
   */
  int getChannelCount() const { return m_properties.channels; }

  /**
   * @brief Set the number of color channels.
   * The channel count will be clamped to a valid range between MIN_CHANNEL_COUNT and MAX_CHANNEL_COUNT.
   * @param channels The number of color channels.
   */
  void setChannelCount(int channels) {
    m_properties.channels = std::clamp(channels, MIN_CHANNEL_COUNT, MAX_CHANNEL_COUNT);
  }

  /**
   * @brief Get the maximum number of bounces for ray tracing.
   * @return The maximum number of bounces.
   */
  int getMaxBounces() const { return m_max_bounces; }

  /**
   * @brief Set the maximum number of bounces for ray tracing.
   * The number of bounces will be clamped to a valid range between MIN_MAX_BOUNCES and MAX_BOUNCES.
   * @param max_bounces The maximum number of bounces.
   */
  void setMaxBounces(int max_bounces) { m_max_bounces = std::clamp(max_bounces, MIN_MAX_BOUNCES, MAX_BOUNCES); }

  /**
   * @brief Get the number of samples per pixel for anti-aliasing.
   * @return The number of samples per pixel.
   */
  int getSamplesPerPixel() const { return m_samples_per_pixels; }

  /**
   * @brief Set the number of samples per pixel for anti-aliasing.
   * The number of samples per pixel will be adjusted to the nearest perfect square within the valid range
   * between MIN_SAMPLES_PER_PIXEL and MAX_SAMPLES_PER_PIXEL.
   * @param samples_per_pixels The desired number of samples per pixel.
   */
  void setSamplesPerPixel(int samples_per_pixels);

  /**
   * @brief Get the mode for rendering.
   * @return The current render mode.
   */
  RenderMode getRenderMode() const { return m_render_mode; }

  /**
   * @brief Set the mode for rendering.
   * @param mode The desired render mode.
   */
  void setRenderMode(RenderMode mode) { m_render_mode = mode; }

  /**
   * @brief Set the chunk size for multi-threaded rendering.
   * @param chunk_size The size of the chunk to be used in multi-threaded rendering.
   */
  void setChunkSize(int chunk_size);

  /**
   * @brief Get the chunk size for multi-threaded rendering.
   * @return The size of the chunk used in multi-threaded rendering.
   */
  int getChunkSize() const { return m_chunk_size; }

  /**
   * @brief Set the number of threads to be used for multi-threaded rendering.
   * @param thread_count The number of threads to be used for rendering.
   */
  void setThreadCount(unsigned int thread_count);

  /**
   * @brief Get the number of threads used for multi-threaded rendering.
   * @return The number of threads available for rendering.
   */
  unsigned int getThreadCount() const { return m_thread_count; }

  ~RenderSettings() = default; ///< Default destructor.
};

#endif // RENDERING_RENDERSETTINGS_HPP