// GCOVR_EXCL_START
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <string>

#include "Core/Config.hpp"
#include "Core/MathConstants.hpp"
#include "GPU/OpenGL/EngineGL.hpp"
#include "GPU/OpenGL/FramebufferGL.hpp"
#include "GPU/OpenGL/Lights/PointGL.hpp"
#include "GPU/OpenGL/ObjectGL.hpp"
#include "GPU/OpenGL/ShadersGL.hpp"
#include "GPU/OpenGL/WindowGL.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "Scene/Scene.hpp"

EngineGL::EngineGL(int width, int height, const std::string& title, Scene& scene)
    : m_window(width, height, title), m_viewport_width(width), m_viewport_height(height),
      m_resource_manager(width, height, scene), m_scenePassFramebuffer(width, height, 1, 4),
      m_resolveFramebuffer(width, height, 1, 1), m_shadowMap2D(m_shadowMapsSize), m_shadowMapCube(m_shadowMapsSize) {
  setupGLFWCallbacks();
  loadShaderPrograms();
  initQuadVAO();
  configureOpenGLStates();
}

void EngineGL::setupGLFWCallbacks() {
  glfwSetWindowUserPointer(m_window.gl_window(), this);
  glfwSetFramebufferSizeCallback(m_window.gl_window(), framebufferSizeCallback);
  glfwSetScrollCallback(m_window.gl_window(), scrollCallback);
  glfwSetCursorPosCallback(m_window.gl_window(), cursorPositionCallback);
}

void EngineGL::loadShaderPrograms() {
  m_shadowMap2DProgram.loadShaders("Resources/ShadersGL/ShadowMap2D.vert", "Resources/ShadersGL/ShadowMap2D.frag");
  m_shadowMapCubeProgram.loadShaders("Resources/ShadersGL/ShadowMapCube.vert", "Resources/ShadersGL/ShadowMapCube.geom",
                                     "Resources/ShadersGL/ShadowMapCube.frag");
  m_scenePassProgram.loadShaders("Resources/ShadersGL/ScenePass.vert", "Resources/ShadersGL/ScenePass.frag");
  m_postProcessingProgram.loadShaders("Resources/ShadersGL/PostProcessing.vert",
                                      "Resources/ShadersGL/PostProcessing.frag");
  m_skyboxProgram.loadShaders("Resources/ShadersGL/Skybox.vert", "Resources/ShadersGL/Skybox.frag");
}

void EngineGL::configureOpenGLStates() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
}

void EngineGL::initQuadVAO() {
  constexpr std::array<float, 16> quadVertices = {
      // positions   // texture coords
      -1.0F, -1.0F, 0.0F, 0.0F, 1.0F, -1.0F, 1.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, -1.0F, 1.0F, 0.0F, 1.0F,
  };

  constexpr std::array<unsigned int, 6> quadIndices = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &m_quadVAO);
  glBindVertexArray(m_quadVAO);

  unsigned int VBO = 0U;
  unsigned int EBO = 0U;

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(quadVertices.size() * sizeof(float)), quadVertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(quadIndices.size() * sizeof(unsigned int)),
               quadIndices.data(), GL_STATIC_DRAW);

  // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
  glEnableVertexAttribArray(1);
  // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting, performance-no-int-to-ptr)

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
}

void EngineGL::setShadowMapSize(int size) {
  m_shadowMapsSize = std::clamp(size, MIN_SHADOW_MAP_SIZE, MAX_SHADOW_MAP_SIZE);
}

void EngineGL::setViewportSize(int width, int height) {
  m_viewport_width  = width;
  m_viewport_height = height;

  m_resource_manager.getCamera()->setViewportSize(m_viewport_width, m_viewport_height);
  m_scenePassFramebuffer.resize(m_viewport_width, m_viewport_height);
  m_resolveFramebuffer.resize(m_viewport_width, m_viewport_height);
  m_resolveFramebuffer.setTextureUnit(0, SCENE_TEXTURE_UNIT);
  glViewport(0, 0, m_viewport_width, m_viewport_height);
}

void EngineGL::setExposure(float exposure) {
  m_exposure = exposure;

  m_postProcessingProgram.bind();
  m_postProcessingProgram.setUniform1f("exposure", m_exposure);
}

