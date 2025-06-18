#include <memory>
#include <qobject.h>
#include <qstring.h>
#include <qthread.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <string>

#include "Core/MathConstants.hpp"
#include "Qt/Controller/RenderController.hpp"
#include "Qt/View/RenderView.hpp"
#include "Qt/View/RenderWindowView.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/RenderTime.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
RenderController::RenderController(RenderView* view, RenderSettings* render_settings, Scene* scene)
    : QObject(view), m_view(view), m_render_settings(render_settings),
      m_renderer(std::make_unique<Renderer>(scene, render_settings)),
      m_render_window_view(new RenderWindowView(m_renderer->getFramebuffer(), nullptr)) {
  connectSignals();
  updateViewFromModel();
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void RenderController::connectSignals() {
  connect(m_view, &RenderView::imageWidthChanged, this, &RenderController::onImageWidthChanged);
  connect(m_view, &RenderView::imageHeightChanged, this, &RenderController::onImageHeightChanged);
  connect(m_view, &RenderView::channelsChanged, this, &RenderController::onChannelsChanged);
  connect(m_view, &RenderView::samplesPerPixelChanged, this, &RenderController::onSamplesPerPixelChanged);
  connect(m_view, &RenderView::maxBouncesChanged, this, &RenderController::onMaxBouncesChanged);
  connect(m_view, &RenderView::renderModeChanged, this, &RenderController::onRenderModeChanged);
  connect(m_view, &RenderView::threadCountChanged, this, &RenderController::onThreadCountChanged);
  connect(m_view, &RenderView::chunkSizeChanged, this, &RenderController::onChunkSizeChanged);
  connect(m_view, &RenderView::renderRequested, this, &RenderController::onRenderRequested);

  m_renderer->getRenderProgressObserver().add([this](double progress) {
    if(m_displaying_preview && progress < 1.0) {
      const double* framebuffer_data = m_renderer->getPreviewImage(1 / progress);
      emit          updateRenderPreview(framebuffer_data);
    }
  });

  connect(this, &RenderController::renderStarted, m_render_window_view, &RenderWindowView::onRenderStarted);
  connect(this, &RenderController::renderProgress, m_render_window_view, &RenderWindowView::onRenderProgress);
  connect(this, &RenderController::updateRenderPreview, m_render_window_view, &RenderWindowView::onUpdateRenderPreview);
  connect(this, &RenderController::renderFinished, m_render_window_view, &RenderWindowView::onRenderFinished);

  connect(m_render_window_view, &RenderWindowView::renderCancelled, this, &RenderController::onRenderCancelled);
  connect(m_render_window_view, &RenderWindowView::previewStateChanged, this, &RenderController::onPreviewStateChanged);
}

void RenderController::updateViewFromModel() {
  m_view->setImageWidth(m_render_settings->getWidth());
  m_view->setImageHeight(m_render_settings->getHeight());
  m_view->setChannels(m_render_settings->getChannelCount());
  m_view->setSamplesPerPixel(m_render_settings->getSamplesPerPixel());
  m_view->setMaxBounces(m_render_settings->getMaxBounces());

  std::string render_mode_str;
  switch(m_render_settings->getRenderMode()) {
  case RenderMode::SINGLE_THREADED:
    render_mode_str = "Single Threaded";
    break;
  case RenderMode::MULTI_THREADED_CPU:
    render_mode_str = "Multi-Threaded CPU";
    break;
  case RenderMode::GPU_CUDA:
    render_mode_str = "GPU CUDA";
    break;
  default:
    render_mode_str = "Single Threaded"; // Default fallback
  }
  m_view->setRenderMode(QString::fromStdString(render_mode_str));

  m_view->setThreadCount(m_render_settings->getThreadCount());
  m_view->setChunkSize(m_render_settings->getChunkSize());
}

void RenderController::onPreviewStateChanged(bool enabled) { m_displaying_preview = enabled; }

void RenderController::onImageWidthChanged(int width) { m_render_settings->setWidth(width); }

void RenderController::onImageHeightChanged(int height) { m_render_settings->setHeight(height); }

void RenderController::onChannelsChanged(int channels) { m_render_settings->setChannelCount(channels); }

void RenderController::onSamplesPerPixelChanged(int samples) { m_render_settings->setSamplesPerPixel(samples); }

void RenderController::onMaxBouncesChanged(int bounces) { m_render_settings->setMaxBounces(bounces); }

void RenderController::onRenderModeChanged(const QString& mode) {
  RenderMode render_mode = RenderMode::SINGLE_THREADED;
  if(mode.contains("Single")) {
    render_mode = RenderMode::SINGLE_THREADED;
  } else if(mode.contains("CPU")) {
    render_mode = RenderMode::MULTI_THREADED_CPU;
  } else if(mode.contains("CUDA")) {
    render_mode = RenderMode::GPU_CUDA;
  }
  m_render_settings->setRenderMode(render_mode);
}

void RenderController::onThreadCountChanged(int count) { m_render_settings->setThreadCount(count); }

void RenderController::onChunkSizeChanged(int size) { m_render_settings->setChunkSize(size); }

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void RenderController::onRenderRequested() {
  emit renderStarted(m_render_settings->getImageProperties());

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [this]() { emit renderProgress(m_renderer->getRenderTime()->getRenderStats()); });
  timer->start(SECOND_IN_MILLISECONDS);

  bool* success = new bool(false);

  QThread* thread = QThread::create([this, success]() { *success = m_renderer->renderFrame(); });

  connect(thread, &QThread::finished, this, [this, timer, thread, success]() {
    timer->stop();
    timer->deleteLater(); // OK ici : suppression manuelle immédiate

    if(*success) {
      emit renderFinished(m_renderer->getRenderTime()->getRenderStats().elapsed_time);
    }

    delete success;
    thread->deleteLater();
  });

  thread->start();
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void RenderController::onRenderCancelled() { m_renderer->requestStop(); }