#include <QFileDialog>

#include "SkyboxWidget.hpp"
#include "ui_SkyboxWidget.h"

#include "Scene/Skybox.hpp"

SkyboxWidget::SkyboxWidget(QWidget* parent) : QWidget(parent), ui(new Ui::SkyboxWidget) {
  ui->setupUi(this);

  connect(ui->textureComboBox, &QComboBox::currentIndexChanged, this, &SkyboxWidget::onTextureSelected);
  connect(ui->browseButton, &QPushButton::clicked, this, &SkyboxWidget::onBrowseButtonClicked);
  connect(ui->displayViewportCheckBox, &QCheckBox::toggled, this, &SkyboxWidget::displayInViewportToggled);
}

void SkyboxWidget::setSkybox(Skybox* skybox) {
  m_skybox = skybox;
  updateWidget();
}

void SkyboxWidget::setTexturesListModel(DefaultOptionProxyModel* model) {
  m_textures_list_model = model;
  ui->textureComboBox->setModel(m_textures_list_model);
}

void SkyboxWidget::setCurrentTexture(const QString& texture_name) {
  const int index = ui->textureComboBox->findText(texture_name);
  if(index != -1) {
    ui->textureComboBox->setCurrentIndex(index);
  }
}

void SkyboxWidget::updateWidget() {
  if(m_skybox == nullptr) {
    return;
  }
  updateSkyboxPreview();
}

void SkyboxWidget::updateSkyboxPreview() {
  if(m_skybox != nullptr) {
    ui->texturePreview->setImage(m_skybox->getTexture()->getPreviewData(),
                                 m_skybox->getTexture()->getPreviewProperties());
  }
}

void SkyboxWidget::onTextureSelected(int index) {
  if(index < 0 || index >= m_textures_list_model->rowCount()) {
    return;
  }

  const QString texture_name =
      m_textures_list_model->data(m_textures_list_model->index(index, 0), Qt::DisplayRole).toString();
  emit skyboxChanged(texture_name);
}

void SkyboxWidget::onBrowseButtonClicked() {
  QString selected_image_path;

  selected_image_path =
      QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga *.hdr)");

  if(!selected_image_path.isEmpty()) {
    emit skyboxTextureCreated(selected_image_path);
  }
}

SkyboxWidget::~SkyboxWidget() { delete ui; }