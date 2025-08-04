// GCOVR_EXCL_START
#include <QFile>
#include <QMessageBox>

#include "Core/Config.hpp"
#include "Export/OutputFormat.hpp"
#include "Export/RenderExporter.hpp"
#include "ExportWidget.hpp"
#include "PostProcessing/ToneMapping/ToneMapping.hpp"
#include "ui_ExportWidget.h"

ExportWidget::ExportWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::ExportWidget) {
  ui->setupUi(this);
  for(const auto& name : availableToneMappingNames()) {
    ui->toneMappingComboBox->addItem(QString::fromStdString(name));
  }
  for(const auto& [name, value] : stringToOutputFormatMap()) {
    ui->formatComboBox->addItem(QString::fromStdString(name));
  }

  setStyleSheet(QString::fromStdString(std::string(GROUP_BOX_STYLESHEET)));

  ui->fileSelector->setMode(FileSelectorMode::FOLDER);
  ui->jpegQualityWidget->setMinimum(1);
  ui->jpegQualityWidget->setMaximum(MAX_JPEG_QUALITY);
  ui->jpegQualityWidget->setStep(JPEG_QUALITY_STEP);
  ui->jpegQualityWidget->setValue(DEFAULT_JPEG_QUALITY);

  connect(ui->formatComboBox, &QComboBox::currentTextChanged, this, &ExportWidget::onFormatChanged);
  connect(ui->jpegQualityWidget, &SliderSpinBox::valueChanged, this, &ExportWidget::onJpegQualityChanged);
  connect(ui->fileNameLineEdit, &QLineEdit::textChanged, this, &ExportWidget::onFileNameChanged);
  connect(ui->fileSelector, &FileSelector::pathChanged, this, &ExportWidget::onFilePathChanged);
  connect(ui->toneMappingComboBox, &QComboBox::currentTextChanged, this, &ExportWidget::onToneMappingChanged);
  connect(ui->exposureSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ExportWidget::onExposureChanged);
  connect(ui->whitePointSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ExportWidget::onWhitePointChanged);
  connect(ui->maxWhitePointButton, &QPushButton::clicked, this, &ExportWidget::onMaxWhitePointClicked);
  connect(ui->exportButton, &QPushButton::clicked, this, &ExportWidget::onExportButtonClicked);
}

void ExportWidget::setExportReady(bool ready) { ui->exportButton->setEnabled(ready); }

void ExportWidget::updateWidget() {
  const OutputFormat format = m_exporter->getOutputFormat();
  ui->formatComboBox->setCurrentText(QString::fromStdString(outputFormatToString(format)));
  ui->suffixLabel->setText('.' + ui->formatComboBox->currentText().toLower());

  ui->jpegQualityWidget->setValue(m_exporter->getJpegQuality());
  ui->jpegQualityWidget->setVisible(format == OutputFormat::JPEG);
  ui->jpegQualityLabel->setVisible(format == OutputFormat::JPEG);

  ui->fileNameLineEdit->setText(QString::fromStdString(m_exporter->getFilename()));
  ui->fileSelector->setPath(QString::fromStdString(m_exporter->getPath()));

  const ToneMapping tone_mapping = m_exporter->getToneMapping();
  ui->toneMappingComboBox->setCurrentText(QString::fromStdString(toneMappingToString(tone_mapping)));

  ui->exposureSpinBox->setValue(m_exporter->getExposure());
  ui->exposureSpinBox->setVisible(tone_mapping == ToneMapping::EXPOSURE || tone_mapping == ToneMapping::UNCHARTED2);
  ui->exposureLabel->setVisible(tone_mapping == ToneMapping::EXPOSURE || tone_mapping == ToneMapping::UNCHARTED2);

  ui->whitePointSpinBox->setValue(m_exporter->getWhitePoint());
  ui->whitePointWidget->setVisible(tone_mapping == ToneMapping::WHITE_POINT_REINHARD ||
                                   tone_mapping == ToneMapping::UNCHARTED2);
  ui->whitePointLabel->setVisible(tone_mapping == ToneMapping::WHITE_POINT_REINHARD ||
                                  tone_mapping == ToneMapping::UNCHARTED2);
}

