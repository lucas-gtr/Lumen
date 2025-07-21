#include <QMessageBox>
#include <QSignalBlocker>

#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"
#include "TexturesWidget.hpp"
#include "ui_TexturesWidget.h"

TexturesWidget::TexturesWidget(QWidget* parent) : QWidget(parent), ui(new Ui::TexturesWidget) {
  ui->setupUi(this);
  ui->pathWidget->setMode(FileSelectorMode::IMAGE_FILE);
  ui->texturePreview->setMaximumSize(TEXTURE_PREVIEW_MAX_SIZE, TEXTURE_PREVIEW_MAX_SIZE);

  updateWidget();

  connect(ui->addButton, &QPushButton::clicked, this, &TexturesWidget::onAddButtonClicked);
  connect(ui->removeButton, &QPushButton::clicked, this, &TexturesWidget::onRemoveButtonClicked);
  connect(ui->textureNameLineEdit, &QLineEdit::editingFinished, this, &TexturesWidget::onTextureNameEdited);
  connect(ui->typeComboBox, &QComboBox::currentTextChanged, this, &TexturesWidget::onTextureTypeChanged);
  connect(ui->colorButton, &ColorButton::colorChanged, this, &TexturesWidget::onColorChanged);
  connect(ui->pathWidget, &FileSelector::pathChanged, this, &TexturesWidget::onTexturePathChanged);
  connect(ui->flipVerticallyCheckBox, &QCheckBox::toggled, this, &TexturesWidget::onFlipVerticallyToggled);
  connect(ui->colorSpaceComboBox, &QComboBox::currentTextChanged, this, &TexturesWidget::onColorSpaceChanged);
  connect(ui->filteringComboBox, &QComboBox::currentTextChanged, this, &TexturesWidget::onFilteringModeChanged);
  connect(ui->wrappingComboBox, &QComboBox::currentTextChanged, this, &TexturesWidget::onWrappingModeChanged);
  connect(ui->borderColorButton, &ColorButton::colorChanged, this, &TexturesWidget::onBorderColorChanged);
}

void TexturesWidget::setTextureManager(TextureManager* texture_manager) { m_texture_manager = texture_manager; }

void TexturesWidget::setModel(TexturesListModel* model) {
  ui->texturesListView->setModel(model);
  m_textures_list_model = model;

  connect(ui->texturesListView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
          &TexturesWidget::onTextureSelectionChanged);
  connect(m_textures_list_model, &TexturesListModel::dataChanged, this, &TexturesWidget::onTextureRenamed);
}

void TexturesWidget::updateTexturePreview() {
  if(m_current_texture != nullptr) {
    ui->texturePreview->setImage(m_current_texture->getPreviewData(), m_current_texture->getPreviewProperties());
  }
}

