/**
 * @file RenderWindow.hpp
 * @brief Header file for the RenderWindow class, which provides a user interface for rendering and exporting images.
 */
#ifndef GUI_COMPONENTS_RENDERWINDOW_HPP
#define GUI_COMPONENTS_RENDERWINDOW_HPP

#include <QPixmap>
#include <QWidget>

#include "Core/CommonTypes.hpp"
#include "Core/Framebuffer.hpp"
#include "Export/RenderExporter.hpp"
#include "Rendering/RenderTime.hpp"

namespace Ui {
class RenderWindow;
}

/**
 * @class RenderWindow
 * @brief A widget that provides a user interface for rendering and exporting images.
 *
 * This class allows users to view the rendered image, track rendering progress, and export the image in various
 * formats. It also provides functionality to update the image based on the current framebuffer.
 */
class RenderWindow : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the RenderWindow class.
   * @param parent The parent widget for this render window.
   */
  explicit RenderWindow(QWidget* parent = nullptr);

  /**
   * @brief Sets the framebuffer for this render window.
   * @param framebuffer Pointer to the Framebuffer that holds the rendered image.
   */
  void setFramebuffer(Framebuffer* framebuffer);

  /**
   * @brief Updates the image to export based on the current framebuffer.
   */
  ~RenderWindow();

public slots:
  void onRenderStarted(ImageProperties properties);
  void onRenderProgress(RenderStats stats);
  void onRenderFinished(double elapsed_time);

signals:
  void aboutToClose();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void closeEvent(QCloseEvent* event) override;

private:
  Ui::RenderWindow* ui;
  Framebuffer*      m_framebuffer = nullptr;

  std::unique_ptr<RenderExporter> m_exporter;
  bool                            m_render_finished = false;

  ImageProperties m_render_image_properties;
  QPixmap         m_render_image;

  void updateImageToExport();

  static QString        FormatSecondsToString(double seconds);
  static QImage::Format imageFormatFromChannels(int channels);
};

#endif // GUI_COMPONENTS_RENDERWINDOW_HPP