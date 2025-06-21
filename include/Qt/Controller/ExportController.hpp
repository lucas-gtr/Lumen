#ifndef QT_CONTROLLER_EXPORTCONTROLLER_HPP
#define QT_CONTROLLER_EXPORTCONTROLLER_HPP

#include <memory>
#include <qobject.h>
#include <qstring.h>

#include "Export/RenderExporter.hpp"
#include "Qt/View/ExportView.hpp"

class ExportController : public QObject {
  Q_OBJECT
public:
  explicit ExportController(ExportView* view, Framebuffer* framebuffer);

  // Slots
  void onRenderFinished();

signals:
  void exportImageChanged(const unsigned char* image_data);

private:
  // Slots
  void onOutputFormatChanged(const QString& format);
  void onJpegQualityChanged(int quality);
  void onFileNameChanged(const QString& name);
  void onExportFolderChanged(const QString& path);
  void onToneMappingChanged(const QString& mapping);
  void onExposureChanged(double exposure);
  void onExportRequested();

  // Members
  ExportView* m_view;

  std::unique_ptr<RenderExporter> m_exporter;

  void connectSignals();
  void updateViewFromModel();
};

#endif // QT_CONTROLLER_EXPORTCONTROLLER_HPP