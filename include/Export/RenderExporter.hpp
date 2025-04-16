/**
 * @file RenderExporter.hpp
 * @brief Header file for the RenderExporter class.
 */
#ifndef EXPORT_RENDEREXPORTER_HPP
#define EXPORT_RENDEREXPORTER_HPP

#include "Core/Config.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"

#include <string>

/**
 * @class RenderExporter
 * @brief Abstract base class for exporting rendered frames.
 */
class RenderExporter {
private:
  Framebuffer*        m_framebuffer;
  std::string         m_path          = std::string(DEFAULT_FILE_PATH);
  std::string         m_filename      = std::string(DEFAULT_FILE_NAME);
  const OutputFormat* m_output_format = nullptr;

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
  void setOutputFormat(const OutputFormat* output_format) { m_output_format = output_format; }
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