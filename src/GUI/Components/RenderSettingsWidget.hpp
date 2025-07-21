/**
 * @file RenderSettingsWidget.hpp
 * @brief Header file for the RenderSettingsWidget class, which provides a user interface for configuring
 *        rendering settings.
 */
#ifndef GUI_WIDGETS_RENDERSETTINGSWIDGET_HPP
#define GUI_WIDGETS_RENDERSETTINGSWIDGET_HPP

#include <QWidget>

#include "Core/CommonTypes.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderSettings.hpp"

class Scene;

namespace Ui {
  class RenderSettingsWidget;
}

class RenderWindow;

/**
 * @class RenderSettingsWidget
 * @brief A widget for configuring rendering settings.
 * 
 * This class provides a user interface for adjusting various rendering settings such as image dimensions,
 * format, samples per pixel, maximum bounces, render mode, thread count, and chunk size.
 * It allows users to start rendering and provides feedback on the rendering progress.
 */
class RenderSettingsWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the RenderSettingsWidget class.
   * @param parent The parent widget for this render settings widget.
   */
  explicit RenderSettingsWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the scene to be rendered.
   * @param scene The scene to be rendered.
   */
  void setScene(Scene* scene);
  
  /**
   * @brief Updates the widget to reflect the current render settings.
   */
  void updateWidget();

  ~RenderSettingsWidget() override; ///< Default destructor for the RenderSettingsWidget class.

signals:
  void renderStarted(ImageProperties properties);
  void renderProgress(RenderStats stats);
  void renderFinished(double elapsed_time);

private slots:
  void onWidthChanged(int width);
  void onHeightChanged(int height);
  void onFormatChanged(const QString& format);
  void onSamplesChanged(int samples);
  void onBouncesChanged(int bounces);
  void onRenderModeChanged(const QString& mode);
  void onThreadCountChanged(int count);
  void onChunkSizeChanged(int size);
  void onRenderButtonClicked();

private:
  Ui::RenderSettingsWidget* ui;
  RenderWindow*             m_render_window = nullptr;

  RenderSettings            m_render_settings;
  std::unique_ptr<Renderer> m_renderer;

  void openRenderWindow();
};

#endif // GUI_WIDGETS_RENDERSETTINGSWIDGET_HPP
