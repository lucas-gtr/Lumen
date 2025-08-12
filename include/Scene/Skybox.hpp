/**
 * @file Skybox.hpp
 * @brief Header file for the Skybox class.
 */
#ifndef SCENE_SKYBOX_HPP
#define SCENE_SKYBOX_HPP

#include <memory>

#include <linalg/Vec3.hpp>

#include "Core/Color.hpp"
#include "Core/ImageTypes.hpp"
#include "Core/Observer.hpp"

class Texture;

/**
 * @class Skybox
 * @brief Class representing a skybox in a 3D scene.
 *
 * This class encapsulates the properties of a skybox, including its texture and
 * functionality for getting color and UV coordinates based on a given direction.
 */
class Skybox {
private:
  Texture* m_texture;

  Observer<> m_texture_observer;

public:
  Skybox(); ///< Default constructor.

  Skybox(const Skybox&)            = delete;
  Skybox(Skybox&&)                 = delete;
  Skybox& operator=(const Skybox&) = delete;
  Skybox& operator=(Skybox&&)      = delete;

  /**
   * @brief Gets the observer for texture changes.
   * @return A reference to the observer that notifies about texture changes.
   */
  Observer<>& getTextureObserver() { return m_texture_observer; }

  /**
   * @brief Sets the texture for the skybox.
   * @param texture The texture to be set for the skybox.
   */
  void setTexture(Texture* texture);

  /**
   * @brief Gets the UV coordinates based on the given direction.
   * @param direction The direction vector for which to get the UV coordinates.
   * @return The UV coordinates for the given direction.
   */
  static TextureUV GetUvCoordinates(const linalg::Vec3d& direction);

  /**
   * @brief Gets the color based on the given direction.
   * @param direction The direction vector for which to get the color.
   * @return The color for the given direction.
   */
  ColorRGB getColor(const linalg::Vec3d& direction) const;

  /**
   * @brief Gets the texture associated with the skybox.
   * @return A pointer to the Texture used for the skybox.
   */
  Texture* getTexture() const { return m_texture; }

  ~Skybox() = default; ///< Default destructor.
};

#endif // SCENE_SKYBOX_HPP