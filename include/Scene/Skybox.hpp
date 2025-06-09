/**
 * @file Skybox.hpp
 * @brief Header file for the Skybox class.
 */
#ifndef SCENE_SKYBOX_HPP
#define SCENE_SKYBOX_HPP

#include <memory>

#include "Core/CommonTypes.hpp"
#include "Core/Math/Vec3.hpp"
#include "Surface/Texture.hpp"

/**
 * @class Skybox
 * @brief Class representing a skybox in a 3D scene.
 *
 * This class encapsulates the properties of a skybox, including its texture and
 * functionality for getting color and UV coordinates based on a given direction.
 */
class Skybox {
private:
  std::shared_ptr<Texture> m_texture;

public:
  Skybox(); ///< Default constructor.

  Skybox(const Skybox&)            = delete;
  Skybox(Skybox&&)                 = delete;
  Skybox& operator=(const Skybox&) = delete;
  Skybox& operator=(Skybox&&)      = delete;

  /**
   * @brief Sets the texture for the skybox.
   * @param texture The texture to be set for the skybox.
   */
  void setTexture(const std::shared_ptr<Texture>& texture);

  /**
   * @brief Gets the UV coordinates based on the given direction.
   * @param direction The direction vector for which to get the UV coordinates.
   * @return The UV coordinates for the given direction.
   */
  static TextureUV getUVCoordinates(const lin::Vec3d& direction);

  /**
   * @brief Gets the color based on the given direction.
   * @param direction The direction vector for which to get the color.
   * @return The color for the given direction.
   */
  ColorRGBA getColor(const lin::Vec3d& direction) const;

  const Texture* getTexture() const {
    return m_texture ? m_texture.get() : nullptr; ///< Returns the texture if it exists, otherwise returns nullptr.
  }

  ~Skybox() = default; ///< Default destructor.
};

#endif // SCENE_SKYBOX_HPP