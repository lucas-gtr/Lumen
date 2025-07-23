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

  setupDoubleSliderSpinBox(ui->emissiveStrengthSliderSpinBox, 0.0, MAX_EMISSIVE_STRENGTH);
  setupDoubleSliderSpinBox(ui->roughnessDoubleSliderSpinBox, 0.0, 1.0);
  setupDoubleSliderSpinBox(ui->metalDoubleSliderSpinBox, 0.0, 1.0);

  setPreviewsMaximumSize(TEXTURE_PREVIEW_MAX_SIZE);

  updateWidget();

  initializeTextureBindings();

  setupConnections();
}

void MaterialsWidget::setupDoubleSliderSpinBox(DoubleSliderSpinBox* slider_spin_box, double min_value,
                                               double max_value) {
  slider_spin_box->setMinimum(min_value);
  slider_spin_box->setMaximum(max_value);
}

void MaterialsWidget::setPreviewsMaximumSize(int size) {
  const QSize max_size(size, size);
  ui->diffusePreview->setMaximumSize(max_size);
  ui->normalPreview->setMaximumSize(max_size);
  ui->emissivePreview->setMaximumSize(max_size);
  ui->roughnessPreview->setMaximumSize(max_size);
  ui->metalPreview->setMaximumSize(max_size);
}

void MaterialsWidget::initializeTextureBindings() {
  m_diffuse_binding = {ui->diffuseTextureComboBox,
                       ui->browseDiffuseButton,
                       ui->diffusePreview,
                       TextureManager::DefaultDiffuseTexture(),
                       [this](const QString& p) { emit diffuseTextureCreated(p); },
                       [this](Texture* texture) {
                         if(m_current_material != nullptr) {
                           m_current_material->setDiffuseTexture(texture);
                           updateTexturePreview(texture, ui->diffusePreview);
                         }
                       }};

  m_normal_binding = {ui->normalTextureComboBox,
                      ui->browseNormalButton,
                      ui->normalPreview,
                      TextureManager::DefaultNormalTexture(),
                      [this](const QString& p) { emit normalTextureCreated(p); },
                      [this](Texture* texture) {
                        if(m_current_material != nullptr) {
                          m_current_material->setNormalTexture(texture);
                          updateTexturePreview(texture, ui->normalPreview);
                        }
                      }};

  m_emissive_binding = {ui->emissiveTextureComboBox,
                        ui->browseEmissiveButton,
                        ui->emissivePreview,
                        TextureManager::DefaultBlackTexture(),
                        [this](const QString& p) { emit emissiveTextureCreated(p); },
                        [this](Texture* texture) {
                          if(m_current_material != nullptr) {
                            m_current_material->setEmissiveTexture(texture);
                            updateTexturePreview(texture, ui->emissivePreview);
                          }
                        }};

  m_roughness_binding = {ui->roughnessTextureComboBox,
                         ui->browseRoughnessButton,
                         ui->roughnessPreview,
                         TextureManager::DefaultMidGrayTexture(),
                         [this](const QString& p) { emit roughnessTextureCreated(p); },
                         [this](Texture* texture) {
                           if(m_current_material != nullptr) {
                             m_current_material->setRoughnessTexture(texture);
                             updateTexturePreview(texture, ui->roughnessPreview);
                           }
                         }};

  m_metal_binding = {ui->metalTextureComboBox,
                     ui->browseMetalButton,
                     ui->metalPreview,
                     TextureManager::DefaultBlackTexture(),
                     [this](const QString& p) { emit metalTextureCreated(p); },
                     [this](Texture* texture) {
                       if(m_current_material != nullptr) {
                         m_current_material->setMetallicTexture(texture);
                         updateTexturePreview(texture, ui->metalPreview);
                       }
                     }};

  setupTextureBinding(&m_diffuse_binding);
  setupTextureBinding(&m_normal_binding);
  setupTextureBinding(&m_emissive_binding);
  setupTextureBinding(&m_roughness_binding);
  setupTextureBinding(&m_metal_binding);
}