void ExportWidget::setExporter(RenderExporter* exporter) {
  m_exporter = exporter;
  if(m_exporter != nullptr) {
    updateWidget();
  }
}

void ExportWidget::onFormatChanged(const QString& format) {
  if(m_exporter == nullptr) {
    return;
  }
  m_exporter->setOutputFormat(stringToOutputFormat(format.toStdString()));
  ui->suffixLabel->setText('.' + ui->formatComboBox->currentText().toLower());

  const OutputFormat current_format = m_exporter->getOutputFormat();
  ui->jpegQualityWidget->setVisible(current_format == OutputFormat::JPEG);
  ui->jpegQualityLabel->setVisible(current_format == OutputFormat::JPEG);
}

void ExportWidget::onJpegQualityChanged(int quality) {
  if(m_exporter != nullptr) {
    m_exporter->setJpegQuality(quality);
  }
}

void ExportWidget::onFileNameChanged(const QString& filename) {
  if(m_exporter != nullptr) {
    m_exporter->setFilename(filename.toStdString() + ui->suffixLabel->text().toStdString());
  }
}

void ExportWidget::onFilePathChanged(const QString& path) {
  if(m_exporter != nullptr) {
    m_exporter->setPath(path.toStdString());
  }
}

void ExportWidget::onToneMappingChanged(const QString& tone_mapping) {
  if(m_exporter == nullptr) {
    return;
  }
  m_exporter->setToneMapping(stringToToneMapping(tone_mapping.toStdString()));
  const ToneMapping t_m = m_exporter->getToneMapping();
  ui->exposureSpinBox->setVisible(t_m == ToneMapping::EXPOSURE || t_m == ToneMapping::UNCHARTED2);
  ui->exposureLabel->setVisible(t_m == ToneMapping::EXPOSURE || t_m == ToneMapping::UNCHARTED2);
  ui->whitePointWidget->setVisible(t_m == ToneMapping::WHITE_POINT_REINHARD || t_m == ToneMapping::UNCHARTED2);
  ui->whitePointLabel->setVisible(t_m == ToneMapping::WHITE_POINT_REINHARD || t_m == ToneMapping::UNCHARTED2);
  emit toneMappingChanged();
}

void ExportWidget::onExposureChanged(double exposure) {
  if(m_exporter != nullptr) {
    m_exporter->setExposure(exposure);
    emit toneMappingChanged();
  }
}

void ExportWidget::onWhitePointChanged(double white_point) {
  if(m_exporter != nullptr) {
    m_exporter->setWhitePoint(white_point);
    emit toneMappingChanged();
  }
}

void ExportWidget::onMaxWhitePointClicked() {
  if(m_exporter == nullptr) {
    return;
  }
  m_exporter->setWhitePoint(m_exporter->getFramebuffer()->getMaximumValue());
  ui->whitePointSpinBox->setValue(m_exporter->getWhitePoint());
}

void ExportWidget::onExportButtonClicked() {
  if(m_exporter == nullptr) {
    return;
  }
  const QString path           = QString::fromStdString(m_exporter->getPath());
  const QString filename       = QString::fromStdString(m_exporter->getFilename());
  const QString full_file_path = path + "/" + filename;

  if(QFile::exists(full_file_path)) {
    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "File Already Exists",
        QString("The file \"%1\" already exists.\nDo you want to overwrite it?").arg(full_file_path),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if(reply != QMessageBox::Yes) {
      return;
    }
  }

  if(m_exporter->exportRender()) {
    QMessageBox::information(this, "Export Successful",
                             QString("The render was successfully exported to:\n%1").arg(full_file_path));
  } else {
    QMessageBox::critical(this, "Export Failed", "An error occurred while exporting the render.");
  }
}

ExportWidget::~ExportWidget() { delete ui; }
// GCOVR_EXCL_STOP