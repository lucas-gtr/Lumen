
#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtreewidget.h>
#include <qwidget.h>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Qt/TexturePreviewWidget.hpp"
#include "Qt/TextureSelectorWidget.hpp"
#include "Qt/View/MaterialView.hpp"

MaterialView::MaterialView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void MaterialView::setupUI() {
  auto* group_box = new QGroupBox("Material Manager", this);
  group_box->setStyleSheet("QGroupBox {font-weight: bold; font-size: 14px;}");

  m_material_list = new QTreeWidget(group_box);
  m_material_list->setHeaderHidden(true);
  m_material_list->setMinimumWidth(TEXTURE_MATERIAL_SELECTOR_MINIMUM_WIDTH);
  m_material_list->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

  m_add_button = new QPushButton("+", group_box);
  m_add_button->setFixedSize(ADD_REMOVE_BUTTON_WIDTH, ADD_REMOVE_BUTTON_HEIGHT);
  m_remove_button = new QPushButton("-", group_box);
  m_remove_button->setFixedSize(ADD_REMOVE_BUTTON_WIDTH, ADD_REMOVE_BUTTON_HEIGHT);

  auto* buttons_layout = new QVBoxLayout;
  buttons_layout->addWidget(m_add_button);
  buttons_layout->addWidget(m_remove_button);
  buttons_layout->addStretch();

  auto* tree_layout = new QHBoxLayout;
  tree_layout->addWidget(m_material_list);
  tree_layout->addLayout(buttons_layout);

  m_name_edit = new QLineEdit(group_box);
  m_name_edit->setPlaceholderText("Material name");

  m_diffuse_image_texture_widget = new TexturePreviewWidget(MATERIAL_VIEW_TEXTURE_PREVIEW_MAX_SIZE,
                                                            MATERIAL_VIEW_TEXTURE_PREVIEW_MAX_SIZE, group_box);
  m_diffuse_texture_selector     = new TextureSelectorWidget(DEFAULT_DIFFUSE_TEXTURE_NAME, group_box);

  auto* diffuse_layout = new QHBoxLayout;
  auto* diffuse_label  = new QLabel("Diffuse ", group_box);
  diffuse_layout->addWidget(diffuse_label);
  diffuse_layout->addWidget(m_diffuse_texture_selector);

  m_normal_image_texture_widget = new TexturePreviewWidget(MATERIAL_VIEW_TEXTURE_PREVIEW_MAX_SIZE,
                                                           MATERIAL_VIEW_TEXTURE_PREVIEW_MAX_SIZE, group_box);
  m_normal_texture_selector     = new TextureSelectorWidget(DEFAULT_NORMAL_TEXTURE_NAME, group_box);

  auto* normal_layout = new QHBoxLayout;
  auto* normal_label  = new QLabel("Normal ", group_box);
  normal_layout->addWidget(normal_label);
  normal_layout->addWidget(m_normal_texture_selector);

  auto* material_details_layout = new QVBoxLayout;
  material_details_layout->addWidget(m_name_edit);
  material_details_layout->addWidget(m_diffuse_image_texture_widget, 0, Qt::AlignHCenter);
  material_details_layout->addLayout(diffuse_layout);
  material_details_layout->addWidget(m_normal_image_texture_widget, 0, Qt::AlignHCenter);
  material_details_layout->addLayout(normal_layout);

  m_material_details_widget = new QWidget(group_box);
  m_material_details_widget->setLayout(material_details_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addLayout(tree_layout);
  main_layout->addWidget(m_material_details_widget);

  group_box->setLayout(main_layout);

  auto* outer_layout = new QVBoxLayout;
  outer_layout->addWidget(group_box);
  outer_layout->addStretch();

  setLayout(outer_layout);

  m_material_details_widget->setVisible(false);
  m_remove_button->setEnabled(false);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void MaterialView::connectSignals() {
  connect(m_material_list, &QTreeWidget::itemSelectionChanged, this, &MaterialView::onMaterialSelected);
  connect(m_material_list, &QTreeWidget::itemChanged, this, &MaterialView::onMaterialItemChanged);
  connect(m_add_button, &QPushButton::clicked, this, &MaterialView::onAddButtonClicked);
  connect(m_remove_button, &QPushButton::clicked, this, &MaterialView::materialRemoved);
  connect(m_name_edit, &QLineEdit::editingFinished, this, &MaterialView::onMaterialNameEdited);

  connect(m_diffuse_texture_selector, &TextureSelectorWidget::textureSelected, this,
          &MaterialView::onDiffuseTextureSelected);
  connect(m_normal_texture_selector, &TextureSelectorWidget::textureSelected, this,
          &MaterialView::onNormalTextureSelected);
  connect(m_diffuse_texture_selector, &TextureSelectorWidget::textureCreationRequested, this,
          &MaterialView::diffuseTextureImageCreated);
  connect(m_normal_texture_selector, &TextureSelectorWidget::textureCreationRequested, this,
          &MaterialView::normalTextureImageCreated);
}

void MaterialView::onAddButtonClicked() {
  const QString new_material_name = "New Material";
  emit          materialAdded(new_material_name);
}

void MaterialView::onMaterialSelected() {
  auto selected_items = m_material_list->selectedItems();
  if(!selected_items.isEmpty()) {
    const QTreeWidgetItem* item          = selected_items.first();
    const QString          material_name = item->text(0);
    m_name_edit->setText(material_name);
    m_remove_button->setEnabled(false);
    m_material_details_widget->setVisible(true);
    emit materialSelected(material_name);
  } else {
    m_name_edit->clear();
    m_remove_button->setEnabled(false);
    m_material_details_widget->setVisible(false);
    emit materialUnselected();
  }
}

void MaterialView::onMaterialItemChanged(QTreeWidgetItem* item, int column) {
  if(column == 0) {
    const QString new_name = item->text(0).trimmed();
    if(!new_name.isEmpty()) {
      emit materialNameEdited(new_name);
    }
  }
}

void MaterialView::onMaterialNameEdited() {
  const QString new_name = m_name_edit->text();
  if(!new_name.isEmpty()) {
    emit materialNameEdited(new_name);
  }
}

void MaterialView::onDiffuseTextureSelected(const QString& texture_name) { emit diffuseTextureSelected(texture_name); }

void MaterialView::onNormalTextureSelected(const QString& texture_name) { emit normalTextureSelected(texture_name); }

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void MaterialView::onMaterialAdded(const QString& material_name) {
  m_material_list->blockSignals(true);
  auto* item = new QTreeWidgetItem();
  item->setText(0, material_name);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  m_material_list->addTopLevelItem(item);
  m_material_list->blockSignals(false);
  m_material_list->setCurrentItem(item);
}

void MaterialView::onMaterialRemoved(const QString& material_name) {
  auto selected_items = m_material_list->selectedItems();
  if(!selected_items.isEmpty()) {
    QTreeWidgetItem* item = selected_items.first();
    if(item->text(0) == material_name) {
      delete item;
    }
  }
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void MaterialView::onMaterialRenamed(const QString& /*old_name*/, const QString& new_name) {
  const QSignalBlocker blocker(m_material_list);
  const QSignalBlocker name_blocker(m_name_edit);
  auto                 selected_items = m_material_list->selectedItems();
  if(!selected_items.isEmpty()) {
    QTreeWidgetItem* item = selected_items.first();
    item->setText(0, new_name);
    m_name_edit->setText(new_name);
  }
}

void MaterialView::onDiffuseTextureImageChanged(const unsigned char*   image_data,
                                                const ImageProperties& preview_properties) {
  m_diffuse_image_texture_widget->setTexturePreview(image_data, preview_properties);
}

void MaterialView::onNormalTextureImageChanged(const unsigned char*   image_data,
                                               const ImageProperties& preview_properties) {
  m_normal_image_texture_widget->setTexturePreview(image_data, preview_properties);
}

void MaterialView::setDiffuseTexture(const QString& texture_name) {
  const QSignalBlocker blocker(m_diffuse_texture_selector);
  m_diffuse_texture_selector->setCurrentTexture(texture_name);
}

void MaterialView::setNormalTexture(const QString& texture_name) {
  const QSignalBlocker blocker(m_normal_texture_selector);
  m_normal_texture_selector->setCurrentTexture(texture_name);
}