void MaterialsWidget::setupConnections() {
  connect(ui->addButton, &QPushButton::clicked, this, &MaterialsWidget::onAddButtonClicked);
  connect(ui->removeButton, &QPushButton::clicked, this, &MaterialsWidget::onRemoveButtonClicked);
  connect(ui->materialNameLineEdit, &QLineEdit::editingFinished, this, &MaterialsWidget::onMaterialNameEdited);

  connect(ui->emissiveStrengthSliderSpinBox, &DoubleSliderSpinBox::valueChanged, this,
          &MaterialsWidget::onEmissiveStrengthChanged);

  connect(ui->roughnessDoubleSliderSpinBox, &DoubleSliderSpinBox::valueChanged, this,
          &MaterialsWidget::onRoughnessValueChanged);
  connect(ui->roughnessUseTextureCheckbox, &QCheckBox::toggled, this, &MaterialsWidget::onUseTextureRoughnessChanged);

  connect(ui->metalDoubleSliderSpinBox, &DoubleSliderSpinBox::valueChanged, this,
          &MaterialsWidget::onMetallicValueChanged);
  connect(ui->metalUseTextureCheckbox, &QCheckBox::toggled, this, &MaterialsWidget::onUseTextureMetallicChanged);
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
  ui->emissiveTextureComboBox->setModel(m_textures_list_model);
  ui->roughnessTextureComboBox->setModel(m_textures_list_model);
  ui->metalTextureComboBox->setModel(m_textures_list_model);
}

void MaterialsWidget::setTexture(QComboBox* combo_box, const QString& name) {
  const int index = combo_box->findText(name);
  if(index != -1) {
    combo_box->setCurrentIndex(index);
  }
}

void MaterialsWidget::setDiffuseTexture(const QString& texture_name) {
  setTexture(ui->diffuseTextureComboBox, texture_name);
}

void MaterialsWidget::setNormalTexture(const QString& texture_name) {
  setTexture(ui->normalTextureComboBox, texture_name);
}

void MaterialsWidget::setEmissiveTexture(const QString& texture_name) {
  setTexture(ui->emissiveTextureComboBox, texture_name);
}

void MaterialsWidget::setRoughnessTexture(const QString& texture_name) {
  setTexture(ui->roughnessTextureComboBox, texture_name);
}

void MaterialsWidget::setMetalTexture(const QString& texture_name) {
  setTexture(ui->metalTextureComboBox, texture_name);
}

void MaterialsWidget::setupTextureBinding(TextureBinding* binding) {
  connect(binding->combo_box, &QComboBox::currentIndexChanged, this, [this, binding](int index) {
    if(index < 0 || index >= m_textures_list_model->rowCount()) {
      return;
    }

    const QString texture_name =
        m_textures_list_model->data(m_textures_list_model->index(index, 0), Qt::DisplayRole).toString();
    if(m_current_material != nullptr) {
      if(texture_name == "Default") {
        binding->materialTextureSetter(binding->default_texture);
      } else {
        Texture* texture = m_texture_manager->getTexture(texture_name.toStdString());
        binding->materialTextureSetter(texture);
      }
    }
  });

  connect(binding->browse_button, &QPushButton::clicked, this, [this, binding]() {
    const QString texture_path =
        QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga)");
    if(!texture_path.isEmpty()) {
      binding->emitTextureCreatedSignal(texture_path);
    }
  });
}

