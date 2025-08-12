#include "PostProcessing/ToneMapping/ToneMapping.hpp"

void ToneMappingStrategy::apply(const double* framebuffer, unsigned char* output, int pixel_count, int in_channels,
                                int out_channels) const {
  for(int framebuffer_index = 0; framebuffer_index < pixel_count * in_channels; framebuffer_index += in_channels) {
    const ColorRGB pixel_color = {framebuffer[framebuffer_index], framebuffer[framebuffer_index + 1],
                                  framebuffer[framebuffer_index + 2]};
    ColorRGB       ldr_color   = convertToLDR(pixel_color);
    ldr_color.clamp(0.0, 1.0);

    const int output_index = (framebuffer_index / in_channels) * out_channels;
    switch(out_channels) {
    case 1: // Grayscale
      output[output_index] = static_cast<unsigned char>(ldr_color.grayscale() * NORMALIZED_TO_COLOR8);
      break;
    case 4: {
      double alpha = 1.0;
      if(in_channels == 4) {
        alpha = std::clamp(framebuffer[output_index + 3], 0.0, 1.0);
      }
      output[output_index + 3] = static_cast<unsigned char>(alpha * NORMALIZED_TO_COLOR8);
      [[fallthrough]]; // Fall through to RGB case for RGBA output
    }
    case 3: // RGB
      output[output_index]     = static_cast<unsigned char>(ldr_color.r * NORMALIZED_TO_COLOR8);
      output[output_index + 1] = static_cast<unsigned char>(ldr_color.g * NORMALIZED_TO_COLOR8);
      output[output_index + 2] = static_cast<unsigned char>(ldr_color.b * NORMALIZED_TO_COLOR8);
      break;
    default:
      throw std::runtime_error("Unsupported channel count in ToneMappingStrategy::apply : " +
                               std::to_string(out_channels));
    }
  }
}