void EngineGL::setToneMapping(ToneMapping toneMapping) {
  m_postProcessingProgram.bind();
  m_postProcessingProgram.setUniform1i("toneMapping", static_cast<int>(toneMapping));
}

void EngineGL::drawShadowMap2D() {
  m_shadowMap2D.initialize2DMap();
  m_shadowMap2D.bindFramebuffer();

  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  for(int i = 0; i < std::min(static_cast<int>(m_resource_manager.getDirectionalLights().size()), 2); ++i) {
    drawLightShadowMap2D(m_resource_manager.getDirectionalLights()[i]->getLightSpaceMatrix(), i, 0);
  }

  for(int i = 0; i < std::min(static_cast<int>(m_resource_manager.getSpotLights().size()), 2); ++i) {
    drawLightShadowMap2D(m_resource_manager.getSpotLights()[i]->getLightSpaceMatrix(), i, 1);
  }

  m_shadowMap2D.bindTexture(SHADOW_2D_MAP_TEXTURE_UNIT);
}

void EngineGL::drawLightShadowMap2D(const float* lightSpaceMatrix, int indexX, int indexY) {
  const int tileSize = static_cast<int>(m_shadowMapsSize * HALF);
  glViewport(tileSize * indexX, tileSize * indexY, tileSize, tileSize);

  m_shadowMap2DProgram.bind();
  m_shadowMap2DProgram.setUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

  for(const auto& dataBuffer : m_resource_manager.getObjectList()) {
    dataBuffer->bindVAO();
    m_shadowMap2DProgram.setUniformMatrix4fv("model", dataBuffer->getModelMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(dataBuffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }
}

void EngineGL::drawShadowMapCube() {
  m_shadowMapCube.initializeCubeMap();
  m_shadowMapCube.bindFramebuffer();

  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  if(!m_resource_manager.getPointLights().empty()) {
    drawPointShadowMap(m_resource_manager.getPointLights()[0].get());
  }

  m_shadowMapCube.bindTexture(SHADOW_CUBE_MAP_TEXTURE_UNIT);
}

void EngineGL::drawPointShadowMap(const PointLightGL* light) {
  glViewport(0, 0, m_shadowMapsSize, m_shadowMapsSize);

  m_shadowMapCubeProgram.bind();
  for(int i = 0; i < CUBE_MAP_FACE_COUNT; ++i) {
    m_shadowMapCubeProgram.setUniformMatrix4fv(("lightSpaceMatrices[" + std::to_string(i) + "]").c_str(),
                                               light->getLightSpaceMatrix(i));
  }
  m_shadowMapCubeProgram.setUniform3f("lightPos", light->getPosition().x, light->getPosition().y,
                                      light->getPosition().z);
  m_shadowMapCubeProgram.setUniform1f("farPlane", light->getFarPlane());

  for(const auto& dataBuffer : m_resource_manager.getObjectList()) {
    dataBuffer->bindVAO();
    m_shadowMapCubeProgram.setUniformMatrix4fv("model", dataBuffer->getModelMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(dataBuffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }

  m_scenePassProgram.bind();
  m_scenePassProgram.setUniform1f("shadowMapCubeFarPlane", light->getFarPlane());
}

void EngineGL::uploadDirectionalLightData() {
  for(int i = 0; i < m_resource_manager.getDirectionalLights().size(); ++i) {
    const auto& light = m_resource_manager.getDirectionalLights()[i];
    m_scenePassProgram.bind();
    m_scenePassProgram.setUniform3f(("dirLights[" + std::to_string(i) + "].direction").c_str(), light->getDirection().x,
                                    light->getDirection().y, light->getDirection().z);
    m_scenePassProgram.setUniform3f(("dirLights[" + std::to_string(i) + "].color").c_str(), light->getColor().x,
                                    light->getColor().y, light->getColor().z);
    m_scenePassProgram.setUniformMatrix4fv(("dirLights[" + std::to_string(i) + "].lightSpaceMatrix").c_str(),
                                           light->getLightSpaceMatrix());
  }
}

void EngineGL::uploadSpotLightData() {
  for(int i = 0; i < m_resource_manager.getSpotLights().size(); ++i) {
    const auto& light = m_resource_manager.getSpotLights()[i];
    m_scenePassProgram.bind();
    m_scenePassProgram.setUniform3f(("spotLights[" + std::to_string(i) + "].position").c_str(), light->getPosition().x,
                                    light->getPosition().y, light->getPosition().z);
    m_scenePassProgram.setUniform3f(("spotLights[" + std::to_string(i) + "].direction").c_str(),
                                    light->getDirection().x, light->getDirection().y, light->getDirection().z);
    m_scenePassProgram.setUniform1f(("spotLights[" + std::to_string(i) + "].innerCutOff").c_str(),
                                    light->getCosInnerCutoff());
    m_scenePassProgram.setUniform1f(("spotLights[" + std::to_string(i) + "].outerCutOff").c_str(),
                                    light->getCosOuterCutoff());
    m_scenePassProgram.setUniform3f(("spotLights[" + std::to_string(i) + "].color").c_str(), light->getColor().x,
                                    light->getColor().y, light->getColor().z);
    m_scenePassProgram.setUniformMatrix4fv(("spotLights[" + std::to_string(i) + "].lightSpaceMatrix").c_str(),
                                           light->getLightSpaceMatrix());
  }
}

void EngineGL::uploadPointLightData() {
  for(int i = 0; i < m_resource_manager.getPointLights().size(); ++i) {
    const auto& light = m_resource_manager.getPointLights()[i];
    m_scenePassProgram.bind();
    m_scenePassProgram.setUniform3f(("pointLights[" + std::to_string(i) + "].position").c_str(), light->getPosition().x,
                                    light->getPosition().y, light->getPosition().z);
    m_scenePassProgram.setUniform3f(("pointLights[" + std::to_string(i) + "].color").c_str(), light->getColor().x,
                                    light->getColor().y, light->getColor().z);
  }
}

void EngineGL::setupLights() {
  m_scenePassProgram.bind();
  m_scenePassProgram.setUniform1i("numDirLights", static_cast<int>(m_resource_manager.getDirectionalLights().size()));
  m_scenePassProgram.setUniform1i("numPointLights", static_cast<int>(m_resource_manager.getPointLights().size()));
  m_scenePassProgram.setUniform1i("numSpotLights", static_cast<int>(m_resource_manager.getSpotLights().size()));

  drawShadowMap2D();
  drawShadowMapCube();

  uploadDirectionalLightData();
  uploadSpotLightData();
  uploadPointLightData();
}

void EngineGL::configureShadersAndUniforms() {
  m_resolveFramebuffer.setTextureUnit(0, SCENE_TEXTURE_UNIT);

  m_postProcessingProgram.bind();
  m_postProcessingProgram.setUniform1i("screenTexture", SCENE_TEXTURE_UNIT);

  m_skyboxProgram.bind();
  m_resource_manager.getSkyboxTexture()->bind(SKYBOX_TEXTURE_UNIT);
  m_skyboxProgram.setUniform1i("skybox", SKYBOX_TEXTURE_UNIT);
  m_skyboxProgram.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);

  m_scenePassProgram.bind();
  m_scenePassProgram.setUniform1i("diffuseTexture", DIFFUSE_TEXTURE_UNIT);
  m_scenePassProgram.setUniform1i("normalTexture", NORMAL_TEXTURE_UNIT);
  m_scenePassProgram.setUniform1i("shadowMap2D", SHADOW_2D_MAP_TEXTURE_UNIT);
  m_scenePassProgram.setUniform1i("shadowMapCube", SHADOW_CUBE_MAP_TEXTURE_UNIT);
  m_scenePassProgram.bindUniformBlock("Camera", CAMERA_UBO_BINDING_POINT);
}

void EngineGL::renderScenePass() {
  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  m_scenePassProgram.bind();
  for(const auto& dataBuffer : m_resource_manager.getObjectList()) {
    dataBuffer->bindVAO();
    dataBuffer->bindMaterial();
    m_scenePassProgram.setUniformMatrix4fv("model", dataBuffer->getModelMatrix());
    m_scenePassProgram.setUniformMatrix3fv("normalMatrix", dataBuffer->getNormalMatrix());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(dataBuffer->getIndexCount()), GL_UNSIGNED_INT, nullptr);
  }
  glBindVertexArray(0);
}

void EngineGL::drawSkybox() {
  m_skyboxProgram.bind();

  glDepthFunc(GL_LEQUAL); // Set depth function for skybox rendering

  glBindVertexArray(m_quadVAO);
  glDrawElements(GL_TRIANGLES, QUAD_INDICES_COUNT, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  glDepthFunc(GL_LESS); // Reset depth function
}

void EngineGL::blitSceneToResolveFramebuffer() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_scenePassFramebuffer.getFramebufferID());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveFramebuffer.getFramebufferID());

  glBlitFramebuffer(0, 0, m_viewport_width, m_viewport_height, 0, 0, m_viewport_width, m_viewport_height,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void EngineGL::applyPostProcessing() {
  m_postProcessingProgram.bind();

  glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(m_quadVAO);
  glDrawElements(GL_TRIANGLES, QUAD_INDICES_COUNT, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
}

void EngineGL::renderFrame() {
  glfwPollEvents();
  processInput();

  m_scenePassFramebuffer.bind();
  renderScenePass();

  if(m_resource_manager.getSkyboxTexture() != nullptr) {
    drawSkybox();
  }

  blitSceneToResolveFramebuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
  applyPostProcessing();

  m_window.swapBuffers();
}

void EngineGL::render() {
  configureShadersAndUniforms();
  setupLights();

  glViewport(0, 0, m_viewport_width, m_viewport_height);

  while(!m_window.shouldClose()) {
    renderFrame();
    displayFPS(1.0F);
  }
}

void EngineGL::displayFPS(float interval) {
  static double lastTime   = 0.0F;
  static int    frameCount = 0;

  frameCount++;
  const double currentTime = glfwGetTime();
  if(currentTime - lastTime >= interval) {
    const double      fps   = static_cast<double>(frameCount) / (currentTime - lastTime);
    const std::string title = "OpenGL Engine - FPS: " + std::to_string(static_cast<int>(fps)) +
                              " - Window Size: " + std::to_string(m_viewport_width) + "x" +
                              std::to_string(m_viewport_height);
    glfwSetWindowTitle(m_window.gl_window(), title.c_str());
    lastTime   = currentTime;
    frameCount = 0;
  }
}

void EngineGL::processInput() {
  GLFWwindow* window = m_window.gl_window();
  if(window != nullptr) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      m_window.close();
    }
  }
}

void EngineGL::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto* engine = static_cast<EngineGL*>(glfwGetWindowUserPointer(window));
  if(engine != nullptr) {
    engine->setViewportSize(width, height);
  }
}