void MaterialsWidget::updateWidget() {
  const bool widgets_visible = (m_current_material != nullptr);

  ui->materialNameLineEdit->setVisible(widgets_visible);
  ui->materialSettingsWidget->setVisible(widgets_visible);
  ui->removeButton->setEnabled(widgets_visible);

  if(!widgets_visible) {
    return;
  }

  updateTextureWidget(ui->diffuseTextureComboBox, ui->diffusePreview, m_current_material->getDiffuseTexture());
  updateTextureWidget(ui->normalTextureComboBox, ui->normalPreview, m_current_material->getNormalTexture());
  updateTextureWidget(ui->emissiveTextureComboBox, ui->emissivePreview, m_current_material->getEmissiveTexture());

  const QSignalBlocker blocker_roughness_checkbox(ui->roughnessUseTextureCheckbox);
  const QSignalBlocker blocker_roughness_slider(ui->roughnessDoubleSliderSpinBox);
  const QSignalBlocker blocker_metal_checkbox(ui->metalUseTextureCheckbox);
  const QSignalBlocker blocker_metal_slider(ui->metalDoubleSliderSpinBox);

  ui->emissiveStrengthSliderSpinBox->setValue(m_current_material->getEmissiveIntensity());

  const bool is_using_roughness_texture = m_current_material->isUsingTextureRoughness();
  ui->roughnessUseTextureCheckbox->setChecked(is_using_roughness_texture);
  ui->roughnessPreview->setVisible(is_using_roughness_texture);
  ui->roughnessStackedWidget->setCurrentIndex(is_using_roughness_texture ? 0 : 1);
  if(m_current_material->isUsingTextureRoughness()) {
    updateTextureWidget(ui->roughnessTextureComboBox, ui->roughnessPreview, m_current_material->getRoughnessTexture());
  } else {
    ui->roughnessDoubleSliderSpinBox->setValue(m_current_material->getRoughness({0, 0}));
  }

  const bool is_using_metallic_texture = m_current_material->isUsingTextureMetallic();
  ui->metalUseTextureCheckbox->setChecked(is_using_metallic_texture);
  ui->metalPreview->setVisible(is_using_metallic_texture);
  ui->metalStackedWidget->setCurrentIndex(is_using_metallic_texture ? 0 : 1);
  if(m_current_material->isUsingTextureMetallic()) {
    updateTextureWidget(ui->metalTextureComboBox, ui->metalPreview, m_current_material->getMetallicTexture());
  } else {
    ui->metalDoubleSliderSpinBox->setValue(m_current_material->getMetallic({0, 0}));
  }
}

void MaterialsWidget::updateTextureWidget(QComboBox* combo_box, TexturePreview* preview_label, Texture* texture) {
  const QSignalBlocker blocker(combo_box);
  const std::string    texture_name = m_texture_manager->getTextureName(texture);
  combo_box->setCurrentText(QString::fromStdString(texture_name));
  updateTexturePreview(texture, preview_label);
}

void MaterialsWidget::updateTexturePreview(Texture* texture, TexturePreview* preview_label) {
  if(texture != nullptr) {
    preview_label->setImage(texture->getPreviewData(), texture->getPreviewProperties());
  } else {
    std::cout << "Warning: Texture is null, cannot update preview.\n";
  }
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

void MaterialsWidget::onEmissiveStrengthChanged(double value) {
  if(m_current_material != nullptr) {
    m_current_material->setEmissiveIntensity(value);
  }
}

void MaterialsWidget::onUseTextureRoughnessChanged(bool checked) {
  ui->roughnessStackedWidget->setCurrentIndex(checked ? 0 : 1);
  ui->roughnessPreview->setVisible(checked);
  m_current_material->setUseTextureRoughness(checked);
  if(checked) {
    updateTextureWidget(ui->roughnessTextureComboBox, ui->roughnessPreview, m_current_material->getRoughnessTexture());
  } else {
  }
}

void MaterialsWidget::onRoughnessValueChanged(double value) {
  if(m_current_material != nullptr) {
    m_current_material->setRoughnessValue(value);
  }
}

void MaterialsWidget::onUseTextureMetallicChanged(bool checked) {
  ui->metalStackedWidget->setCurrentIndex(checked ? 0 : 1);
  ui->metalPreview->setVisible(checked);
  m_current_material->setUseTextureMetallic(checked);
  if(checked) {
    updateTextureWidget(ui->metalTextureComboBox, ui->metalPreview, m_current_material->getMetallicTexture());
  }
}

void MaterialsWidget::onMetallicValueChanged(double value) {
  if(m_current_material != nullptr) {
    m_current_material->setMetallicValue(value);
  }
}

MaterialsWidget::~MaterialsWidget() { delete ui; }