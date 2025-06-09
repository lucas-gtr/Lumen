#include "GPU/ITextureGPU.hpp"
#include "Surface/Texture.hpp"

ITextureGPU::ITextureGPU(const Texture& texture) : m_texture(&texture) {}