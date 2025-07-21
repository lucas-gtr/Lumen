#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalBlocker>

#include "Surface/MaterialManager.hpp"
#include "Surface/TextureManager.hpp"

#include "MaterialsWidget.hpp"
#include "ui_MaterialsWidget.h"

MaterialsWidget::MaterialsWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MaterialsWidget) {
  ui->setupUi(this);
  ui->diffusePreview->setMaximumSize(TEXTURE_PREVIEW_MAX_SIZE, TEXTURE_PREVIEW_MAX_SIZE);
  ui->normalPreview->setMaximumSize(TEXTURE_PREVIEW_MAX_SIZE, TEXTURE_PREVIEW_MAX_SIZE);

  updateWidget();

  connect(ui->addButton, &QPushButton::clicked, this, &MaterialsWidget::onAddButtonClicked);
  connect(ui->removeButton, &QPushButton::clicked, this, &MaterialsWidget::onRemoveButtonClicked);
  connect(ui->materialNameLineEdit, &QLineEdit::editingFinished, this, &MaterialsWidget::onMaterialNameEdited);
  connect(ui->diffuseTextureComboBox, &QComboBox::currentIndexChanged, this,
          &MaterialsWidget::onDiffuseTextureSelected);
  connect(ui->browseDiffuseButton, &QPushButton::clicked, this, &MaterialsWidget::onBrowseDiffuseButtonClicked);
  connect(ui->normalTextureComboBox, &QComboBox::currentIndexChanged, this, &MaterialsWidget::onNormalTextureSelected);
  connect(ui->browseNormalButton, &QPushButton::clicked, this, &MaterialsWidget::onBrowseNormalButtonClicked);
}

void MaterialsWidget::setMaterialManager(MaterialManager* material_manager) { m_material_manager = material_manager; }

void MaterialsWidget::setTextureManager(TextureManager* texture_manager) { m_texture_manager = texture_manager; }

void MaterialsWidget::setModel(MaterialsListModel* model) {
  ui->materialsListView->setModel(model);
  m_materials_list_model = model;

  connect(ui->materialsListView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
          &MaterialsWidget::onMaterialSelectionChanged);
  connect(m_materials_list_model, &MaterialsListModel::dataChanged, this, &MaterialsWidget::onMaterialRenamed);
}

void MaterialsWidget::setTexturesListModel(DefaultOptionProxyModel* model) {
  m_textures_list_model = model;
  ui->diffuseTextureComboBox->setModel(m_textures_list_model);
  ui->normalTextureComboBox->setModel(m_textures_list_model);
}

void MaterialsWidget::setDiffuseTexture(const QString& texture_name) {
  const int index = ui->diffuseTextureComboBox->findText(texture_name);
  if(index != -1) {
    ui->diffuseTextureComboBox->setCurrentIndex(index);
  }
}

void MaterialsWidget::setNormalTexture(const QString& texture_name) {
  const int index = ui->normalTextureComboBox->findText(texture_name);
  if(index != -1) {
    ui->normalTextureComboBox->setCurrentIndex(index);
  }
}

void MaterialsWidget::updateDiffuseTexturePreview() {
  if(m_current_material != nullptr && m_current_material->getDiffuseTexture() != nullptr) {
    ui->diffusePreview->setImage(m_current_material->getDiffuseTexture()->getPreviewData(),
                                 m_current_material->getDiffuseTexture()->getPreviewProperties());
  }
}

void MaterialsWidget::updateNormalTexturePreview() {
  if(m_current_material != nullptr && m_current_material->getNormalTexture() != nullptr) {
    ui->normalPreview->setImage(m_current_material->getNormalTexture()->getPreviewData(),
                                m_current_material->getNormalTexture()->getPreviewProperties());
  }
}

void MaterialsWidget::updateWidget() {
  const bool widgets_visible = (m_current_material != nullptr);

  ui->materialNameLineEdit->setVisible(widgets_visible);
  ui->materialSettingsWidget->setVisible(widgets_visible);
  ui->removeButton->setEnabled(widgets_visible);

  if(!widgets_visible) {
    return;
  }

  const QSignalBlocker diffuse_blocker(ui->diffuseTextureComboBox);
  const QSignalBlocker normal_blocker(ui->normalTextureComboBox);

  std::string current_diffuse_texture = m_texture_manager->getTextureName(m_current_material->getDiffuseTexture());
  std::string current_normal_texture  = m_texture_manager->getTextureName(m_current_material->getNormalTexture());

  if(current_diffuse_texture.empty()) {
    current_diffuse_texture = "Default";
  }
  if(current_normal_texture.empty()) {
    current_normal_texture = "Default";
  }

  ui->diffuseTextureComboBox->setCurrentText(QString::fromStdString(current_diffuse_texture));
  ui->normalTextureComboBox->setCurrentText(QString::fromStdString(current_normal_texture));

  updateDiffuseTexturePreview();
  updateNormalTexturePreview();
}

