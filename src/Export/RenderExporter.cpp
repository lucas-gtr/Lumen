#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include "Core/Color.hpp"
#include "Core/Config.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"
#include "Export/OutputFormatBmp.hpp"
#include "Export/OutputFormatHdr.hpp"
#include "Export/OutputFormatJpeg.hpp"
#include "Export/OutputFormatPng.hpp"
#include "Export/OutputFormatTga.hpp"
#include "Export/RenderExporter.hpp"
#include "PostProcessing/ToneMapping/ACES.hpp"
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "PostProcessing/ToneMapping/LuminanceReinhard.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "PostProcessing/ToneMapping/Uncharted2.hpp"
#include "PostProcessing/ToneMapping/WhitePointReinhard.hpp"

RenderExporter::RenderExporter(Framebuffer* framebuffer)
    : m_framebuffer(framebuffer), m_output_format_strategy(std::make_unique<OutputFormatPng>()),
      m_tone_mapping_strategy(std::make_unique<NoToneMapping>()) {}

void RenderExporter::updateImageToExport() {
  const double* framebuffer = m_framebuffer->getFramebuffer();
  m_image_to_export.resize(m_framebuffer->getSize());

  m_tone_mapping_strategy->apply(framebuffer, m_image_to_export.data(),
                                 m_framebuffer->getWidth() * m_framebuffer->getHeight(),
                                 m_framebuffer->getChannelCount());
}

void RenderExporter::setExposure(double exposure) {
  m_exposure = std::max(0.0, exposure);
  if(m_tone_mapping == ToneMapping::EXPOSURE) {
    m_tone_mapping_strategy = std::make_unique<ExposureToneMapping>(m_exposure);
  }
  if(m_tone_mapping == ToneMapping::UNCHARTED2) {
    m_tone_mapping_strategy = std::make_unique<Uncharted2ToneMapping>(m_exposure, m_white_point);
  }
}

void RenderExporter::setWhitePoint(double white_point) {
  m_white_point = std::max(0.0, white_point);
  if(m_tone_mapping == ToneMapping::WHITE_POINT_REINHARD) {
    m_tone_mapping_strategy = std::make_unique<WhitePointReinhardToneMapping>(m_white_point);
  }
  if(m_tone_mapping == ToneMapping::UNCHARTED2) {
    m_tone_mapping_strategy = std::make_unique<Uncharted2ToneMapping>(m_exposure, m_white_point);
  }
}

void RenderExporter::setToneMapping(ToneMapping tone_mapping) {
  m_tone_mapping = tone_mapping;

  switch(m_tone_mapping) {
  case ToneMapping::NONE:
    m_tone_mapping_strategy = std::make_unique<NoToneMapping>();
    break;
  case ToneMapping::LUMINANCE_REINHARD:
    m_tone_mapping_strategy = std::make_unique<LuminanceReinhardToneMapping>();
    break;
  case ToneMapping::WHITE_POINT_REINHARD:
    m_tone_mapping_strategy = std::make_unique<WhitePointReinhardToneMapping>(m_white_point);
    break;
  case ToneMapping::ACES:
    m_tone_mapping_strategy = std::make_unique<AcesToneMapping>();
    break;
  case ToneMapping::UNCHARTED2:
    m_tone_mapping_strategy = std::make_unique<Uncharted2ToneMapping>(m_exposure, m_white_point);
    break;
  case ToneMapping::EXPOSURE:
    m_tone_mapping_strategy = std::make_unique<ExposureToneMapping>(m_exposure);
    break;
  }
}

void RenderExporter::setJpegQuality(int quality) {
  m_jpeg_quality = std::clamp(quality, MIN_JPEG_QUALITY, MAX_JPEG_QUALITY);
  if(m_output_format == OutputFormat::JPEG) {
    m_output_format_strategy = std::make_unique<OutputFormatJpeg>(m_jpeg_quality);
  }
}

void RenderExporter::setOutputFormat(OutputFormat output_format) {
  m_output_format = output_format;

  switch(m_output_format) {
  case OutputFormat::PNG:
    m_output_format_strategy = std::make_unique<OutputFormatPng>();
    break;
  case OutputFormat::JPEG:
    m_output_format_strategy = std::make_unique<OutputFormatJpeg>(m_jpeg_quality);
    break;
  case OutputFormat::BMP:
    m_output_format_strategy = std::make_unique<OutputFormatBmp>();
    break;
  case OutputFormat::TGA:
    m_output_format_strategy = std::make_unique<OutputFormatTga>();
    break;
  case OutputFormat::HDR:
    m_output_format_strategy = std::make_unique<OutputFormatHdr>();
    break;
  }
}

bool RenderExporter::exportHDR() const {
  std::vector<float> framebuffer(
      static_cast<size_t>(m_framebuffer->getWidth() * m_framebuffer->getHeight() * m_framebuffer->getChannelCount()));

  for(size_t i = 0; i < framebuffer.size(); ++i) {
    framebuffer[i] = static_cast<float>(m_framebuffer->getFramebuffer()[i]);
  }

  return OutputFormatHdr::WriteImageHdr(getPath() + getFilename(), m_framebuffer->getWidth(),
                                        m_framebuffer->getHeight(), m_framebuffer->getChannelCount(),
                                        framebuffer.data());
}

bool RenderExporter::exportLDR() const {
  return m_output_format_strategy->writeImage(getPath() + getFilename(), m_framebuffer->getWidth(),
                                              m_framebuffer->getHeight(), m_framebuffer->getChannelCount(),
                                              m_image_to_export.data());
}

bool RenderExporter::exportRender() {
  updateImageToExport();
  bool render_success = false;

  if(m_output_format == OutputFormat::HDR) {
    render_success = exportHDR();
  } else {
    render_success = exportLDR();
  }

  if(!render_success) {
    std::cerr << "Failed to export render to " << getPath() + getFilename() << '\n';
  } else {
    std::cout << "Render exported successfully to " << getPath() + getFilename() << '\n';
  }
  return render_success;
}