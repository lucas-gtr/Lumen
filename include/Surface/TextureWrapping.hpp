/**
 * @file TextureWrapping.hpp
 * @brief Header file for texture wrapping modes and functions.
 */
#ifndef SURFACE_TEXTUREWRAPPING_HPP
#define SURFACE_TEXTUREWRAPPING_HPP

#include <cstdint>

#include "Core/CommonTypes.hpp"

/**
 * @namespace TextureSampling
 * @brief Namespace for texture sampling functions and enums.
 */
namespace TextureSampling {
/**
 * @enum TextureWrapping
 * @brief Enumeration for texture wrapping modes.
 *
 * This enum defines the wrapping modes for textures, including REPEAT, MIRRORED_REPEAT,
 * CLAMP_TO_EDGE, and CLAMP_TO_BORDER.
 */
enum class TextureWrapping : std::uint8_t { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER };

/**
 * @brief Wraps the UV coordinates based on the specified wrapping mode.
 *
 * This function modifies the given UV coordinates according to the specified wrapping mode.
 *
 * @param uv_coord The UV coordinates to wrap.
 * @param wrappingMode The wrapping mode to apply.
 */
void wrapCoordinates(TextureUV& uv_coord, TextureWrapping wrappingMode);

/**
 * @brief Wraps the UV coordinates using the REPEAT wrapping mode.
 *
 * This function modifies the given UV coordinates to wrap around using the REPEAT mode.
 *
 * @param uv_coord The UV coordinates to wrap.
 */
void wrapRepeat(TextureUV& uv_coord);

/**
 * @brief Mirrors the coordinate value between 0 and 1.
 *
 * This function takes a coordinate value and mirrors it around the center of the texture.
 *
 * @param coord The coordinate value to mirror.
 */
double mirror(double coord);

/**
 * @brief Wraps the UV coordinates using the MIRRORED_REPEAT wrapping mode.
 *
 * This function modifies the given UV coordinates to wrap around using the MIRRORED_REPEAT mode.
 *
 * @param uv_coord The UV coordinates to wrap.
 */
void wrapMirroredRepeat(TextureUV& uv_coord);

/**
 * @brief Wraps the UV coordinates using the CLAMP_TO_EDGE wrapping mode.
 *
 * This function modifies the given UV coordinates to clamp to the edge of the texture.
 *
 * @param uv_coord The UV coordinates to wrap.
 */
void wrapClampToEdge(TextureUV& uv_coord);

/**
 * @brief Wraps the UV coordinates using the CLAMP_TO_BORDER wrapping mode.
 *
 * This function modifies the given UV coordinates to clamp to the border of the texture.
 *
 * @param uv_coord The UV coordinates to wrap.
 */
void wrapClampToBorder(TextureUV& uv_coord);

} // namespace TextureSampling

#endif // SURFACE_TEXTUREWRAPPING_HPP
