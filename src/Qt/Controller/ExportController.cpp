#include <memory>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <string>

#include "Core/Framebuffer.hpp"
#include "Export/OutputFormat.hpp"
#include "Export/RenderExporter.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "Qt/Controller/ExportController.hpp"
#include "Qt/View/ExportView.hpp"

ExportController::ExportController(ExportView* view, Framebuffer* framebuffer)
    : QObject(view), m_view(view), m_exporter(std::make_unique<RenderExporter>(framebuffer)) {
  connectSignals();
  updateViewFromModel();
}

void ExportController::connectSignals() {
  connect(m_view, &ExportView::outputFormatChanged, this, &ExportController::onOutputFormatChanged);
  connect(m_view, &ExportView::jpegQualityChanged, this, &ExportController::onJpegQualityChanged);
  connect(m_view, &ExportView::fileNameChanged, this, &ExportController::onFileNameChanged);
  connect(m_view, &ExportView::exportFolderChanged, this, &ExportController::onExportFolderChanged);
  connect(m_view, &ExportView::toneMappingChanged, this, &ExportController::onToneMappingChanged);
  connect(m_view, &ExportView::exposureChanged, this, &ExportController::onExposureChanged);
  connect(m_view, &ExportView::exportRequested, this, &ExportController::onExportRequested);
}

void ExportController::onOutputFormatChanged(const QString& format) {
  OutputFormat output_format = OutputFormat::PNG;
  if(format == "PNG") {
    output_format = OutputFormat::PNG;
  } else if(format == "JPEG") {
    output_format = OutputFormat::JPEG;
  } else if(format == "BMP") {
    output_format = OutputFormat::BMP;
  } else if(format == "TGA") {
    output_format = OutputFormat::TGA;
  } else if(format == "HDR") {
    output_format = OutputFormat::HDR;
  }

  m_exporter->setOutputFormat(output_format);
}

void ExportController::onJpegQualityChanged(int quality) { m_exporter->setJpegQuality(quality); }

void ExportController::onFileNameChanged(const QString& name) { m_exporter->setFilename(name.toStdString()); }

void ExportController::onExportFolderChanged(const QString& path) { m_exporter->setPath(path.toStdString()); }

void ExportController::onToneMappingChanged(const QString& mapping) {
  ToneMapping tone_mapping = ToneMapping::NONE;
  if(mapping == "None") {
    tone_mapping = ToneMapping::NONE;
  } else if(mapping == "Reinhard") {
    tone_mapping = ToneMapping::REINHARD;
  } else if(mapping == "Exposure") {
    tone_mapping = ToneMapping::EXPOSURE;
  }

  m_exporter->setToneMapping(tone_mapping);
  m_exporter->updateImageToExport();

  emit exportImageChanged(m_exporter->getImageToExport());
}

void ExportController::onExposureChanged(double exposure) {
  m_exporter->setExposure(exposure);
  m_exporter->updateImageToExport();

  emit exportImageChanged(m_exporter->getImageToExport());
}

void ExportController::onExportRequested() { m_exporter->exportRender(); }

void ExportController::onRenderFinished() {
  m_exporter->updateImageToExport();
  emit exportImageChanged(m_exporter->getImageToExport());
}

void ExportController::updateViewFromModel() {
  m_view->setFileName(QString::fromStdString(m_exporter->getFilename()));
  m_view->setExportFolder(QString::fromStdString(m_exporter->getPath()));

  std::string format_str;
  switch(m_exporter->getOutputFormat()) {
  case OutputFormat::PNG:
    format_str = "PNG";
    break;
  case OutputFormat::JPEG:
    format_str = "JPEG";
    break;
  case OutputFormat::BMP:
    format_str = "BMP";
    break;
  case OutputFormat::TGA:
    format_str = "TGA";
    break;
  case OutputFormat::HDR:
    format_str = "HDR";
    break;
  default:
    format_str = "Unknown";
  }
  m_view->outputFormatChanged(QString::fromStdString(format_str));

  std::string tone_mapping_str;
  switch(m_exporter->getToneMapping()) {
  case ToneMapping::NONE:
    tone_mapping_str = "None";
    break;
  case ToneMapping::REINHARD:
    tone_mapping_str = "Reinhard";
    break;
  case ToneMapping::EXPOSURE:
    tone_mapping_str = "Exposure";
    break;
  default:
    tone_mapping_str = "Unknown";
  }
  m_view->setToneMapping(QString::fromStdString(tone_mapping_str));

  m_view->setJpegQuality(m_exporter->getJpegQuality());
  m_view->setExposure(m_exporter->getExposure());
}