#include "RenderSettingsWidget.hpp"
#include "RenderWindow.hpp"
#include "ui_RenderSettingsWidget.h"

#include <QThread>
#include <QTimer>
#include <thread>

RenderSettingsWidget::RenderSettingsWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::RenderSettingsWidget), m_renderer(std::make_unique<Renderer>(&m_render_settings)),
      m_render_window(new RenderWindow()) {

  m_render_window->setFramebuffer(m_renderer->getFramebuffer());
  ui->setupUi(this);
  const unsigned int max_threads = std::max(1U, std::thread::hardware_concurrency() - THREADS_TO_KEEP_FREE);
  ui->threadCountSpinBox->setMaximum(static_cast<int>(max_threads));
  ui->chunksSizeLabel->setVisible(false);
  ui->chunksSizeSpinBox->setVisible(false);
  ui->threadCountLabel->setVisible(false);
  ui->threadCountSpinBox->setVisible(false);

  connect(ui->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderSettingsWidget::onWidthChanged);
  connect(ui->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RenderSettingsWidget::onHeightChanged);

  connect(ui->formatComboBox, &QComboBox::currentTextChanged, this, &RenderSettingsWidget::onFormatChanged);

  connect(ui->samplesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &RenderSettingsWidget::onSamplesChanged);
  connect(ui->maxBouncesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &RenderSettingsWidget::onBouncesChanged);

  connect(ui->renderModeComboBox, &QComboBox::currentTextChanged, this, &RenderSettingsWidget::onRenderModeChanged);

  connect(ui->threadCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &RenderSettingsWidget::onThreadCountChanged);
  connect(ui->chunksSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &RenderSettingsWidget::onChunkSizeChanged);

  connect(ui->renderButton, &QPushButton::clicked, this, &RenderSettingsWidget::onRenderButtonClicked);

  connect(this, &RenderSettingsWidget::renderStarted, m_render_window, &RenderWindow::onRenderStarted);
  connect(this, &RenderSettingsWidget::renderProgress, m_render_window, &RenderWindow::onRenderProgress);
  connect(this, &RenderSettingsWidget::renderFinished, m_render_window, &RenderWindow::onRenderFinished);
  connect(m_render_window, &RenderWindow::aboutToClose, this, &RenderSettingsWidget::onRenderStopped);

  updateWidget();
}

void RenderSettingsWidget::updateWidget() {
  ui->widthSpinBox->setValue(m_render_settings.getWidth());
  ui->heightSpinBox->setValue(m_render_settings.getHeight());

  const int channels = m_render_settings.getChannelCount();
  QString   format;
  switch(channels) {
  case 1:
    format = "Grayscale";
    break;
  case 3:
    format = "RGB";
    break;
  case 4:
    format = "RGBA";
    break;
  default:
    format = "RGB";
    break;
  }
  ui->formatComboBox->setCurrentText(format);

  ui->samplesSpinBox->setValue(m_render_settings.getSamplesPerPixel());
  ui->maxBouncesSpinBox->setValue(m_render_settings.getMaxBounces());

  const RenderMode mode     = m_render_settings.getRenderMode();
  const QString    mode_str = (mode == RenderMode::SINGLE_THREADED) ? "Single-threaded" : "Multi-threaded CPU";
  ui->renderModeComboBox->setCurrentText(mode_str);

  ui->threadCountSpinBox->setValue(static_cast<int>(m_render_settings.getThreadCount()));
  ui->chunksSizeSpinBox->setValue(m_render_settings.getChunkSize());
}

void RenderSettingsWidget::setScene(Scene* scene) { m_renderer->setScene(scene); }

void RenderSettingsWidget::openRenderWindow() { m_render_window->show(); }

void RenderSettingsWidget::onWidthChanged(int width) { m_render_settings.setWidth(width); }

void RenderSettingsWidget::onHeightChanged(int height) { m_render_settings.setHeight(height); }

void RenderSettingsWidget::onFormatChanged(const QString& format) {
  if(format == "Grayscale") {
    m_render_settings.setChannelCount(1);
  } else if(format == "RGB") {
    m_render_settings.setChannelCount(3);
  } else if(format == "RGBA") {
    m_render_settings.setChannelCount(4);
  }
}

void RenderSettingsWidget::onSamplesChanged(int samples) { m_render_settings.setSamplesPerPixel(samples); }

void RenderSettingsWidget::onBouncesChanged(int bounces) { m_render_settings.setMaxBounces(bounces); }

void RenderSettingsWidget::onRenderModeChanged(const QString& mode) {
  if(mode == "Single-threaded") {
    m_render_settings.setRenderMode(RenderMode::SINGLE_THREADED);
  } else if(mode == "Multi-threaded CPU") {
    m_render_settings.setRenderMode(RenderMode::MULTI_THREADED_CPU);
  }

  ui->chunksSizeSpinBox->setVisible(mode == "Multi-threaded CPU");
  ui->chunksSizeLabel->setVisible(mode == "Multi-threaded CPU");
  ui->threadCountSpinBox->setVisible(mode == "Multi-threaded CPU");
  ui->threadCountLabel->setVisible(mode == "Multi-threaded CPU");
}

void RenderSettingsWidget::onThreadCountChanged(int count) { m_render_settings.setThreadCount(count); }

void RenderSettingsWidget::onChunkSizeChanged(int size) { m_render_settings.setChunkSize(size); }

void RenderSettingsWidget::onRenderButtonClicked() {
  if(!m_renderer) {
    return;
  }
  ui->renderButton->setEnabled(false);
  openRenderWindow();
  emit renderStarted(m_render_settings.getImageProperties());

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this,
          [this]() { emit renderProgress(m_renderer->getRenderTime()->getRenderStats()); });
  timer->start(1000); // NOLINT

  auto     success = std::make_shared<bool>(false);
  QThread* thread  = QThread::create([this, success]() { *success = m_renderer->renderFrame(); });

  connect(thread, &QThread::finished, this, [this, timer, thread, success]() {
    timer->stop();
    timer->deleteLater();

    if(*success) {
      emit renderFinished(m_renderer->getRenderTime()->getRenderStats().elapsed_time);
    }

    thread->deleteLater();
    ui->renderButton->setEnabled(true);
  });

  thread->start();
}

void RenderSettingsWidget::onRenderStopped() {
  if(m_renderer) {
    m_renderer->requestStop();
  }
}

RenderSettingsWidget::~RenderSettingsWidget() {
  delete m_render_window;
  delete ui;
}
