// GCOVR_EXCL_START
#include <GL/gl.h>
#include <GL/glext.h>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Core/Config.hpp"
#include "GPU/OpenGL/ShadersGL.hpp"

ShadersGL::ShadersGL() { initializeOpenGLFunctions(); }

unsigned int ShadersGL::compileShader(const std::string& source, unsigned int type) {
  const unsigned int shader       = glCreateShader(type);
  const char*        shaderSource = source.c_str();
  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  if(checkCompileErrorsShader(shader) == 0) {
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

int ShadersGL::checkCompileErrorsShader(unsigned int shader) {
  int                                    success = 0;
  std::array<char, ERROR_MESSAGE_LENGTH> infoLog{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(success == 0) {
    glGetShaderInfoLog(shader, ERROR_MESSAGE_LENGTH, nullptr, infoLog.data());
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog.data() << '\n';
  }
  return success;
}

int ShadersGL::checkCompileErrorsProgram(unsigned int program) {
  int                                    success = 0;
  std::array<char, ERROR_MESSAGE_LENGTH> infoLog{};
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(success == 0) {
    glGetProgramInfoLog(program, ERROR_MESSAGE_LENGTH, nullptr, infoLog.data());
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << '\n';
  }
  return success;
}

std::string ShadersGL::readShaderFromFile(const char* filePath) {
  std::ifstream shaderFile(filePath);
  if(!shaderFile) {
    std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << filePath << '\n';
    return "";
  }
  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  shaderFile.close();
  return shaderStream.str();
}

void ShadersGL::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
  compileShadersFromSources({{GL_VERTEX_SHADER, vertexShaderPath}, {GL_FRAGMENT_SHADER, fragmentShaderPath}});
}

void ShadersGL::loadShaders(const char* vertexShaderPath, const char* geometryShaderPath,
                            const char* fragmentShaderPath) {
  compileShadersFromSources({{GL_VERTEX_SHADER, vertexShaderPath},
                             {GL_GEOMETRY_SHADER, geometryShaderPath},
                             {GL_FRAGMENT_SHADER, fragmentShaderPath}});
}

void ShadersGL::compileShadersFromSources(const std::vector<std::pair<GLenum, const char*>>& shaders) {
  std::vector<unsigned int> compiledShaders;

  for(const auto& [type, path] : shaders) {
    const std::string  source = readShaderFromFile(path);
    const unsigned int shader = compileShader(source, type);
    if(shader == 0) {
      std::cerr << "Could not compile shader from file: " << path << '\n';
      for(const unsigned int s : compiledShaders) {
        glDeleteShader(s);
      }
      return;
    }
    compiledShaders.push_back(shader);
  }

  m_program = glCreateProgram();
  for(const unsigned int shader : compiledShaders) {
    glAttachShader(m_program, shader);
  }

  glLinkProgram(m_program);

  for(const unsigned int shader : compiledShaders) {
    glDeleteShader(shader);
  }

  if(checkCompileErrorsProgram(m_program) == 0) {
    std::cerr << "Could not link shader program.\n";
    glDeleteProgram(m_program);
    m_program = 0;
    return;
  }

  std::cout << "Shaders loaded successfully: ";
  for(const auto& [_, path] : shaders) {
    std::cout << path << " ";
  }
  std::cout << '\n';
}

void ShadersGL::bind() {
  if(m_program != 0) {
    glUseProgram(m_program);
  } else {
    std::cerr << "ERROR::SHADER::PROGRAM_NOT_LOADED\n";
  }
}

void ShadersGL::bindUniformBlock(const char* blockName, unsigned int bindingPoint) {
  const unsigned int blockIndex = glGetUniformBlockIndex(m_program, blockName);
  if(blockIndex == -1) {
    std::cerr << "Warning: uniform block '" << blockName << "' doesn't exist!" << '\n';
    return;
  }
  glUniformBlockBinding(m_program, blockIndex, bindingPoint);
}

int ShadersGL::getUniformLocation(const char* name) {
  auto it = m_uniformLocationCache.find(name);
  if(it != m_uniformLocationCache.end()) {
    return it->second;
  }

  const int location = glGetUniformLocation(m_program, name);

  if(location == -1) {
    std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << '\n';
  }
  m_uniformLocationCache[name] = location;

  return location;
}

ShadersGL::~ShadersGL() {
  glUseProgram(0);
  if(m_program != 0) {
    glDeleteProgram(m_program);
  }
}

// GCOVR_EXCL_STOP