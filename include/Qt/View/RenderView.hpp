#ifndef QT_VIEW_RENDERVIEW_HPP
#define QT_VIEW_RENDERVIEW_HPP

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>

/**
 * @class RenderView
 * @brief Widget for configuring render parameters.
 *
 * Emits signals when parameters are changed.
 */
class RenderView : public QWidget {
  Q_OBJECT

public:
  explicit RenderView(QWidget* parent);

  void setImageWidth(int width);
  void setImageHeight(int height);
  void setChannels(int channels);
  void setSamplesPerPixel(int samples);
  void setMaxBounces(int bounces);
  void setRenderMode(const QString& mode);
  void setThreadCount(unsigned int count);
  void setChunkSize(int size);

signals:
  void imageWidthChanged(int width);
  void imageHeightChanged(int height);
  void channelsChanged(int channels);
  void samplesPerPixelChanged(int samples);
  void maxBouncesChanged(int bounces);
  void renderModeChanged(const QString& mode);
  void threadCountChanged(int count);
  void chunkSizeChanged(int size);
  void renderRequested();

private:
  // Slots
  void onRenderModeChanged(const QString& mode);

  // Members
  QSpinBox*  m_image_width_spin  = nullptr;
  QSpinBox*  m_image_height_spin   = nullptr;
  QComboBox* m_channels_combo    = nullptr;
  QSpinBox*  m_samples_spin      = nullptr;
  QSpinBox*  m_bounces_spin      = nullptr;
  QComboBox* m_render_mode_combo = nullptr;

  QWidget*  m_gpu_options_widget = nullptr;
  QSpinBox* m_thread_count_spin  = nullptr;
  QSpinBox* m_chunk_size_spin    = nullptr;

  QPushButton* m_render_button = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_RENDERVIEW_HPP
