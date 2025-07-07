#include <algorithm>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qglobal.h>
#include <qgroupbox.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <thread>

#include "Core/Config.hpp"
#include "Qt/View/RenderView.hpp"

RenderView::RenderView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void RenderView::setupUI() {
  auto* render_group = new QGroupBox("Render settings", this);
  render_group->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");

  m_image_width_spin = new QSpinBox(this);
  m_image_width_spin->setRange(1, MAX_IMAGE_WIDTH);
  m_image_width_spin->setSuffix(" px");

  m_image_height_spin = new QSpinBox(this);
  m_image_height_spin->setRange(1, MAX_IMAGE_HEIGHT);
  m_image_height_spin->setSuffix(" px");

  m_channels_combo = new QComboBox(this);
  m_channels_combo->addItems({"1", "3", "4"});

  m_samples_spin = new QSpinBox(this);
  m_samples_spin->setRange(1, MAX_SAMPLES_PER_PIXEL);

  m_bounces_spin = new QSpinBox(this);
  m_bounces_spin->setRange(1, MAX_BOUNCES);

  m_render_mode_combo = new QComboBox(this);
  m_render_mode_combo->addItems({"Single-threaded CPU", "Multi-threaded CPU", "Multi-threaded GPU (CUDA)"});

  m_thread_count_spin = new QSpinBox(this);
  m_thread_count_spin->setRange(
      1, static_cast<int>(std::max(1U, std::thread::hardware_concurrency() - THREADS_TO_KEEP_FREE)));

  m_chunk_size_spin = new QSpinBox(this);
  m_chunk_size_spin->setRange(1, MAX_CHUNK_SIZE);

  m_gpu_options_widget = new QWidget(this);
  auto* gpu_layout     = new QFormLayout(m_gpu_options_widget);
  gpu_layout->setContentsMargins(0, 0, 0, 0);
  gpu_layout->addRow("Thread count", m_thread_count_spin);
  gpu_layout->addRow("Chunk size", m_chunk_size_spin);
  m_gpu_options_widget->hide();

  m_render_button = new QPushButton("Render", this);

  auto* form_layout = new QFormLayout;
  form_layout->addRow("Image width", m_image_width_spin);
  form_layout->addRow("Image height", m_image_height_spin);
  form_layout->addRow("Channels", m_channels_combo);
  form_layout->addRow("Samples per pixel", m_samples_spin);
  form_layout->addRow("Max bounces", m_bounces_spin);
  form_layout->addRow("Render mode", m_render_mode_combo);
  form_layout->addRow(m_gpu_options_widget);
  form_layout->addRow(m_render_button);

  render_group->setLayout(form_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(render_group);
  main_layout->addStretch();
  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void RenderView::connectSignals() {
  connect(m_image_width_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::imageWidthChanged);
  connect(m_image_height_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::imageHeightChanged);
  connect(m_channels_combo, &QComboBox::currentTextChanged, this,
          [this](const QString& text) { channelsChanged(text.toInt()); });
  connect(m_samples_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::samplesPerPixelChanged);
  connect(m_bounces_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::maxBouncesChanged);
  connect(m_render_mode_combo, &QComboBox::currentTextChanged, this, &RenderView::onRenderModeChanged);
  connect(m_thread_count_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::threadCountChanged);
  connect(m_chunk_size_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderView::chunkSizeChanged);
  connect(m_render_button, &QPushButton::clicked, this, &RenderView::renderRequested);
}

void RenderView::onRenderModeChanged(const QString& mode) {
  m_gpu_options_widget->setVisible(mode == "Multi-threaded CPU");
  emit renderModeChanged(mode);
}

void RenderView::setImageWidth(int width) {
  const QSignalBlocker blocker(m_image_width_spin);
  m_image_width_spin->setValue(width);
}

void RenderView::setImageHeight(int height) {
  const QSignalBlocker blocker(m_image_height_spin);
  m_image_height_spin->setValue(height);
}

void RenderView::setChannels(int channels) {
  const QSignalBlocker blocker(m_channels_combo);
  m_channels_combo->setCurrentText(QString::number(channels));
}

void RenderView::setSamplesPerPixel(int samples) {
  const QSignalBlocker blocker(m_samples_spin);
  m_samples_spin->setValue(samples);
}

void RenderView::setMaxBounces(int bounces) {
  const QSignalBlocker blocker(m_bounces_spin);
  m_bounces_spin->setValue(bounces);
}

void RenderView::setRenderMode(const QString& mode) {
  const QSignalBlocker blocker(m_render_mode_combo);
  m_render_mode_combo->setCurrentText(mode);
}

void RenderView::setThreadCount(unsigned int count) {
  const QSignalBlocker blocker(m_thread_count_spin);
  m_thread_count_spin->setValue(static_cast<int>(count));
}

void RenderView::setChunkSize(int size) {
  const QSignalBlocker blocker(m_chunk_size_spin);
  m_chunk_size_spin->setValue(size);
}
