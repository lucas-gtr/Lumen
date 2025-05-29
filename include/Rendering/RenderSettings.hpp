/**
 * @file RenderSettings.hpp
 * @brief Header file for the RenderSettings class.
 */
#ifndef RENDERING_RENDERSETTINGS_HPP
#define RENDERING_RENDERSETTINGS_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "ImplementationParameters/Parameters.hpp"

enum class RenderExecutionMode : std::uint8_t { SINGLE_THREADED, MULTI_THREADED_CPU, GPU_CUDA };

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
  ImageProperties m_properties = {DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_CHANNEL_COUNT};

  double dx = 1.0 / static_cast<double>(m_properties.width);
  double dy = 1.0 / static_cast<double>(m_properties.height);

  double m_near_plane = DEFAULT_NEAR_PLANE; // in meters
  double m_far_plane  = DEFAULT_FAR_PLANE;  // in meters

  int m_max_bounces        = DEFAULT_MAX_BOUNCES;
  int m_samples_per_pixels = DEFAULT_SAMPLES_PER_PIXEL;

  RenderExecutionMode m_execution_mode = RenderExecutionMode::SINGLE_THREADED;

  std::unique_ptr<Parameters> m_renderer_parameters;

public:
  RenderSettings();
  RenderSettings(const RenderSettings&)            = delete;
  RenderSettings& operator=(const RenderSettings&) = delete;
  RenderSettings(RenderSettings&&)                 = delete;
  RenderSettings& operator=(RenderSettings&&)      = delete;

  /**
   * @brief Get the width of the rendered image.
   * @return The width of the image in pixels.
   */
  int getWidth() const { return m_properties.width; }

  /**
   * @brief Get the horizontal step size for pixel coordinates.
   * @return The horizontal step size (dx).
   */
  double getDx() const { return dx; }

  /**
   * @brief Set the width of the rendered image.
   * The width will be clamped to a valid range between MIN_WIDTH and MAX_WIDTH.
   * @param width The width of the image in pixels.
   */
  void setWidth(int width) {
    m_properties.width = std::clamp(width, MIN_WIDTH, MAX_WIDTH);
    dx                 = 1.0 / static_cast<double>(width);
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
  double getDy() const { return dy; }

  /**
   * @brief Set the height of the rendered image.
   * The height will be clamped to a valid range between MIN_HEIGHT and MAX_HEIGHT.
   * @param height The height of the image in pixels.
   */
  void setHeight(int height) {
    m_properties.height = std::clamp(height, MIN_HEIGHT, MAX_HEIGHT);
    dy                  = 1.0 / static_cast<double>(height);
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
   * The number of samples per pixel will be adjusted to the nearest perfect square within the valid range
   * between MIN_SAMPLES_PER_PIXEL and MAX_SAMPLES_PER_PIXEL.
   * @param samples_per_pixels The desired number of samples per pixel.
   */
  void setSamplesPerPixel(int samples_per_pixels);

  /**
   * @brief Get the execution mode for rendering.
   * @return The current render execution mode.
   */
  RenderExecutionMode getExecutionMode() const { return m_execution_mode; }

  /**
   * @brief Set the execution mode for rendering.
   * @param mode The desired render execution mode.
   */
  void setExecutionMode(RenderExecutionMode mode) { m_execution_mode = mode; }

  /**
   * @brief Get the parameters for the renderer strategies.
   * @return A pointer to the Parameters object containing renderer parameters.
   */
  Parameters* getRendererParameters() const { return m_renderer_parameters.get(); }

  /**
   * @brief Set a parameter for the renderer.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setParameter(const std::string& name, int value) { m_renderer_parameters->setParameter(name, value); }

  ~RenderSettings() = default; ///< Default destructor.
};

#endif // RENDERING_RENDERSETTINGS_HPP