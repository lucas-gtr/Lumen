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
#include "ImplementationParameters/Parameters.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"

/**
 * @class RenderExporter
 * @brief Abstract base class for exporting rendered frames.
 */
class RenderExporter {
private:
  Framebuffer*                  m_framebuffer;
  std::string                   m_path     = std::string(DEFAULT_FILE_PATH);
  std::string                   m_filename = std::string(DEFAULT_FILE_NAME);
  std::unique_ptr<OutputFormat> m_output_format;

  std::unique_ptr<Parameters>          m_tonemapping_parameters;
  std::unique_ptr<ToneMappingStrategy> m_tone_mapping;

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
  void setOutputFormat(std::unique_ptr<OutputFormat> output_format);

  /**
   * @brief Sets the tone mapping strategy for the render.
   * @param tone_mapping Pointer to the ToneMappingStrategy object that defines the tone mapping.
   */
  void setToneMapping(ToneMapping tone_mapping);

  /**
   * @brief Retrieves the framebuffer.
   * @return Pointer to the framebuffer containing the rendered image.
   */
  Framebuffer* getFramebuffer() const { return m_framebuffer; }

  /**
   * @brief Retrieves the parameters for tone mapping.
   * @return Pointer to the Parameters object containing tone mapping parameters.
   */
  Parameters* getParameters() const { return m_tonemapping_parameters.get(); }

  /**
   * @brief Sets a parameter (an integer) for exporting.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setParameter(const std::string& name, int value) { m_tonemapping_parameters->setParameter(name, value); }

  /**
   * @brief Sets a parameter (a double) for exporting.
   * @param name  The name of the parameter to set.
   * @param value The value of the parameter to set.
   */
  void setParameter(const std::string& name, double value) { m_tonemapping_parameters->setParameter(name, value); }

  /**
   * @brief Exports the render to the specified location.
   *
   * This method must be implemented by derived classes to define the actual export behavior.
   */
  void exportRender();

  ~RenderExporter() = default; ///< Default destructor.
};

#endif // EXPORT_RENDEREXPORTER_HPP