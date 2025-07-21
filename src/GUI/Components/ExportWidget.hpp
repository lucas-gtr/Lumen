/**
 * @file ExportWidget.hpp
 * @brief Header file for the ExportWidget class, which provides a user interface for exporting renders.
 */
#ifndef GUI_COMPONENTS_EXPORTWIDGET_HPP
#define GUI_COMPONENTS_EXPORTWIDGET_HPP

#include <QGroupBox>

class RenderExporter;

namespace Ui {
  class ExportWidget;
}

/**
 * @class ExportWidget
 * @brief A widget for exporting rendered images with various settings.
 *
 * This class provides a user interface for exporting rendered images, allowing users to set the output format,
 * JPEG quality, file name, file path, tone mapping, and exposure settings. It also provides a button to trigger
 * the export process.
 */
class ExportWidget : public QGroupBox {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the ExportWidget class.
   * @param parent The parent widget for this export widget.
   */
  explicit ExportWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the RenderExporter instance for this widget.
   * @param exporter Pointer to the RenderExporter instance that handles the export logic.
   */
  void setExporter(RenderExporter* exporter);
  
  /**
   * @brief Sets whether the export is ready to be performed.
   * @param ready True if the export is ready, false otherwise.
   */
  void setExportReady(bool ready);

  /**
   * @brief Updates the widget to reflect the current export settings.
   */
  void updateWidget();

  ~ExportWidget() override; ///< Default destructor for the ExportWidget class.

signals:
  void toneMappingChanged();

private slots:
  void onFormatChanged(const QString& format);
  void onJpegQualityChanged(int quality);
  void onFileNameChanged(const QString& filename);
  void onFilePathChanged(const QString& path);
  void onToneMappingChanged(const QString& tone_mapping);
  void onExposureChanged(double exposure);
  void onExportButtonClicked();

private:
  Ui::ExportWidget* ui;
  RenderExporter*   m_exporter = nullptr;

  bool m_export_ready = false;
};

#endif // GUI_WIDGETS_LIGHTWIDGET_HPP
