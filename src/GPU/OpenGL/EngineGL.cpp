// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "GPU/OpenGL/EngineGL.hpp"
#include "GPU/OpenGL/FramebufferGL.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "GPU/OpenGL/Lights/ShadowMapGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "GPU/OpenGL/ResourceManagerGL.hpp"
#include "GPU/OpenGL/ShadersGL.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "Scene/Scene.hpp"

EngineGL::EngineGL(int width, int height) : m_viewport_width(width), m_viewport_height(height) {}

void EngineGL::initialize(Scene* scene) {
  initializeOpenGLFunctions();

  m_resource_manager       = std::make_unique<ResourceManagerGL>(m_viewport_width, m_viewport_height, scene);
  m_scene_pass_framebuffer = std::make_unique<FramebufferGL>(m_viewport_width, m_viewport_height, 1, 4);
  m_resolve_framebuffer    = std::make_unique<FramebufferGL>(m_viewport_width, m_viewport_height, 1, 1);
  m_shadow_map_2_d         = std::make_unique<ShadowMapGL>(m_shadow_map_size);
  m_shadow_map_cube        = std::make_unique<ShadowMapGL>(m_shadow_map_size);

  loadShaderPrograms();
  initQuadVAO();
  configureOpenGLStates();
}

void EngineGL::loadShaderPrograms() {
  m_shadow_map_2_d_program.loadShaders("Resources/ShadersGL/ShadowMap2D.vert", "Resources/ShadersGL/ShadowMap2D.frag");
  m_shadow_map_cube_program.loadShaders("Resources/ShadersGL/ShadowMapCube.vert",
                                        "Resources/ShadersGL/ShadowMapCube.geom",
                                        "Resources/ShadersGL/ShadowMapCube.frag");
  m_scene_pass_program.loadShaders("Resources/ShadersGL/ScenePass.vert", "Resources/ShadersGL/ScenePass.frag");
  m_outline_program.loadShaders("Resources/ShadersGL/OutlineObject.vert", "Resources/ShadersGL/OutlineObject.frag");
  m_post_processing_program.loadShaders("Resources/ShadersGL/PostProcessing.vert",
                                        "Resources/ShadersGL/PostProcessing.frag");
  m_skybox_program.loadShaders("Resources/ShadersGL/Skybox.vert", "Resources/ShadersGL/Skybox.frag");
  m_viewport_grid_program.loadShaders("Resources/ShadersGL/ViewportGrid.vert", "Resources/ShadersGL/ViewportGrid.frag");
}

