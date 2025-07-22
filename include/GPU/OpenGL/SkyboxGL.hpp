/**
 * @file SkyboxGL.hpp
 * @brief Header file for the SkyboxGL class, which manages OpenGL skybox rendering
 */
#ifndef GPU_OPENGL_SKYBOXGL_HPP
#define GPU_OPENGL_SKYBOXGL_HPP

// GCOVR_EXCL_START

#include <memory>

#include "GPU/OpenGL/TextureGL.hpp"
#include "Scene/Skybox.hpp"
#include "Surface/Texture.hpp"

/**
 * @class SkyboxGL
 * @brief Represents a skybox in OpenGL, managing the skybox texture and rendering.
 *
 * This class encapsulates the functionality for rendering a skybox using OpenGL.
 * It manages the skybox texture and provides methods for rendering the skybox in the scene.
 */
class SkyboxGL {
private:
  std::unique_ptr<TextureGL> m_texture; ///< The OpenGL texture representing the skybox.
  Skybox*                    m_skybox;  ///< Pointer to the Skybox object containing the texture and properties.
public:
  /**
   * @brief Constructor for the SkyboxGL class.
   * @param skybox Pointer to the Skybox object to be managed.
   */
  explicit SkyboxGL(Skybox* skybox);

  /**
   * @brief Updates the skybox texture based on the current properties of the Skybox object.
   */
  void updateSkyboxTexture();

  /**
   * @brief Gets the current skybox texture.
   * @return A pointer to the TextureGL object representing the skybox texture.
   */
  TextureGL* getTexture() const;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_SKYBOXGL_HPP