/**
 * @file WindowGL.hpp
 * @brief Header file for the WindowGL class, which manages an OpenGL window using GLFW.
 */
#ifndef GPU_OPENGL_WINDOWGL_HPP
#define GPU_OPENGL_WINDOWGL_HPP

// GCOVR_EXCL_START
#include <GLFW/glfw3.h>
#include <string>

/**
 * @class WindowGL
 * @brief Class representing an OpenGL window using GLFW.
 *
 * This class provides methods to create, manage, and interact with an OpenGL window.
 * It handles window creation, buffer swapping, and checking if the window should close.
 */
class WindowGL {
private:
  GLFWwindow* m_window = nullptr;

  void        initGLFW();
  static void setWindowHints();
  void        createWindow(int width, int height, const std::string& title);
  void        initGLEW();

public:
  /**
   * @brief Constructor for the WindowGL class.
   * @param width The width of the window.
   * @param height The height of the window.
   * @param title The title of the window.
   */
  WindowGL(int width, int height, const std::string& title);

  WindowGL(const WindowGL&)            = delete;
  WindowGL& operator=(const WindowGL&) = delete;
  WindowGL(WindowGL&&)                 = delete;
  WindowGL& operator=(WindowGL&&)      = delete;

  /**
   * @brief Checks if the window should close.
   * @return True if the window should close, false otherwise.
   */
  bool shouldClose() const;

  /**
   * @brief Closes the window.
   */
  void close() const;

  /**
   * @brief Swaps the front and back buffers of the window.
   * This is typically called after rendering to display the rendered content.
   */
  void swapBuffers() const;

  /**
   * @brief Gets the GLFW window pointer.
   * @return The GLFW window pointer.
   */
  GLFWwindow* gl_window() const { return m_window; }

  /**
   * @brief Destructor for the WindowGL class.
   * Cleans up the GLFW window resources.
   */
  ~WindowGL();
};

// GCOVR_EXCL_STOP
#endif // GPU_OPENGL_WINDOWGL_HPP