void EngineGL::configureOpenGLStates() {
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void EngineGL::initQuadVAO() {
  constexpr std::array<float, 16> quad_vertices = {
      // positions   // texture coords
      -1.0F, -1.0F, 0.0F, 0.0F, 1.0F, -1.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, -1.0F, 1.0F, 0.0F, 1.0F,
  };

  constexpr std::array<unsigned int, 6> quad_indices = {0, 1, 2, 2, 3, 0};
  glGenVertexArrays(1, &m_quad_vao);
  glBindVertexArray(m_quad_vao);

  unsigned int vbo = 0U;
  unsigned int ebo = 0U;

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(quad_vertices.size() * sizeof(float)), quad_vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(quad_indices.size() * sizeof(unsigned int)),
               quad_indices.data(), GL_STATIC_DRAW);

  // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
  glEnableVertexAttribArray(1);
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

void EngineGL::setShadowMapSize(int size) {
  m_shadow_map_size = std::clamp(size, MIN_SHADOW_MAP_SIZE, MAX_SHADOW_MAP_SIZE);
  m_shadow_map_2_d->resize(m_shadow_map_size);
  m_shadow_map_cube->resize(m_shadow_map_size);
  drawShadowMap2D();
  drawShadowMapCube();
}

void EngineGL::setViewportSize(int width, int height) {
  m_viewport_width  = width;
  m_viewport_height = height;

  m_resource_manager->getCamera()->setViewportSize(m_viewport_width, m_viewport_height);
  m_scene_pass_framebuffer->resize(m_viewport_width, m_viewport_height);
  m_resolve_framebuffer->resize(m_viewport_width, m_viewport_height);
  m_resolve_framebuffer->setTextureUnit(0, SCENE_TEXTURE_UNIT);
  glViewport(0, 0, m_viewport_width, m_viewport_height);
}

void EngineGL::setDynamicLighting(bool enabled) { m_dynamic_lighting = enabled; }

void EngineGL::setDynamicShadowMap(bool enabled) { m_dynamic_shadow_map = enabled; }

void EngineGL::setExposure(float exposure) {
  m_exposure = static_cast<float>(std::max(static_cast<double>(exposure), 0.0));

  m_post_processing_program.bind();
  m_post_processing_program.setUniform1f("exposure", m_exposure);
}

void EngineGL::setToneMapping(ToneMapping tone_mapping) {
  m_tone_mapping = tone_mapping;
  m_post_processing_program.bind();
  m_post_processing_program.setUniform1i("toneMapping", static_cast<int>(tone_mapping));
}

void EngineGL::drawShadowMap2D() {
  m_shadow_map_2_d->bindFramebuffer();

  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  for(int i = 0; i < std::min(static_cast<int>(m_resource_manager->getDirectionalLights().size()), 2); ++i) {
    drawLightShadowMap2D(m_resource_manager->getDirectionalLights()[i]->getLightSpaceMatrix(), i, 0);
  }

  for(int i = 0; i < std::min(static_cast<int>(m_resource_manager->getSpotLights().size()), 2); ++i) {
    drawLightShadowMap2D(m_resource_manager->getSpotLights()[i]->getLightSpaceMatrix(), i, 1);
  }

  m_shadow_map_2_d->bindTexture(SHADOW_2D_MAP_TEXTURE_UNIT);
}

void EngineGL::drawLightShadowMap2D(const float* light_space_matrix, int index_x, int index_y) {
  const int tile_size = static_cast<int>(m_shadow_map_size * HALF);
  glViewport(tile_size * index_x, tile_size * index_y, tile_size, tile_size);

  m_shadow_map_2_d_program.bind();
  m_shadow_map_2_d_program.setUniformMatrix4fv("lightSpaceMatrix", light_space_matrix);

  for(const auto& data_buffer : m_resource_manager->getObjectList()) {
    data_buffer->bindVAO();
    m_shadow_map_2_d_program.setUniformMatrix4fv("model", data_buffer->getModelMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data_buffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }
}

void EngineGL::drawShadowMapCube() {
  m_shadow_map_cube->bindFramebuffer();

  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  if(!m_resource_manager->getPointLights().empty()) {
    drawPointShadowMap(m_resource_manager->getPointLights()[0].get());
  }

  m_shadow_map_cube->bindTexture(SHADOW_CUBE_MAP_TEXTURE_UNIT);
}

void EngineGL::drawPointShadowMap(const PointLightGL* light) {
  glViewport(0, 0, m_shadow_map_size, m_shadow_map_size);

  m_shadow_map_cube_program.bind();
  for(int i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
    std::string const matrix_uniform = "lightSpaceMatrices[" + std::to_string(i) + "]";
    m_shadow_map_cube_program.setUniformMatrix4fv(matrix_uniform.c_str(), light->getLightSpaceMatrix(i));
  }

  m_shadow_map_cube_program.setUniform3f("lightPos", light->getPosition().x, light->getPosition().y,
                                         light->getPosition().z);
  m_shadow_map_cube_program.setUniform1f("farPlane", light->getFarPlane());

  for(const auto& data_buffer : m_resource_manager->getObjectList()) {
    data_buffer->bindVAO();
    m_shadow_map_cube_program.setUniformMatrix4fv("model", data_buffer->getModelMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data_buffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }

  m_scene_pass_program.bind();
  m_scene_pass_program.setUniform1f("shadowMapCubeFarPlane", light->getFarPlane());
}

void EngineGL::uploadLightsData() {
  m_scene_pass_program.bind();

  uploadDirectionalLightsData();
  uploadSpotLightsData();
  uploadPointLightsData();
}

void EngineGL::uploadDirectionalLightsData() {
  m_scene_pass_program.setUniform1i("numDirLights",
                                    static_cast<int>(m_resource_manager->getDirectionalLights().size()));
  for(int i = 0; i < m_resource_manager->getDirectionalLights().size(); ++i) {
    const auto& light     = m_resource_manager->getDirectionalLights()[i];
    const auto& direction = light->getDirection();
    const auto& color     = light->getColor();

    const std::string dir_uniform    = "dirLights[" + std::to_string(i) + "].direction";
    const std::string color_uniform  = "dirLights[" + std::to_string(i) + "].color";
    const std::string matrix_uniform = "dirLights[" + std::to_string(i) + "].lightSpaceMatrix";

    m_scene_pass_program.setUniform3f(dir_uniform.c_str(), direction.x, direction.y, direction.z);
    m_scene_pass_program.setUniform3f(color_uniform.c_str(), color.x, color.y, color.z);

    m_scene_pass_program.setUniformMatrix4fv(matrix_uniform.c_str(), light->getLightSpaceMatrix());
  }
}

void EngineGL::uploadSpotLightsData() {
  m_scene_pass_program.setUniform1i("numSpotLights", static_cast<int>(m_resource_manager->getSpotLights().size()));
  for(int i = 0; i < m_resource_manager->getSpotLights().size(); ++i) {
    const auto& light         = m_resource_manager->getSpotLights()[i];
    const auto& position      = light->getPosition();
    const auto& direction     = light->getDirection();
    const auto& color         = light->getColor();
    const float inner_cut_off = light->getCosInnerCutoff();
    const float outer_cut_off = light->getCosOuterCutoff();

    const std::string pos_uniform    = "spotLights[" + std::to_string(i) + "].position";
    const std::string dir_uniform    = "spotLights[" + std::to_string(i) + "].direction";
    const std::string inner_uniform  = "spotLights[" + std::to_string(i) + "].innerCutOff";
    const std::string outer_uniform  = "spotLights[" + std::to_string(i) + "].outerCutOff";
    const std::string color_uniform  = "spotLights[" + std::to_string(i) + "].color";
    const std::string matrix_uniform = "spotLights[" + std::to_string(i) + "].lightSpaceMatrix";

    m_scene_pass_program.setUniform3f(pos_uniform.c_str(), position.x, position.y, position.z);
    m_scene_pass_program.setUniform3f(dir_uniform.c_str(), direction.x, direction.y, direction.z);
    m_scene_pass_program.setUniform1f(inner_uniform.c_str(), inner_cut_off);
    m_scene_pass_program.setUniform1f(outer_uniform.c_str(), outer_cut_off);
    m_scene_pass_program.setUniform3f(color_uniform.c_str(), color.x, color.y, color.z);
    m_scene_pass_program.setUniformMatrix4fv(matrix_uniform.c_str(), light->getLightSpaceMatrix());
  }
}

void EngineGL::uploadPointLightsData() {
  m_scene_pass_program.setUniform1i("numPointLights", static_cast<int>(m_resource_manager->getPointLights().size()));
  for(int i = 0; i < m_resource_manager->getPointLights().size(); ++i) {
    const auto& light    = m_resource_manager->getPointLights()[i];
    const auto& position = light->getPosition();
    const auto& color    = light->getColor();

    const std::string pos_uniform   = "pointLights[" + std::to_string(i) + "].position";
    const std::string color_uniform = "pointLights[" + std::to_string(i) + "].color";

    m_scene_pass_program.setUniform3f(pos_uniform.c_str(), position.x, position.y, position.z);
    m_scene_pass_program.setUniform3f(color_uniform.c_str(), color.x, color.y, color.z);
  }
}

void EngineGL::bakeLights() {
  drawShadowMap2D();
  drawShadowMapCube();

  uploadLightsData();
}

void EngineGL::configureShadersAndUniforms() {
  m_resolve_framebuffer->setTextureUnit(0, SCENE_TEXTURE_UNIT);

  m_post_processing_program.bind();
  m_post_processing_program.setUniform1i("screenTexture", SCENE_TEXTURE_UNIT);
  m_post_processing_program.setUniform1f("exposure", m_exposure);

  m_skybox_program.bind();
  m_skybox_program.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);

  m_viewport_grid_program.bind();
  m_viewport_grid_program.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);

  m_outline_program.bind();
  m_outline_program.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);

  m_scene_pass_program.bind();
  m_scene_pass_program.setUniform1i("diffuseTexture", DIFFUSE_TEXTURE_UNIT);
  m_scene_pass_program.setUniform1i("normalTexture", NORMAL_TEXTURE_UNIT);
  m_scene_pass_program.setUniform1i("shadowMap2D", SHADOW_2D_MAP_TEXTURE_UNIT);
  m_scene_pass_program.setUniform1i("shadowMapCube", SHADOW_CUBE_MAP_TEXTURE_UNIT);
  m_scene_pass_program.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);
}

