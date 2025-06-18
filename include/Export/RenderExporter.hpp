/**
 * @file RenderExporter.hpp
 * @brief Header file for the RenderExporter class.
 */
#ifndef EXPORT_RENDEREXPORTER_HPP
#define EXPORT_RENDEREXPORTER_HPP

#include <memory>
#include <string>

#include "Core/Config.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class RenderExporter
 * @brief Abstract base class for exporting rendered frames.
 */
class RenderExporter {
private:
  Framebuffer*               m_framebuffer;
  std::vector<unsigned char> m_image_to_export;

  std::string m_path     = std::string(DEFAULT_FILE_PATH);
  std::string m_filename = std::string(DEFAULT_FILE_NAME);

  int                                   m_jpeg_quality  = DEFAULT_JPEG_QUALITY;
  OutputFormat                          m_output_format = OutputFormat::PNG;
  std::unique_ptr<OutputFormatStrategy> m_output_format_strategy;

  double                               m_exposure     = DEFAULT_TONE_MAPPING_EXPOSURE;
  ToneMapping                          m_tone_mapping = ToneMapping::NONE;
  std::unique_ptr<ToneMappingStrategy> tone_mapping_strategy;

public:
  /**
   * @brief Constructs a RenderExporter instance.
   * @param framebuffer Pointer to the framebuffer that holds the rendered image.
   */
  explicit RenderExporter(Framebuffer* framebuffer);

  RenderExporter(const RenderExporter&)            = delete;
  RenderExporter& operator=(const RenderExporter&) = delete;
  RenderExporter(RenderExporter&&)                 = delete;
  RenderExporter& operator=(RenderExporter&&)      = delete;

  /**
   * @brief Updates the image to export based on the current framebuffer.
   */
  void updateImageToExport();

  /**
   * @brief Retrieves the image data to be exported.
   * @return Pointer to the image data (unsigned char array).
   */
  const unsigned char* getImageToExport() { return m_image_to_export.data(); }

  /**
   * @brief Retrieves the filename.
   * @return The filename of the exported render.
   */
  std::string getFilename() const { return m_filename; }

  /**
   * @brief Sets the filename.
   * @param filename The new filename for the exported render.
   */
  void setFilename(const std::string& filename) { m_filename = filename; }

  /**
   * @brief Retrieves the directory path.
   * @return The directory path where the render will be exported.
   */
  std::string getPath() const { return m_path; }

  /**
   * @brief Sets the directory path.
   * @param path The new directory path for the exported render.
   */
  void setPath(const std::string& path) { m_path = path.back() == '/' ? path : path + '/'; }

  /**
   * @brief Sets the output format for the render.
   * @param output_format Pointer to the OutputFormat object that defines the export format.
   */
  void setOutputFormat(OutputFormat output_format);

  /**
   * @brief Retrieves the current output format.
   * @return The current output format of the render.
   */
  OutputFormat getOutputFormat() const { return m_output_format; }

  /**
   * @brief Sets the tone mapping strategy for the render.
   * @param tone_mapping Pointer to the ToneMappingStrategy object that defines the tone mapping.
   */
  void setToneMapping(ToneMapping tone_mapping);

  /**
   * @brief Retrieves the current tone mapping strategy.
   * @return The current tone mapping strategy used for the render.
   */
  ToneMapping getToneMapping() const { return m_tone_mapping; }

  /**
   * @brief Sets the exposure value for the render, for exposure-based tone mapping.
   * @param exposure The exposure value to be set for the render.
   */
  void setExposure(double exposure);

  /**
   * @brief Retrieves the current exposure value.
   * @return The current exposure value used for tone mapping.
   */
  double getExposure() const { return m_exposure; }

  /**
   * @brief Sets the JPEG quality for the render.
   * @param quality The JPEG quality level (0-100).
   */
  void setJpegQuality(int quality);

  /**
   * @brief Retrieves the current JPEG quality.
   * @return The current JPEG quality level.
   */
  int getJpegQuality() const { return m_jpeg_quality; }

  /**
   * @brief Retrieves the framebuffer.
   * @return Pointer to the framebuffer containing the rendered image.
   */
  Framebuffer* getFramebuffer() const { return m_framebuffer; }

  /**
   * @brief Exports the render to the specified location.
   *
   * This method must be implemented by derived classes to define the actual export behavior.
   */
  void exportRender();

  ~RenderExporter() = default; ///< Default destructor.
};

#endif // EXPORT_RENDEREXPORTER_HPP