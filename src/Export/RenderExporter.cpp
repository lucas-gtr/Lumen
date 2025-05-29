#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Core/ColorUtils.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"
#include "Export/RenderExporter.hpp"
#include "ImplementationParameters/Parameters.hpp"
#include "PostProcessing/ToneMapping/Exposure.hpp"
#include "PostProcessing/ToneMapping/None.hpp"
#include "PostProcessing/ToneMapping/Reinhard.hpp"

RenderExporter::RenderExporter(Framebuffer* framebuffer)
    : m_framebuffer(framebuffer), m_output_format(nullptr), m_tone_mapping(nullptr) {
  m_tonemapping_parameters = std::make_unique<Parameters>();
}

void RenderExporter::setToneMapping(ToneMapping tone_mapping) {
  switch(tone_mapping) {
  case ToneMapping::NONE:
    m_tone_mapping = std::make_unique<NoToneMapping>();
    break;
  case ToneMapping::REINHARD:
    m_tone_mapping = std::make_unique<ReinhardToneMapping>();
    break;
  case ToneMapping::EXPOSURE:
    m_tone_mapping = std::make_unique<ExposureToneMapping>(m_tonemapping_parameters.get());
    break;
  }
}

void RenderExporter::setOutputFormat(std::unique_ptr<OutputFormat> output_format) {
  m_output_format = std::move(output_format);
}

void RenderExporter::exportRender() {
  if(m_output_format == nullptr) {
    std::cerr << "Output format is not set. Using default PNG format." << '\n';
    return;
  }

  if(m_tone_mapping == nullptr) {
    std::cerr << "Tone mapping strategy is not set. Using default NoToneMapping." << '\n';
    m_tone_mapping = std::make_unique<NoToneMapping>();
  }

  m_tone_mapping->updateParameters(m_tonemapping_parameters.get());
  const double*              framebuffer = m_framebuffer->getFramebuffer();
  std::vector<unsigned char> image(m_framebuffer->getSize());

  const int channel_count = m_framebuffer->getChannelCount();
  for(int i = 0; i < m_framebuffer->getWidth() * m_framebuffer->getHeight(); ++i) {
    for(int j = 0; j < std::min(channel_count, 3); j++) {
      double value = framebuffer[i * channel_count + j];
      m_tone_mapping->apply(value);
      image[i * channel_count + j] = static_cast<unsigned char>(value * NORMALIZED_TO_COLOR8);
    }
    if(channel_count == 4) {
      const double alpha           = framebuffer[i * channel_count + 3];
      image[i * channel_count + 3] = static_cast<unsigned char>(alpha * NORMALIZED_TO_COLOR8);
    }
  }

  m_output_format->write_image(getPath() + getFilename(), getFramebuffer()->getWidth(), getFramebuffer()->getHeight(),
                               getFramebuffer()->getChannelCount(), image.data());
}