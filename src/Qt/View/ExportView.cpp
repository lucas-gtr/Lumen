#include <qboxlayout.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qformlayout.h>
#include <qglobal.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include "Core/Config.hpp"
#include "Qt/View/ExportView.hpp"

ExportView::ExportView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void ExportView::setupUI() {
  setMinimumWidth(EXPORT_VIEW_MINIMUM_WIDTH);

  auto* group_box = new QGroupBox("Export settings", this);
  group_box->setStyleSheet("QGroupBox {font-weight :bold; font-size: 14px;}");

  auto* form_layout = new QFormLayout();

  m_output_format_combo = new QComboBox(group_box);
  m_output_format_combo->addItem("PNG");
  m_output_format_combo->addItem("JPEG");
  m_output_format_combo->addItem("BMP");
  m_output_format_combo->addItem("TGA");
  m_output_format_combo->addItem("HDR");

  m_jpeg_quality_label = new QLabel("JPEG Quality", group_box);
  m_jpeg_quality_label->setVisible(false);

  m_jpeg_quality_slider = new QSlider(Qt::Horizontal, group_box);
  m_jpeg_quality_slider->setRange(MIN_JPEG_QUALITY, MAX_JPEG_QUALITY);
  m_jpeg_quality_slider->setValue(DEFAULT_JPEG_QUALITY);
  m_jpeg_quality_slider->setSingleStep(1);
  m_jpeg_quality_slider->setTickPosition(QSlider::TicksBelow);
  m_jpeg_quality_slider->setVisible(false);

  m_jpeg_quality_spin = new QSpinBox(group_box);
  m_jpeg_quality_spin->setRange(MIN_JPEG_QUALITY, MAX_JPEG_QUALITY);
  m_jpeg_quality_spin->setValue(DEFAULT_JPEG_QUALITY);
  m_jpeg_quality_spin->setSingleStep(JPEG_QUALITY_COMBO_STEP);
  m_jpeg_quality_spin->setSuffix("%");
  m_jpeg_quality_spin->setVisible(false);

  auto* jpeg_quality_widget = new QWidget(group_box);
  auto* jpeg_quality_layout = new QHBoxLayout(jpeg_quality_widget);
  jpeg_quality_layout->setContentsMargins(0, 0, 0, 0);
  jpeg_quality_layout->addWidget(m_jpeg_quality_slider);
  jpeg_quality_layout->addWidget(m_jpeg_quality_spin);
  jpeg_quality_widget->setLayout(jpeg_quality_layout);

  m_file_name_edit = new QLineEdit(group_box);
  m_file_name_edit->setText("render_output.png");

  m_export_folder_edit = new QLineEdit(group_box);
  m_browse_button      = new QPushButton("Browse...", group_box);

  auto* folder_widget = new QWidget(group_box);
  auto* folder_layout = new QHBoxLayout(folder_widget);
  folder_layout->setContentsMargins(0, 0, 0, 0);
  folder_layout->addWidget(m_export_folder_edit);
  folder_layout->addWidget(m_browse_button);
  folder_widget->setLayout(folder_layout);

  m_tone_mapping_combo = new QComboBox(group_box);
  m_tone_mapping_combo->addItems({"None", "Reinhard", "Exposure"});

  m_exposure_label = new QLabel("Exposure", group_box);
  m_exposure_label->setVisible(false);

  m_exposure_spin = new QDoubleSpinBox(group_box);
  m_exposure_spin->setRange(MIN_TONE_MAPPING_EXPOSURE, MAX_TONE_MAPPING_EXPOSURE);
  m_exposure_spin->setSingleStep(EXPOSURE_STEP);
  m_exposure_spin->setValue(1.0);
  m_exposure_spin->setVisible(false);

  m_export_button = new QPushButton("Export", group_box);

  form_layout->addRow("Format ", m_output_format_combo);
  form_layout->addRow(m_jpeg_quality_label, jpeg_quality_widget);
  form_layout->addRow("File name ", m_file_name_edit);
  form_layout->addRow("Folder ", folder_widget);
  form_layout->addRow("Tone mapping ", m_tone_mapping_combo);
  form_layout->addRow(m_exposure_label, m_exposure_spin);
  form_layout->addRow(m_export_button);

  group_box->setLayout(form_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(group_box);
  main_layout->addStretch();
  setLayout(main_layout);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void ExportView::connectSignals() {
  connect(m_file_name_edit, &QLineEdit::textChanged, this, &ExportView::onFileNameEdited);
  connect(m_browse_button, &QPushButton::clicked, this, &ExportView::onBrowseButtonClicked);

  connect(m_output_format_combo, &QComboBox::currentTextChanged, this, &ExportView::onOutputFormatChanged);
  connect(m_jpeg_quality_slider, &QSlider::valueChanged, m_jpeg_quality_spin, &QSpinBox::setValue);
  connect(m_jpeg_quality_spin, QOverload<int>::of(&QSpinBox::valueChanged), m_jpeg_quality_slider, &QSlider::setValue);
  connect(m_jpeg_quality_spin, &QSpinBox::valueChanged, this, &ExportView::jpegQualityChanged);

  connect(m_tone_mapping_combo, &QComboBox::currentTextChanged, this, &ExportView::onToneMappingChanged);
  connect(m_exposure_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ExportView::exposureChanged);
  connect(m_export_button, &QPushButton::clicked, this, &ExportView::exportRequested);
}

void ExportView::onToneMappingChanged(const QString& mapping) {
  m_exposure_label->setVisible(mapping == "Exposure");
  m_exposure_spin->setVisible(mapping == "Exposure");
  emit toneMappingChanged(mapping);
}

void ExportView::updateFileNameExtension(const QString& new_extension) {
  const QString current_text      = m_file_name_edit->text();
  QString       base              = current_text;
  const QString current_extension = QFileInfo(current_text).suffix();

  if(!current_extension.isEmpty() && base.endsWith("." + current_extension, Qt::CaseInsensitive)) {
    base.chop(current_extension.length() + 1);
  }

  const QString updated_file_name = base + "." + new_extension;
  if(m_file_name_edit->text() != updated_file_name) {
    m_file_name_edit->blockSignals(true);
    m_file_name_edit->setText(updated_file_name);
    m_file_name_edit->blockSignals(false);
  }
  emit fileNameChanged(m_file_name_edit->text());
}

void ExportView::onOutputFormatChanged(const QString& format) {
  const bool is_jpeg = (format == "JPEG");
  m_jpeg_quality_label->setVisible(is_jpeg);
  m_jpeg_quality_slider->setVisible(is_jpeg);
  m_jpeg_quality_spin->setVisible(is_jpeg);

  updateFileNameExtension(format.toLower());
  emit outputFormatChanged(format);
}

void ExportView::onFileNameEdited(const QString& text) {
  Q_UNUSED(text);
  updateFileNameExtension(m_output_format_combo->currentText().toLower());
}

void ExportView::onBrowseButtonClicked() {
  const QString dir = QFileDialog::getExistingDirectory(this, "Select Export Folder");
  if(!dir.isEmpty()) {
    m_export_folder_edit->setText(dir);
    emit exportFolderChanged(dir);
  }
}

void ExportView::onRenderStarted() { m_export_button->setEnabled(false); }

void ExportView::onRenderFinished() { m_export_button->setEnabled(true); }

void ExportView::setOutputFormat(const QString& format) {
  const QSignalBlocker blocker(m_output_format_combo);
  const int            index = m_output_format_combo->findText(format);
  if(index != -1) {
    m_output_format_combo->setCurrentIndex(index);
  }
}

void ExportView::setFileName(const QString& name) {
  const QSignalBlocker blocker(m_file_name_edit);
  m_file_name_edit->setText(name);
  updateFileNameExtension(m_output_format_combo->currentText().toLower());
}

void ExportView::setExportFolder(const QString& path) {
  const QSignalBlocker blocker(m_export_folder_edit);
  m_export_folder_edit->setText(path);
}

void ExportView::setToneMapping(const QString& tone_mapping) {
  const QSignalBlocker blocker(m_tone_mapping_combo);
  const int            index = m_tone_mapping_combo->findText(tone_mapping);
  if(index != -1) {
    m_tone_mapping_combo->setCurrentIndex(index);
  }
}

void ExportView::setExposure(double exposure) {
  const QSignalBlocker blocker(m_exposure_spin);
  m_exposure_spin->setValue(exposure);
}

void ExportView::setJpegQuality(int quality) {
  const QSignalBlocker blocker(m_jpeg_quality_slider);
  const QSignalBlocker blocker_spin(m_jpeg_quality_spin);
  m_jpeg_quality_slider->setValue(quality);
  m_jpeg_quality_spin->setValue(quality);
}
