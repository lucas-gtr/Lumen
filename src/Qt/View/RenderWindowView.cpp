#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qprogressbar.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <utility>

#include "Core/ColorUtils.hpp"
#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Core/Framebuffer.hpp"
#include "Qt/Controller/ExportController.hpp"
#include "Qt/View/ExportView.hpp"
#include "Qt/View/RenderWindowView.hpp"
#include "Rendering/RenderTime.hpp"

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
RenderWindowView::RenderWindowView(Framebuffer* framebuffer, QWidget* parent)
    : QWidget(parent), m_export_view(new ExportView(this)),
      m_export_controller(new ExportController(m_export_view, framebuffer)) {
  setupUI();
  connectSignals();
}
void RenderWindowView::setupUI() {
  m_image_label = new QLabel(this);
  m_image_label->setAlignment(Qt::AlignCenter);

  m_status_label     = new QLabel("Waiting...", this);
  m_preview_checkbox = new QCheckBox("Display Preview", this);
  m_preview_checkbox->setChecked(true);

  auto* status_layout = new QHBoxLayout;
  status_layout->addWidget(m_status_label);
  status_layout->addStretch();
  status_layout->addWidget(m_preview_checkbox);

  m_progress_bar = new QProgressBar(this);
  m_progress_bar->setRange(0, RENDER_PROGRESS_BAR_MAX_PERCENT);
  m_progress_bar->setValue(0);
  m_progress_bar->setTextVisible(true);

  auto* left_layout = new QVBoxLayout;
  left_layout->addWidget(m_image_label);
  left_layout->addLayout(status_layout);
  left_layout->addWidget(m_progress_bar);

  auto* right_layout = new QVBoxLayout;
  right_layout->addWidget(m_export_view);
  right_layout->addStretch();

  auto* main_layout = new QHBoxLayout;
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);

  setLayout(main_layout);
  setWindowTitle("Render Window");
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void RenderWindowView::connectSignals() {
  connect(m_preview_checkbox, &QCheckBox::stateChanged, this, &RenderWindowView::previewStateChanged);
  connect(m_export_controller, &ExportController::exportImageChanged, this, &RenderWindowView::onExportImageChanged);

  connect(this, &RenderWindowView::renderStarted, m_export_view, &ExportView::onRenderStarted);
  connect(this, &RenderWindowView::renderFinished, m_export_view, &ExportView::onRenderFinished);
  connect(this, &RenderWindowView::renderFinished, m_export_controller, &ExportController::onRenderFinished);
}

void RenderWindowView::onRenderStarted(ImageProperties properties) {
  m_render_finished = false;

  m_image_width   = properties.width;
  m_image_height  = properties.height;
  m_channel_count = properties.channels;
  m_image_label->setFixedSize(m_image_width, m_image_height);
  clearImage();

  m_progress_bar->setValue(0);
  m_status_label->setText("Estimating render time...");

  emit renderStarted();
  show();
}

void RenderWindowView::onRenderProgress(RenderStats stats) {
  const double progress = static_cast<double>(stats.current_chunk) / stats.total_chunks * 100.0;
  m_progress_bar->setValue(static_cast<int>(progress));

  const QString statusText = QString("Rendering: %1/%2 chunks - Elapsed: %3, Remaining: %4")
                                 .arg(stats.current_chunk)
                                 .arg(stats.total_chunks)
                                 .arg(formatSecondsToString(stats.elapsed_time))
                                 .arg(formatSecondsToString(stats.remaining_time));
  m_status_label->setText(statusText);
}

void RenderWindowView::onUpdateRenderPreview(const double* framebuffer_data) { updateImage(framebuffer_data); }

void RenderWindowView::onRenderFinished(double elapsed_time) {
  m_render_finished = true;

  m_progress_bar->setValue(RENDER_PROGRESS_BAR_MAX_PERCENT);
  m_status_label->setText("Rendering completed in " + formatSecondsToString(elapsed_time));
  emit renderFinished();
}

void RenderWindowView::onExportImageChanged(const unsigned char* image_data) {
  if(!m_render_finished) {
    return;
  }
  updateImage(image_data);
}

QImage RenderWindowView::createImage() const {
  QImage::Format format = QImage::Format_Invalid;
  switch(m_channel_count) {
  case 1:
    format = QImage::Format_Grayscale8;
    break;
  case 3:
    format = QImage::Format_RGB888;
    break;
  case 4:
    format = QImage::Format_RGBA8888;
    break;
  default:
    break;
  }

  return {m_image_width, m_image_height, format};
}

void RenderWindowView::clearImage() {
  QImage image = createImage();

  image.fill(Qt::black);
  m_image_label->setPixmap(QPixmap::fromImage(std::move(image)));
}

void RenderWindowView::updateImage(const double* image_data) {
  QImage    image      = createImage();
  const int total_size = m_image_width * m_image_height * m_channel_count;

  uint8_t* dst = image.bits();
  for(int i = 0; i < total_size; ++i) {
    const double value = std::clamp(image_data[i], 0.0, 1.0);
    dst[i]             = static_cast<uint8_t>(value * NORMALIZED_TO_COLOR8);
  }

  m_image_label->setPixmap(QPixmap::fromImage(std::move(image)));
}

void RenderWindowView::updateImage(const unsigned char* image_data) {
  QImage image = createImage();
  std::memcpy(image.bits(), image_data,
              static_cast<size_t>(m_image_width) * static_cast<size_t>(m_image_height) *
                  static_cast<size_t>(m_channel_count));

  m_image_label->setPixmap(QPixmap::fromImage(std::move(image)));
}

QString RenderWindowView::formatSecondsToString(double seconds) {
  const int hours   = static_cast<int>(seconds / 3600);
  const int minutes = static_cast<int>((seconds - hours * 3600) / 60);
  const int secs    = static_cast<int>(seconds - hours * 3600 - minutes * 60);

  return QString("%1h %2m %3s").arg(hours).arg(minutes).arg(secs);
}

void RenderWindowView::closeEvent(QCloseEvent* event) {
  QWidget::closeEvent(event);
  emit renderCancelled();
}