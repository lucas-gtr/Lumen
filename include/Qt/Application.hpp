/**
 * @file Application.hpp
 * @brief Header file for the Application class, which manages the Qt application lifecycle.
 */
#ifndef QT_APPLICATION_HPP
#define QT_APPLICATION_HPP

#include <memory>
#include <qapplication.h>

#include "Qt/MainWindow.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Scene/Scene.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

/**
 * @class Application
 * @brief The main application class that initializes and runs the Qt application.
 *
 * This class is responsible for setting up the main window, creating the default scene,
 * and managing the render settings, texture manager, and material manager.
 */
class Application : public QApplication {
private:
  std::unique_ptr<Scene>           m_scene;
  std::unique_ptr<RenderSettings>  m_render_settings;
  std::unique_ptr<TextureManager>  m_texture_manager;
  std::unique_ptr<MaterialManager> m_material_manager;

  MainWindow* m_window = nullptr;
  void        createDefaultScene();
  static void setStylesheet();

public:
  /**
   * @brief Constructor for the Application class.
   * * Initializes the Qt application with command line arguments and sets up the main window.
   * @param argc Reference to the argument count.
   * @param argv Array of command line arguments.
   */
  Application(int& argc, char** argv);

  Application(const Application&)            = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&)                 = delete;
  Application& operator=(Application&&)      = delete;

  /**
   * @brief Runs the application event loop.
   * @return The exit code of the application.
   */
  int run();

  /**
   * @brief Default destructor for the Application class.
   */
  ~Application() override = default;
};

#endif // QT_APPLICATION_HPP