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
  const unsigned int shader        = glCreateShader(type);
  const char*        shader_source = source.c_str();
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  if(checkCompileErrorsShader(shader) == 0) {
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

int ShadersGL::checkCompileErrorsShader(unsigned int shader) {
  int                                    success = 0;
  std::array<char, ERROR_MESSAGE_LENGTH> info_log{};
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(success == 0) {
    glGetShaderInfoLog(shader, ERROR_MESSAGE_LENGTH, nullptr, info_log.data());
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log.data() << '\n';
  }
  return success;
}

int ShadersGL::checkCompileErrorsProgram(unsigned int program) {
  int                                    success = 0;
  std::array<char, ERROR_MESSAGE_LENGTH> info_log{};
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(success == 0) {
    glGetProgramInfoLog(program, ERROR_MESSAGE_LENGTH, nullptr, info_log.data());
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log.data() << '\n';
  }
  return success;
}

std::string ShadersGL::ReadShaderFromFile(const char* file_path) {
  std::ifstream shader_file(file_path);
  if(!shader_file) {
    std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << file_path << '\n';
    return "";
  }
  std::stringstream shader_stream;
  shader_stream << shader_file.rdbuf();
  shader_file.close();
  return shader_stream.str();
}

void ShadersGL::loadShaders(const char* vertex_shader_path, const char* fragment_shader_path) {
  compileShadersFromSources({{GL_VERTEX_SHADER, vertex_shader_path}, {GL_FRAGMENT_SHADER, fragment_shader_path}});
}

void ShadersGL::loadShaders(const char* vertex_shader_path, const char* geometry_shader_path,
                            const char* fragment_shader_path) {
  compileShadersFromSources({{GL_VERTEX_SHADER, vertex_shader_path},
                             {GL_GEOMETRY_SHADER, geometry_shader_path},
                             {GL_FRAGMENT_SHADER, fragment_shader_path}});
}

void ShadersGL::compileShadersFromSources(const std::vector<std::pair<GLenum, const char*>>& shaders) {
  std::vector<unsigned int> compiled_shaders;

  for(const auto& [type, path] : shaders) {
    const std::string  source = ReadShaderFromFile(path);
    const unsigned int shader = compileShader(source, type);
    if(shader == 0) {
      std::cerr << "Could not compile shader from file: " << path << '\n';
      for(const unsigned int s : compiled_shaders) {
        glDeleteShader(s);
      }
      return;
    }
    compiled_shaders.push_back(shader);
  }

  m_program = glCreateProgram();
  for(const unsigned int shader : compiled_shaders) {
    glAttachShader(m_program, shader);
  }

  glLinkProgram(m_program);

  for(const unsigned int shader : compiled_shaders) {
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

void ShadersGL::bindUniformBlock(const char* block_name, unsigned int binding_point) {
  const unsigned int block_index = glGetUniformBlockIndex(m_program, block_name);
  if(block_index == -1) {
    std::cerr << "Warning: uniform block '" << block_name << "' doesn't exist!" << '\n';
    return;
  }
  glUniformBlockBinding(m_program, block_index, binding_point);
}

int ShadersGL::getUniformLocation(const char* name) {
  auto it = m_uniform_location_cache.find(name);
  if(it != m_uniform_location_cache.end()) {
    return it->second;
  }

  const int location = glGetUniformLocation(m_program, name);

  if(location == -1) {
    std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << '\n';
  }
  m_uniform_location_cache[name] = location;

  return location;
}

ShadersGL::~ShadersGL() {
  glUseProgram(0);
  if(m_program != 0) {
    glDeleteProgram(m_program);
  }
}

// GCOVR_EXCL_STOP