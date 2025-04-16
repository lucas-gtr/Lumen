/**
 * @file RenderSettings.hpp
 * @brief Header file for the RenderSettings class.
 */
#ifndef RENDERING_RENDERSETTINGS_HPP
#define RENDERING_RENDERSETTINGS_HPP

#include "Core/Config.hpp"
#include <algorithm>
#include <cstddef>
#include <limits>

/**
 * @class RenderSettings
 * @brief Class to manage the settings for rendering, including resolution, clipping planes, and other related
 * parameters.
 *
 * This class provides getter and setter methods for each rendering setting, with bounds checking
 * to ensure valid values are used. The class supports image resolution, number of color channels,
 * near and far clipping planes, maximum bounces for ray tracing, samples per pixel, and background color.
 */
class RenderSettings {
private:
  size_t m_width  = DEFAULT_WIDTH;  // in pixels
  size_t m_height = DEFAULT_HEIGHT; // in pixels

  size_t m_channel_count = DEFAULT_CHANNEL_COUNT;

  double m_near_plane = DEFAULT_NEAR_PLANE; // in meters
  double m_far_plane  = DEFAULT_FAR_PLANE;  // in meters

  int m_max_bounces        = DEFAULT_MAX_BOUNCES;
  int m_samples_per_pixels = DEFAULT_SAMPLES_PER_PIXEL;

public:
  RenderSettings()                                 = default; ///< Default constructor.
  RenderSettings(const RenderSettings&)            = default; ///< Copy constructor.
  RenderSettings& operator=(const RenderSettings&) = default; ///< Copy assignment operator.
  RenderSettings(RenderSettings&&)                 = default; ///< Move constructor.
  RenderSettings& operator=(RenderSettings&&)      = default; ///< Move assignment operator.

  /**
   * @brief Get the width of the rendered image.
   * @return The width of the image in pixels.
   */
  size_t getWidth() const { return m_width; }

  /**
   * @brief Set the width of the rendered image.
   * The width will be clamped to a valid range between MIN_WIDTH and MAX_WIDTH.
   * @param width The width of the image in pixels.
   */
  void setWidth(size_t width) { m_width = std::clamp(width, MIN_WIDTH, MAX_WIDTH); }

  /**
   * @brief Get the height of the rendered image.
   * @return The height of the image in pixels.
   */
  size_t getHeight() const { return m_height; }

  /**
   * @brief Set the height of the rendered image.
   * The height will be clamped to a valid range between MIN_HEIGHT and MAX_HEIGHT.
   * @param height The height of the image in pixels.
   */
  void setHeight(size_t height) { m_height = std::clamp(height, MIN_HEIGHT, MAX_HEIGHT); }

  /**
   * @brief Get the number of color channels.
   * @return The number of color channels (e.g., 3 for RGB, 4 for RGBA).
   */
  size_t getChannelCount() const { return m_channel_count; }

  /**
   * @brief Set the number of color channels.
   * The channel count will be clamped to a valid range between MIN_CHANNEL_COUNT and MAX_CHANNEL_COUNT.
   * @param channel_count The number of color channels.
   */
  void setChannelCount(size_t channel_count) {
    m_channel_count = std::clamp(channel_count, MIN_CHANNEL_COUNT, MAX_CHANNEL_COUNT);
  }

  /**
   * @brief Get the near clipping plane value.
   * @return The near clipping plane value.
   */
  double getNearPlane() const { return m_near_plane; }

  /**
   * @brief Set the near clipping plane value.
   * The near plane will be clamped to a valid range between MIN_NEAR_PLANE and MAX_NEAR_PLANE.
   * @param near_plane The near clipping plane value.
   */
  void setNearPlane(double near_plane) {
    near_plane   = std::clamp(near_plane, MIN_NEAR_PLANE, MAX_NEAR_PLANE);
    m_near_plane = std::min(near_plane, m_far_plane - std::numeric_limits<double>::epsilon());
  }

  /**
   * @brief Get the far clipping plane value.
   * @return The far clipping plane value.
   */
  double getFarPlane() const { return m_far_plane; }

  /**
   * @brief Set the far clipping plane value.
   * The far plane will be clamped to a valid range between MIN_FAR_PLANE and MAX_FAR_PLANE.
   * @param far_plane The far clipping plane value.
   */
  void setFarPlane(double far_plane) {
    far_plane   = std::clamp(far_plane, MIN_FAR_PLANE, MAX_FAR_PLANE);
    m_far_plane = std::max(far_plane, m_near_plane + std::numeric_limits<double>::epsilon());
  }

  /**
   * @brief Get the maximum number of bounces for ray tracing.
   * @return The maximum number of bounces.
   */
  int getMaxBounces() const { return m_max_bounces; }

  /**
   * @brief Set the maximum number of bounces for ray tracing.
   * The number of bounces will be clamped to a valid range between MIN_MAX_BOUNCES and MAX_MAX_BOUNCES.
   * @param max_bounces The maximum number of bounces.
   */
  void setMaxBounces(int max_bounces) { m_max_bounces = std::clamp(max_bounces, MIN_MAX_BOUNCES, MAX_MAX_BOUNCES); }

  /**
   * @brief Get the number of samples per pixel for anti-aliasing.
   * @return The number of samples per pixel.
   */
  int getSamplesPerPixel() const { return m_samples_per_pixels; }

  /**
   * @brief Set the number of samples per pixel for anti-aliasing.
   * The number of samples per pixel will be clamped to a valid range between MIN_SAMPLES_PER_PIXEL and
   * MAX_SAMPLES_PER_PIXEL.
   * @param samples_per_pixels The number of samples per pixel.
   */
  void setSamplesPerPixel(int samples_per_pixels) {
    m_samples_per_pixels = std::clamp(samples_per_pixels, MIN_SAMPLES_PER_PIXEL, MAX_SAMPLES_PER_PIXEL);
  }

  ~RenderSettings() = default; ///< Default destructor.
};

#endif // RENDERING_RENDERSETTINGS_HPP