// GCOVR_EXCL_START
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Core/Config.hpp"
#include "GPU/OpenGL/WindowGL.hpp"

WindowGL::WindowGL(int width, int height, const std::string& title) {
  initGLFW();
  setWindowHints();
  createWindow(width, height, title);
  initGLEW();
}

void WindowGL::initGLFW() {
  if(glfwInit() == 0) {
    std::cerr << "Failed to initialize GLFW\n";
    m_window = nullptr;
  }
}

void WindowGL::setWindowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void WindowGL::createWindow(int width, int height, const std::string& title) {
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if(m_window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(m_window);
  glViewport(0, 0, width, height);
}

void WindowGL::initGLEW() {
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_window = nullptr;
  }
}

void WindowGL::close() const {
  if(m_window != nullptr) {
    glfwSetWindowShouldClose(m_window, 1);
  }
}

bool WindowGL::shouldClose() const { return m_window == nullptr || glfwWindowShouldClose(m_window) != 0; }

void WindowGL::swapBuffers() const {
  if(m_window != nullptr) {
    glfwSwapBuffers(m_window);
  }
}

WindowGL::~WindowGL() {
  if(m_window != nullptr) {
    glfwDestroyWindow(m_window);
  }
  // glfwTerminate();
}

// GCOVR_EXCL_STOP