void EngineGL::renderScenePass() {
  glEnable(GL_DEPTH_TEST);
  glClearStencil(0);
  glEnable(GL_STENCIL_TEST);
  glStencilMask(FULL_STENCIL_MASK);
  glClearColor(BACKGROUND_COLOR, BACKGROUND_COLOR, BACKGROUND_COLOR, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDisable(GL_STENCIL_TEST);

  m_scene_pass_program.bind();

  for(const auto& data_buffer : m_resource_manager->getObjectList()) {
    data_buffer->bindVAO();
    data_buffer->bindMaterial();
    m_scene_pass_program.setUniformMatrix4fv("model", data_buffer->getModelMatrix());
    m_scene_pass_program.setUniformMatrix3fv("normalMatrix", data_buffer->getNormalMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data_buffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }

  glBindVertexArray(0);
}

void EngineGL::drawOutline() {
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 1, FULL_STENCIL_MASK);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
  glStencilMask(FULL_STENCIL_MASK);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  m_outline_program.bind();

  for(const auto& data_buffer : m_resource_manager->getObjectList()) {
    if(!data_buffer->isSelected()) {
      continue;
    }
    data_buffer->bindVAO();
    data_buffer->bindMaterial();
    m_outline_program.setUniformMatrix4fv("model", data_buffer->getModelMatrix());
    m_outline_program.setUniform1f("outline_scale", 1.00F);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data_buffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_TRUE);
  glStencilFunc(GL_NOTEQUAL, 1, FULL_STENCIL_MASK);
  glStencilMask(NO_STENCIL_MASK);

  for(const auto& data_buffer : m_resource_manager->getObjectList()) {
    if(!data_buffer->isSelected()) {
      continue;
    }
    data_buffer->bindVAO();
    data_buffer->bindMaterial();
    m_outline_program.setUniformMatrix4fv("model", data_buffer->getModelMatrix());
    m_outline_program.setUniform1f("outline_scale", OUTLINE_SCALE);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data_buffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
}

void EngineGL::drawSkybox() {
  m_skybox_program.bind();
  m_skybox_program.setUniform1i("skybox", SKYBOX_TEXTURE_UNIT);

  glDepthFunc(GL_LEQUAL); // Set depth function for skybox rendering

  glBindVertexArray(m_quad_vao);
  glDrawElements(GL_TRIANGLES, QUAD_INDICES_COUNT, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS); // Reset depth function
}

void EngineGL::drawViewportGrid() {
  m_skybox_program.bind();
  m_skybox_program.setUniform1i("skybox", 0);

  m_viewport_grid_program.bind();

  glBindVertexArray(m_quad_vao);
  glDrawElements(GL_TRIANGLES, QUAD_INDICES_COUNT, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void EngineGL::blitSceneToResolveFramebuffer() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_scene_pass_framebuffer->getFramebufferID());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolve_framebuffer->getFramebufferID());

  glBlitFramebuffer(0, 0, m_viewport_width, m_viewport_height, 0, 0, m_viewport_width, m_viewport_height,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void EngineGL::applyPostProcessing() {

  m_post_processing_program.bind();

  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(m_quad_vao);

  glDrawElements(GL_TRIANGLES, QUAD_INDICES_COUNT, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
}

void EngineGL::setupRendering() {
  configureShadersAndUniforms();
  m_shadow_map_2_d->initialize2DMap();
  m_shadow_map_cube->initializeCubeMap();
  bakeLights();

  glViewport(0, 0, m_viewport_width, m_viewport_height);
}

void EngineGL::renderFrame(unsigned int default_ramebuffer) {
  if(m_dynamic_lighting) {
    uploadLightsData();
  }

  if(m_dynamic_shadow_map) {
    drawShadowMap2D();
    // drawShadowMapCube();
  }

  m_scene_pass_framebuffer->bind();
  renderScenePass();
  drawOutline();

  if(m_draw_skybox) {
    drawSkybox();
  } else {
    drawViewportGrid();
  }

  blitSceneToResolveFramebuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, default_ramebuffer); // back to default
  applyPostProcessing();
}

EngineGL::~EngineGL() {
  glDeleteVertexArrays(1, &m_quad_vao);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

// GCOVR_EXCL_STOP