void EngineGL::scrollCallback(GLFWwindow* window, double /*xOffset*/, double yOffset) {
  auto* engine = static_cast<EngineGL*>(glfwGetWindowUserPointer(window));
  if(engine != nullptr) {
    engine->m_resource_manager.getCamera()->moveForward(static_cast<float>(yOffset * engine->cameraZoomSpeed()));
  }
}

void EngineGL::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
  static double lastX = xpos;
  static double lastY = ypos;

  auto* engine = static_cast<EngineGL*>(glfwGetWindowUserPointer(window));
  if(engine != nullptr) {
    GLFWwindow* gl_window = engine->m_window.gl_window();
    if(gl_window != nullptr) {
      if(glfwGetMouseButton(gl_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        const auto xOffset = static_cast<float>(lastX - xpos);
        const auto yOffset = static_cast<float>(ypos - lastY);
        if(glfwGetKey(gl_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
          const float camera_move_speed = engine->cameraMoveSpeed();
          engine->m_resource_manager.getCamera()->moveRight(xOffset * camera_move_speed);
          engine->m_resource_manager.getCamera()->moveUp(yOffset * camera_move_speed);
        } else {
          const float camera_rotate_speed = engine->cameraRotateSpeed();
          engine->m_resource_manager.getCamera()->rotate(xOffset * camera_rotate_speed, -yOffset * camera_rotate_speed);
        }
      }

      lastX = xpos;
      lastY = ypos;
    }
  }
}

EngineGL::~EngineGL() {
  glDeleteVertexArrays(1, &m_quadVAO);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
  m_window.close();
}

// GCOVR_EXCL_STOP