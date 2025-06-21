#ifndef QT_CONTROLLER_RENDERCONTROLLER_HPP
#define QT_CONTROLLER_RENDERCONTROLLER_HPP

#include <memory>
#include <qobject.h>

#include "Export/RenderExporter.hpp"
#include "Qt/View/RenderView.hpp"
#include "Qt/View/RenderWindowView.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/RenderTime.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"

class RenderController : public QObject {
  Q_OBJECT

public:
  explicit RenderController(RenderView* view, RenderSettings* render_settings, Scene* scene);

signals:
  void renderStarted(ImageProperties properties);
  void renderProgress(RenderStats stats);
  void updateRenderPreview(const double* framebuffer_data);
  void renderFinished(double elapsed_time);

private:
  // Slots
  void onImageWidthChanged(int width);
  void onImageHeightChanged(int height);
  void onChannelsChanged(int channels);
  void onSamplesPerPixelChanged(int samples);
  void onMaxBouncesChanged(int bounces);
  void onRenderModeChanged(const QString& mode);
  void onThreadCountChanged(int count);
  void onChunkSizeChanged(int size);
  void onPreviewStateChanged(bool enabled);
  void onRenderRequested();
  void onRenderCancelled();

  // Members
  std::unique_ptr<Renderer> m_renderer;

  RenderView*     m_view;
  RenderSettings* m_render_settings;

  RenderWindowView* m_render_window_view;

  bool m_displaying_preview = true;

  void updateViewFromModel();
  void connectSignals();
};

#endif // QT_CONTROLLER_RENDERCONTROLLER_HPP
