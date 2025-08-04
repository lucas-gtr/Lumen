// GCOVR_EXCL_START
#include "FileSelector.hpp"
#include "ui_FileSelector.h"

#include <QFileDialog>

FileSelector::FileSelector(QWidget* parent) : QWidget(parent), ui(new Ui::FileSelector) {
  ui->setupUi(this);
  setupConnections();
}

void FileSelector::setupConnections() {
  connect(ui->browseButton, &QPushButton::clicked, this, &FileSelector::openDialog);
  connect(ui->lineEdit, &QLineEdit::textChanged, this, &FileSelector::pathChanged);
}

void FileSelector::setMode(FileSelectorMode mode) { m_mode = mode; }

void FileSelector::setPath(const QString& path) { ui->lineEdit->setText(path); }

QString FileSelector::getPath() const { return ui->lineEdit->text(); }

void FileSelector::openDialog() {
  QString selected_path;

  switch(m_mode) {
  case FileSelectorMode::FOLDER:
    selected_path = QFileDialog::getExistingDirectory(this, "Select Folder", getPath());
    break;

  case FileSelectorMode::IMAGE_FILE:
    selected_path =
        QFileDialog::getOpenFileName(this, "Select Image", getPath(), "Images (*.png *.jpg *.jpeg *.bmp *.tga *.hdr)");
    break;
  }

  if(!selected_path.isEmpty()) {
    ui->lineEdit->setText(selected_path);
  }
}

FileSelector::~FileSelector() { delete ui; }
// GCOVR_EXCL_STOP