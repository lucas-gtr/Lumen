#include <QFile>
#include <QMessageBox>

#include "Core/Config.hpp"
#include "Export/RenderExporter.hpp"
#include "ExportWidget.hpp"
#include "ui_ExportWidget.h"

ExportWidget::ExportWidget(QWidget* parent) : QGroupBox(parent), ui(new Ui::ExportWidget) {
  ui->setupUi(this);

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
  connect(ui->exportButton, &QPushButton::clicked, this, &ExportWidget::onExportButtonClicked);
}

void ExportWidget::setExportReady(bool ready) {
  m_export_ready = ready;
  ui->exportButton->setEnabled(ready);
  if(ready) {
    m_exporter->updateImageToExport();
  }
}

void ExportWidget::updateWidget() {
  const OutputFormat format = m_exporter->getOutputFormat();
  switch(format) {
  case OutputFormat::PNG:
    ui->formatComboBox->setCurrentText("PNG");
    break;
  case OutputFormat::JPEG:
    ui->formatComboBox->setCurrentText("JPEG");
    break;
  case OutputFormat::BMP:
    ui->formatComboBox->setCurrentText("BMP");
    break;
  case OutputFormat::TGA:
    ui->formatComboBox->setCurrentText("TGA");
    break;
  case OutputFormat::HDR:
    ui->formatComboBox->setCurrentText("HDR");
    break;
  }
  ui->suffixLabel->setText('.' + ui->formatComboBox->currentText().toLower());

  ui->jpegQualityWidget->setValue(m_exporter->getJpegQuality());
  ui->jpegQualityWidget->setVisible(format == OutputFormat::JPEG);
  ui->jpegQualityLabel->setVisible(format == OutputFormat::JPEG);

  ui->fileNameLineEdit->setText(QString::fromStdString(m_exporter->getFilename()));
  ui->fileSelector->setPath(QString::fromStdString(m_exporter->getPath()));

  const ToneMapping tone_mapping = m_exporter->getToneMapping();
  switch(tone_mapping) {
  case ToneMapping::NONE:
    ui->toneMappingComboBox->setCurrentText("None");
    break;
  case ToneMapping::REINHARD:
    ui->toneMappingComboBox->setCurrentText("Reinhard");
    break;
  case ToneMapping::EXPOSURE:
    ui->toneMappingComboBox->setCurrentText("Exposure");
    break;
  }

  ui->exposureSpinBox->setValue(m_exporter->getExposure());
  ui->exposureSpinBox->setVisible(tone_mapping == ToneMapping::EXPOSURE);
  ui->exposureLabel->setVisible(tone_mapping == ToneMapping::EXPOSURE);
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
  if(format == "PNG") {
    m_exporter->setOutputFormat(OutputFormat::PNG);
  } else if(format == "JPEG") {
    m_exporter->setOutputFormat(OutputFormat::JPEG);
  } else if(format == "BMP") {
    m_exporter->setOutputFormat(OutputFormat::BMP);
  } else if(format == "TGA") {
    m_exporter->setOutputFormat(OutputFormat::TGA);
  } else if(format == "HDR") {
    m_exporter->setOutputFormat(OutputFormat::HDR);
  }
  ui->suffixLabel->setText('.' + ui->formatComboBox->currentText().toLower());

  ui->jpegQualityWidget->setVisible(format == "JPEG");
  ui->jpegQualityLabel->setVisible(format == "JPEG");
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
  if(tone_mapping == "None") {
    m_exporter->setToneMapping(ToneMapping::NONE);
  } else if(tone_mapping == "Reinhard") {
    m_exporter->setToneMapping(ToneMapping::REINHARD);
  } else if(tone_mapping == "Exposure") {
    m_exporter->setToneMapping(ToneMapping::EXPOSURE);
  }
  ui->exposureSpinBox->setVisible(tone_mapping == "Exposure");
  ui->exposureLabel->setVisible(tone_mapping == "Exposure");

  if(m_export_ready) {
    m_exporter->updateImageToExport();
    emit toneMappingChanged();
  }
}

void ExportWidget::onExposureChanged(double exposure) {
  if(m_exporter != nullptr) {
    m_exporter->setExposure(exposure);
    if(m_export_ready) {
      m_exporter->updateImageToExport();
      emit toneMappingChanged();
    }
  }
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
