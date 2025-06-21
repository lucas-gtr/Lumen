#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include "Core/ColorUtils.hpp"
#include "Core/Config.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"
#include "Export/OutputFormatBmp.hpp"
#include "Export/OutputFormatHdr.hpp"
#include "Export/OutputFormatJpeg.hpp"
#include "Export/OutputFormatPng.hpp"
#include "Export/OutputFormatTga.hpp"
#include "Export/RenderExporter.hpp"
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "PostProcessing/ToneMapping/Reinhard.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"

RenderExporter::RenderExporter(Framebuffer* framebuffer)
    : m_framebuffer(framebuffer), m_output_format_strategy(std::make_unique<OutputFormatPng>()),
      tone_mapping_strategy(std::make_unique<NoToneMapping>()) {}

void RenderExporter::updateImageToExport() {
  const double* framebuffer = m_framebuffer->getFramebuffer();
  m_image_to_export.resize(m_framebuffer->getSize());

  const int channel_count = m_framebuffer->getChannelCount();
  for(int i = 0; i < m_framebuffer->getWidth() * m_framebuffer->getHeight(); ++i) {
    for(int j = 0; j < std::min(channel_count, 3); j++) {
      double value = framebuffer[i * channel_count + j];
      tone_mapping_strategy->apply(value);
      m_image_to_export[i * channel_count + j] = static_cast<unsigned char>(value * NORMALIZED_TO_COLOR8);
    }
    if(channel_count == 4) {
      const double alpha                       = framebuffer[i * channel_count + 3];
      m_image_to_export[i * channel_count + 3] = static_cast<unsigned char>(alpha * NORMALIZED_TO_COLOR8);
    }
  }
}

void RenderExporter::setExposure(double exposure) {
  m_exposure = std::clamp(exposure, MIN_TONE_MAPPING_EXPOSURE, MAX_TONE_MAPPING_EXPOSURE);
  if(m_tone_mapping == ToneMapping::EXPOSURE) {
    tone_mapping_strategy = std::make_unique<ExposureToneMapping>(m_exposure);
  }
}

void RenderExporter::setToneMapping(ToneMapping tone_mapping) {
  m_tone_mapping = tone_mapping;

  switch(m_tone_mapping) {
  case ToneMapping::NONE:
    tone_mapping_strategy = std::make_unique<NoToneMapping>();
    break;
  case ToneMapping::REINHARD:
    tone_mapping_strategy = std::make_unique<ReinhardToneMapping>();
    break;
  case ToneMapping::EXPOSURE:
    tone_mapping_strategy = std::make_unique<ExposureToneMapping>(m_exposure);
    break;
  default:
    std::cerr << "Unknown tone mapping strategy. Using no tone mapping." << '\n';
    tone_mapping_strategy = std::make_unique<NoToneMapping>();
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
  default:
    std::cerr << "Unknown output format. Using PNG format." << '\n';
    m_output_format_strategy = std::make_unique<OutputFormatPng>();
  }
}

void RenderExporter::exportRender() {
  if(m_output_format_strategy == nullptr) {
    std::cerr << "Output format is not set. Using default PNG format." << '\n';
    return;
  }
  bool render_success = false;

  if(m_output_format == OutputFormat::HDR) {
    std::vector<float> framebuffer(
        static_cast<size_t>(m_framebuffer->getWidth() * m_framebuffer->getHeight() * m_framebuffer->getChannelCount()));
    for(size_t i = 0; i < framebuffer.size(); ++i) {
      framebuffer[i] = static_cast<float>(m_framebuffer->getFramebuffer()[i]);
    }
    OutputFormatHdr::write_image_hdr(getPath() + getFilename(), m_framebuffer->getWidth(), m_framebuffer->getHeight(),
                                     m_framebuffer->getChannelCount(), framebuffer.data());

  } else {
    render_success = m_output_format_strategy->write_image(
        getPath() + getFilename(), getFramebuffer()->getWidth(), getFramebuffer()->getHeight(),
        getFramebuffer()->getChannelCount(), m_image_to_export.data());
  }

  if(!render_success) {
    std::cerr << "Failed to export render to " << getPath() + getFilename() << '\n';
  } else {
    std::cout << "Render exported successfully to " << getPath() + getFilename() << '\n';
  }
}