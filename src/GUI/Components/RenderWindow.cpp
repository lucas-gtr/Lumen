// GCOVR_EXCL_START
#include <QResizeEvent>
#include <QThread>
#include <cstdint>

#include "Core/Framebuffer.hpp"
#include "RenderWindow.hpp"
#include "ui_RenderWindow.h"

RenderWindow::RenderWindow(QWidget* parent) : QWidget(parent), ui(new Ui::RenderWindow) {
  ui->setupUi(this);

  connect(ui->exportWidget, &ExportWidget::exportPreviewChanged, this, &RenderWindow::updateImageToExport);
}

void RenderWindow::setFramebuffer(Framebuffer* framebuffer) {
  m_framebuffer = framebuffer;
  m_exporter    = std::make_unique<RenderExporter>(m_framebuffer);
  ui->exportWidget->setExporter(m_exporter.get());
}

void RenderWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  if(m_render_image.isNull()) {
    return;
  }
  ui->imageLabel->setPixmap(
      m_render_image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void RenderWindow::closeEvent(QCloseEvent* event) {
  emit aboutToClose();
  QWidget::closeEvent(event);

  m_render_image = QPixmap(); // Clear the image to free resources
  ui->imageLabel->clear();

  ui->imageLabel->resize(0, 0);
}

void RenderWindow::onRenderStarted(Resolution resolution) {
  m_render_finished  = false;
  m_image_resolution = resolution;

  ui->exportWidget->setExportReady(false);
  ui->statusLabel->setText("Estimating render time...");
  ui->renderProgress->setValue(0);

  const QImage::Format format = imageFormatFromChannels(m_exporter->getChannelCount());
  QImage               image(resolution.width, resolution.height, format);
  image.fill(Qt::black);

  m_render_image = QPixmap::fromImage(image);

  this->resize(this->minimumSizeHint());
  ui->imageLabel->resize(resolution.width, resolution.height);

  ui->imageLabel->setPixmap(m_render_image);
}

void RenderWindow::onRenderProgress(RenderStats stats) {
  const double progress = static_cast<double>(stats.current_chunk) / stats.total_chunks * 100.0;
  ui->renderProgress->setValue(static_cast<int>(progress));

  if(progress == 0.0) {
    return;
  }
  const QString status_text = QString("Rendering: %1/%2 chunks - Elapsed: %3, Remaining: %4")
                                  .arg(stats.current_chunk)
                                  .arg(stats.total_chunks)
                                  .arg(FormatSecondsToString(stats.elapsed_time))
                                  .arg(FormatSecondsToString(stats.remaining_time));
  ui->statusLabel->setText(status_text);
}

void RenderWindow::onRenderFinished(double elapsed_time) {
  m_render_finished = true;

  ui->statusLabel->setText("Rendering completed in " + FormatSecondsToString(elapsed_time));
  ui->renderProgress->setValue(100); // NOLINT

  updateImageToExport();

  ui->exportWidget->setExportReady(true);
}

void RenderWindow::updateImageToExport() {
  if(!m_render_finished) {
    return;
  }
  m_exporter->updateImageToExport();
  const QImage::Format format = imageFormatFromChannels(m_exporter->getChannelCount());
  QImage               image(m_image_resolution.width, m_image_resolution.height, format);

  const unsigned char* image_data = m_exporter->getImageToExport();
  const int total_size = m_image_resolution.width * m_image_resolution.height * m_exporter->getChannelCount();

  uint8_t* dst = image.bits();
  for(int i = 0; i < total_size; ++i) {
    dst[i] = static_cast<uint8_t>(image_data[i]);
  }

  m_render_image = QPixmap::fromImage(image);
  ui->imageLabel->setPixmap(
      m_render_image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QString RenderWindow::FormatSecondsToString(double seconds) {
  const int hours   = static_cast<int>(seconds / 3600);
  const int minutes = static_cast<int>((seconds - hours * 3600) / 60);
  const int secs    = static_cast<int>(seconds) % 60;

  return QString("%1h %2m %3s").arg(hours).arg(minutes).arg(secs);
}

QImage::Format RenderWindow::imageFormatFromChannels(int channels) {
  switch(channels) {
  case 1:
    return QImage::Format_Grayscale8;
  case 3:
    return QImage::Format_RGB888;
  case 4:
    return QImage::Format_RGBA8888;
  default:
    return QImage::Format_Invalid;
  }
}

RenderWindow::~RenderWindow() { delete ui; }
// GCOVR_EXCL_STOP