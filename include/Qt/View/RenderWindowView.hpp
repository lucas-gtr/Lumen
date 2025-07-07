#ifndef QT_VIEW_RENDERWINDOWVIEW_HPP
#define QT_VIEW_RENDERWINDOWVIEW_HPP

#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qprogressbar.h>
#include <qwidget.h>

#include "Qt/Controller/ExportController.hpp"
#include "Qt/View/ExportView.hpp"
#include "Rendering/RenderSettings.hpp"
#include "Rendering/Renderer.hpp"
#include "Scene/Scene.hpp"

class RenderWindowView : public QWidget {
  Q_OBJECT

public:
  explicit RenderWindowView(Framebuffer* framebuffer, QWidget* parent);

protected:
  void closeEvent(QCloseEvent* event) override;

signals:
  void previewStateChanged(bool enabled);

  void renderStarted();
  void renderCancelled();
  void renderFinished();

public slots:
  void onRenderStarted(ImageProperties properties);
  void onRenderProgress(RenderStats stats);
  void onUpdateRenderPreview(const double* framebuffer_data);
  void onRenderFinished(double elapsed_time);

  void onExportImageChanged(const unsigned char* image_data);

private:
  QLabel*       m_image_label  = nullptr;
  QLabel*       m_status_label = nullptr;
  QProgressBar* m_progress_bar = nullptr;

  QCheckBox* m_preview_checkbox = nullptr;

  ExportView*       m_export_view;
  ExportController* m_export_controller;

  int m_image_width   = 0;
  int m_image_height  = 0;
  int m_channel_count = 0;

  bool m_render_finished = false;

  void setupUI();
  void connectSignals();

  QImage createImage() const;

  void updateImage(const double* image_data);
  void updateImage(const unsigned char* image_data);
  void clearImage();

  static QString FormatSecondsToString(double seconds);
};

#endif // QT_VIEW_RENDERWINDOWVIEW_HPP