void TexturesWidget::updateWidget() {
  const bool widgets_visible = (m_current_texture != nullptr);

  ui->textureNameLineEdit->setVisible(widgets_visible);
  ui->texturePreview->setVisible(widgets_visible);
  ui->textureSettingsWidget->setVisible(widgets_visible);
  ui->removeButton->setEnabled(widgets_visible);

  if(!widgets_visible) {
    return;
  }

  const QSignalBlocker type_blocker(ui->typeComboBox);
  const QSignalBlocker color_blocker(ui->colorButton);
  const QSignalBlocker path_blocker(ui->pathWidget);
  const QSignalBlocker flip_blocker(ui->flipVerticallyCheckBox);
  const QSignalBlocker color_space_blocker(ui->colorSpaceComboBox);
  const QSignalBlocker filtering_blocker(ui->filteringComboBox);
  const QSignalBlocker wrapping_blocker(ui->wrappingComboBox);
  const QSignalBlocker border_color_blocker(ui->borderColorButton);

  const TextureType type = m_current_texture->getTextureType();
  switch(type) {
  case TextureType::IMAGE_TEXTURE:
    ui->typeComboBox->setCurrentText("Image Texture");
    ui->pathWidget->setPath(QString::fromStdString(m_current_texture->getTexturePath()));
    ui->flipVerticallyCheckBox->setChecked(m_current_texture->isFlippedVertically());
    break;
  case TextureType::COLOR_TEXTURE:
    ui->typeComboBox->setCurrentText("Color Texture");
    ui->colorButton->setColor(m_current_texture->getValue3d({0, 0}));
    break;
  }
  ui->colorLabel->setVisible(type == TextureType::COLOR_TEXTURE);
  ui->colorButton->setVisible(type == TextureType::COLOR_TEXTURE);
  ui->pathLabel->setVisible(type == TextureType::IMAGE_TEXTURE);
  ui->pathWidget->setVisible(type == TextureType::IMAGE_TEXTURE);
  ui->flipVerticallyLabel->setVisible(type == TextureType::IMAGE_TEXTURE);
  ui->flipVerticallyCheckBox->setVisible(type == TextureType::IMAGE_TEXTURE);

  const ColorSpace color_space = m_current_texture->getColorSpace();
  switch(color_space) {
  case ColorSpace::S_RGB:
    ui->colorSpaceComboBox->setCurrentText("sRGB");
    break;
  case ColorSpace::LINEAR:
    ui->colorSpaceComboBox->setCurrentText("Linear");
    break;
  }

  const TextureSampling::TextureFiltering filtering_mode = m_current_texture->getFilteringMode();
  switch(filtering_mode) {
  case TextureSampling::TextureFiltering::BILINEAR:
    ui->filteringComboBox->setCurrentText("Bilinear");
    break;
  case TextureSampling::TextureFiltering::NEAREST:
    ui->filteringComboBox->setCurrentText("Nearest");
    break;
  }

  const TextureSampling::TextureWrapping wrapping_mode = m_current_texture->getWrappingMode();
  switch(wrapping_mode) {
  case TextureSampling::TextureWrapping::REPEAT:
    ui->wrappingComboBox->setCurrentText("Repeat");
    break;
  case TextureSampling::TextureWrapping::MIRRORED_REPEAT:
    ui->wrappingComboBox->setCurrentText("Mirrored repeat");
    break;
  case TextureSampling::TextureWrapping::CLAMP_TO_EDGE:
    ui->wrappingComboBox->setCurrentText("Clamp to edge");
    break;
  case TextureSampling::TextureWrapping::CLAMP_TO_BORDER:
    ui->wrappingComboBox->setCurrentText("Clamp to border");
    ui->borderColorButton->setColor(m_current_texture->getBorderColor());
    break;
  }
  ui->borderColorLabel->setVisible(wrapping_mode == TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  ui->borderColorButton->setVisible(wrapping_mode == TextureSampling::TextureWrapping::CLAMP_TO_BORDER);

  updateTexturePreview();
}

void TexturesWidget::onAddButtonClicked() {
  m_textures_list_model->addItem(QString("New Texture"));
  const QModelIndex new_index = m_textures_list_model->index(m_textures_list_model->rowCount() - 1, 0);
  ui->texturesListView->setCurrentIndex(new_index);
}

void TexturesWidget::onRemoveButtonClicked() {
  if(m_current_texture != nullptr) {
    const QString                     texture_name = ui->texturesListView->currentIndex().data().toString();
    const QMessageBox::StandardButton reply        = QMessageBox::question(
        this, "Confirm Deletion", QString("Are you sure you want to delete the texture '%1'?").arg(texture_name),
        QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
      m_textures_list_model->removeItem(ui->texturesListView->currentIndex().row());
      updateWidget();
    }
  }
}

void TexturesWidget::onTextureRenamed(const QModelIndex& top_left, const QModelIndex& /*bottom_right*/,
                                      const QList<int>&  roles) {
  if(!roles.contains(Qt::EditRole)) {
    return;
  }
  const QModelIndex index    = m_textures_list_model->index(top_left.row(), 0);
  const QString     new_name = m_textures_list_model->data(index, Qt::DisplayRole).toString();
  ui->textureNameLineEdit->setText(new_name);
}

void TexturesWidget::onTextureSelectionChanged(const QItemSelection& selected, const QItemSelection& /*unused*/) {
  const QModelIndexList indexes = selected.indexes();
  if(!indexes.isEmpty()) {
    const QModelIndex index        = indexes.first();
    const QString     texture_name = index.data().toString();
    m_current_texture              = m_texture_manager->getTexture(texture_name.toStdString());
    ui->textureNameLineEdit->setText(texture_name);
  } else {
    m_current_texture = nullptr;
  }
  updateWidget();
}

void TexturesWidget::onTextureNameEdited() {
  const QString new_name = ui->textureNameLineEdit->text().trimmed();
  if(!new_name.isEmpty() && m_current_texture != nullptr) {
    const QString current_name = ui->texturesListView->currentIndex().data().toString();
    if(!m_textures_list_model->renameItem(ui->texturesListView->currentIndex().row(), new_name)) {
      ui->textureNameLineEdit->setText(current_name);
    }
  }
}

void TexturesWidget::onTextureTypeChanged(const QString& type) {
  m_current_texture->setTextureType(type == "Image Texture" ? TextureType::IMAGE_TEXTURE : TextureType::COLOR_TEXTURE);
  ui->colorLabel->setVisible(type == "Color Texture");
  ui->colorButton->setVisible(type == "Color Texture");
  ui->pathLabel->setVisible(type == "Image Texture");
  ui->pathWidget->setVisible(type == "Image Texture");
  ui->flipVerticallyLabel->setVisible(type == "Image Texture");
  ui->flipVerticallyCheckBox->setVisible(type == "Image Texture");

  updateTexturePreview();
}

void TexturesWidget::onColorChanged(const ColorRGB& new_color) {
  m_current_texture->setValue(new_color);
  updateTexturePreview();
}

void TexturesWidget::onTexturePathChanged(const QString& new_path) {
  m_current_texture->loadFromFile(new_path.toStdString().c_str());
  updateTexturePreview();
}

void TexturesWidget::onFlipVerticallyToggled(bool checked) {
  m_current_texture->setFlippedVertically(checked);
  updateTexturePreview();
}

void TexturesWidget::onColorSpaceChanged(const QString& color_space) {
  m_current_texture->setColorSpace(color_space == "Linear" ? ColorSpace::LINEAR : ColorSpace::S_RGB);
}

void TexturesWidget::onFilteringModeChanged(const QString& mode) {
  if(mode == "Bilinear") {
    m_current_texture->setFilteringMode(TextureSampling::TextureFiltering::BILINEAR);
  } else if(mode == "Nearest") {
    m_current_texture->setFilteringMode(TextureSampling::TextureFiltering::NEAREST);
  }
}

void TexturesWidget::onWrappingModeChanged(const QString& mode) {
  if(mode == "Repeat") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::REPEAT);
  } else if(mode == "Mirrored repeat") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::MIRRORED_REPEAT);
  } else if(mode == "Clamp to edge") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_EDGE);
  } else if(mode == "Clamp to border") {
    m_current_texture->setWrappingMode(TextureSampling::TextureWrapping::CLAMP_TO_BORDER);
  }
  ui->borderColorLabel->setVisible(mode == "Clamp to border");
  ui->borderColorButton->setVisible(mode == "Clamp to border");
}

void TexturesWidget::onBorderColorChanged(const ColorRGB& new_color) { m_current_texture->setBorderColor(new_color); }

TexturesWidget::~TexturesWidget() { delete ui; }