#include "Export/RenderExporter.hpp"
#include "Core/Framebuffer.hpp"

RenderExporter::RenderExporter(Framebuffer* framebuffer) : m_framebuffer(framebuffer) {}

void RenderExporter::exportRender() {
  getFramebuffer()->generateImage();
  m_output_format->write_image(getPath() + getFilename(), static_cast<int>(getFramebuffer()->getWidth()),
                               static_cast<int>(getFramebuffer()->getHeight()),
                               static_cast<int>(getFramebuffer()->getChannelCount()), getFramebuffer()->getImage());
}