#ifndef QT_VIEW_EXPORTVIEW_HPP
#define QT_VIEW_EXPORTVIEW_HPP

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>

/**
 * @class ExportView
 * @brief Widget for configuring export settings.
 *
 * This view emits signals when export parameters are changed.
 */
class ExportView : public QWidget {
  Q_OBJECT

public:
  explicit ExportView(QWidget* parent);

  void setFileName(const QString& name);
  void setJpegQuality(int quality);
  void setOutputFormat(const QString& format);
  void setExportFolder(const QString& path);
  void setToneMapping(const QString& tone_mapping);
  void setExposure(double exposure);

  // Slots
  void onRenderStarted();
  void onRenderFinished();

signals:
  void outputFormatChanged(const QString& format);
  void jpegQualityChanged(int quality);
  void fileNameChanged(const QString& name);
  void exportFolderChanged(const QString& path);
  void toneMappingChanged(const QString& mapping);
  void exposureChanged(double exposure);
  void exportRequested();

private:
  // Slots
  void onToneMappingChanged(const QString& mapping);
  void onOutputFormatChanged(const QString& format);
  void onFileNameEdited(const QString& text);
  void onBrowseButtonClicked();

  // Members
  QLineEdit*      m_file_name_edit      = nullptr;
  QLineEdit*      m_export_folder_edit  = nullptr;
  QPushButton*    m_browse_button       = nullptr;
  QComboBox*      m_output_format_combo = nullptr;
  QLabel*         m_jpeg_quality_label  = nullptr;
  QSlider*        m_jpeg_quality_slider = nullptr;
  QSpinBox*       m_jpeg_quality_spin   = nullptr;
  QComboBox*      m_tone_mapping_combo  = nullptr;
  QLabel*         m_exposure_label      = nullptr;
  QDoubleSpinBox* m_exposure_spin       = nullptr;
  QPushButton*    m_export_button       = nullptr;

  void updateFileNameExtension(const QString& extension);

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_EXPORTVIEW_HPP
