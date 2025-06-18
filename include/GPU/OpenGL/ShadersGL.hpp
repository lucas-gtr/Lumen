/**
 * @file ShadersGL.hpp
 * @brief Header file for the ShadersGL class, which manages OpenGL shaders.
 */
#ifndef GPU_OPENGL_SHADERSGL_HPP
#define GPU_OPENGL_SHADERSGL_HPP

// GCOVR_EXCL_START

#include <qopenglfunctions_3_3_core.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @class ShadersGL
 * @brief A class that manages OpenGL shaders, including loading, compiling, and linking.
 *
 * This class provides methods to load shaders from source files, compile them, and link them into a shader program.
 * It also provides methods to set uniform variables in the shader program.
 * It caches uniform locations to avoid redundant lookups.
 */
class ShadersGL : protected QOpenGLFunctions_3_3_Core {
private:
  unsigned int                         m_program = 0U;
  std::unordered_map<const char*, int> m_uniformLocationCache;

  unsigned int       compileShader(const std::string& source, unsigned int type);
  int                checkCompileErrorsShader(unsigned int shader);
  int                checkCompileErrorsProgram(unsigned int program);
  static std::string readShaderFromFile(const char* filePath);

  void compileShadersFromSources(const std::vector<std::pair<GLenum, const char*>>& shaders);

public:
  /**
   * @brief Default constructor for ShadersGL.
   */
  ShadersGL();

  ShadersGL(const ShadersGL&)            = delete;
  ShadersGL& operator=(const ShadersGL&) = delete;
  ShadersGL(ShadersGL&&)                 = delete;
  ShadersGL& operator=(ShadersGL&&)      = delete;

  /**
   * @brief Loads and compiles shaders from the specified source files.
   * @param vertexShaderPath Path to the vertex shader source file.
   * @param fragmentShaderPath Path to the fragment shader source file.
   */
  void loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath);

  /**
   * @brief Loads and compiles shaders from the specified source files, including a geometry shader.
   * @param vertexShaderPath Path to the vertex shader source file.
   * @param geometryShaderPath Path to the geometry shader source file.
   * @param fragmentShaderPath Path to the fragment shader source file.
   */
  void loadShaders(const char* vertexShaderPath, const char* geometryShaderPath, const char* fragmentShaderPath);

  /**
   * @brief Binds the shader program for use in rendering.
   */
  void bind();

  /**
   * @brief Binds a uniform block to a specified binding point.
   * @param blockName The name of the uniform block.
   * @param bindingPoint The binding point to which the block should be bound.
   */
  void bindUniformBlock(const char* blockName, unsigned int bindingPoint);

  /**
   * @brief Gets the location of a uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @return The location of the uniform variable.
   */
  int getUniformLocation(const char* name);

  /**
   * @brief Sets an int uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param value The value to set for the uniform variable.
   */
  void setUniform1i(const char* name, int value) { glUniform1i(getUniformLocation(name), value); }

  /**
   * @brief Sets a float uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param value The value to set for the uniform variable.
   */
  void setUniform1f(const char* name, float value) { glUniform1f(getUniformLocation(name), value); }

  /**
   * @brief Sets a 3D vector of float uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param v0 The first component of the vector.
   * @param v1 The second component of the vector.
   * @param v2 The third component of the vector.
   */
  void setUniform3f(const char* name, float v0, float v1, float v2) {
    glUniform3f(getUniformLocation(name), v0, v1, v2);
  }

  /**
   * @brief Sets a 4D vector of float uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param v0 The first component of the vector.
   * @param v1 The second component of the vector.
   * @param v2 The third component of the vector.
   * @param v3 The fourth component of the vector.
   */
  void setUniform4f(const char* name, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
  }

  /**
   * @brief Sets a 3x3 matrix of float uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param value Pointer to the array of float values representing the matrix.
   * @note The matrix is expected to be in row-major order because we transpose it when uploading to OpenGL.
   */
  void setUniformMatrix3fv(const char* name, const float* value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_TRUE, value);
  }

  /**
   * @brief Sets a 4x4 matrix of float uniform variable in the shader program.
   * @param name The name of the uniform variable.
   * @param value Pointer to the array of float values representing the matrix.
   * @note The matrix is expected to be in row-major order because we transpose it when uploading to OpenGL.
   */
  void setUniformMatrix4fv(const char* name, const float* value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, value);
  }

  /**
   * @brief Destructor for ShadersGL.
   * Cleans up any resources used by the shader program.
   */
  ~ShadersGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_SHADERSGL_HPP