void MaterialsWidget::onAddButtonClicked() {
  m_materials_list_model->addItem(QString("New Material"));
  const QModelIndex new_index = m_materials_list_model->index(m_materials_list_model->rowCount() - 1, 0);
  ui->materialsListView->setCurrentIndex(new_index);
}

void MaterialsWidget::onRemoveButtonClicked() {
  if(m_current_material != nullptr) {
    const QString                     material_name = ui->materialsListView->currentIndex().data().toString();
    const QMessageBox::StandardButton reply         = QMessageBox::question(
        this, "Confirm Deletion", QString("Are you sure you want to delete the material '%1'?").arg(material_name),
        QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
      m_materials_list_model->removeItem(ui->materialsListView->currentIndex().row());
      updateWidget();
    }
  }
}

void MaterialsWidget::onMaterialRenamed(const QModelIndex& top_left, const QModelIndex& /*bottomRight*/,
                                        const QList<int>&  roles) {
  if(!roles.contains(Qt::EditRole)) {
    return;
  }
  const QModelIndex index    = m_materials_list_model->index(top_left.row(), 0);
  const QString     new_name = m_materials_list_model->data(index, Qt::DisplayRole).toString();
  ui->materialNameLineEdit->setText(new_name);
}

void MaterialsWidget::onMaterialSelectionChanged(const QItemSelection& selected, const QItemSelection& /*unused*/) {
  const QModelIndexList indexes = selected.indexes();
  if(!indexes.isEmpty()) {
    const QModelIndex index         = indexes.first();
    const QString     material_name = index.data().toString();
    m_current_material              = m_material_manager->getMaterial(material_name.toStdString());
    ui->materialNameLineEdit->setText(material_name);
  } else {
    m_current_material = nullptr;
  }
  updateWidget();
}

void MaterialsWidget::onMaterialNameEdited() {
  const QString new_name = ui->materialNameLineEdit->text().trimmed();
  if(!new_name.isEmpty() && m_current_material != nullptr) {
    const QString current_name = ui->materialsListView->currentIndex().data().toString();
    if(!m_materials_list_model->renameItem(ui->materialsListView->currentIndex().row(), new_name)) {
      ui->materialNameLineEdit->setText(current_name);
    }
  }
}

void MaterialsWidget::onDiffuseTextureSelected(int index) {
  if(index < 0 || index >= m_textures_list_model->rowCount()) {
    return;
  }

  const QString texture_name =
      m_textures_list_model->data(m_textures_list_model->index(index, 0), Qt::DisplayRole).toString();
  if(m_current_material != nullptr) {
    if(texture_name == "Default") {
      m_current_material->setDiffuseTexture(TextureManager::DefaultDiffuseTexture());
    } else {
      Texture* texture = m_texture_manager->getTexture(texture_name.toStdString());
      m_current_material->setDiffuseTexture(texture);
    }
    updateDiffuseTexturePreview();
  }
}

void MaterialsWidget::onBrowseDiffuseButtonClicked() {
  QString texture_path;

  texture_path = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga)");

  if(!texture_path.isEmpty()) {
    emit diffuseTextureCreated(texture_path);
  }
}

void MaterialsWidget::onNormalTextureSelected(int index) {
  if(index < 0 || index >= m_textures_list_model->rowCount()) {
    return;
  }

  const QString texture_name =
      m_textures_list_model->data(m_textures_list_model->index(index, 0), Qt::DisplayRole).toString();
  if(m_current_material != nullptr) {
    if(texture_name == "Default") {
      m_current_material->setNormalTexture(TextureManager::DefaultNormalTexture());
    } else {
      Texture* texture = m_texture_manager->getTexture(texture_name.toStdString());
      m_current_material->setNormalTexture(texture);
    }
    updateNormalTexturePreview();
  }
}

void MaterialsWidget::onBrowseNormalButtonClicked() {
  QString texture_path;

  texture_path = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga)");

  if(!texture_path.isEmpty()) {
    emit normalTextureCreated(texture_path);
  }
}

MaterialsWidget::~MaterialsWidget() { delete ui; }