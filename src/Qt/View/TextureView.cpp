#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpalette.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qtreewidget.h>
#include <qwidget.h>

#include "Core/CommonTypes.hpp"
#include "Core/Config.hpp"
#include "Qt/TexturePreviewWidget.hpp"
#include "Qt/View/TextureView.hpp"

TextureView::TextureView(QWidget* parent) : QWidget(parent) {
  setupUI();
  connectSignals();
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void TextureView::setupUI() {
  auto* group_box = new QGroupBox("Texture Manager", this);
  group_box->setStyleSheet("QGroupBox {font-weight: bold; font-size: 14px;}");

  m_texture_list = new QTreeWidget(group_box);
  m_texture_list->setHeaderHidden(true);
  m_texture_list->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

  m_add_button = new QPushButton("+", group_box);
  m_add_button->setFixedSize(ADD_REMOVE_BUTTON_WIDTH, ADD_REMOVE_BUTTON_HEIGHT);
  m_remove_button = new QPushButton("-", group_box);
  m_remove_button->setFixedSize(ADD_REMOVE_BUTTON_WIDTH, ADD_REMOVE_BUTTON_HEIGHT);

  auto* buttons_layout = new QVBoxLayout;
  buttons_layout->addWidget(m_add_button);
  buttons_layout->addWidget(m_remove_button);
  buttons_layout->addStretch();

  auto* tree_layout = new QHBoxLayout;
  tree_layout->addWidget(m_texture_list);
  tree_layout->addLayout(buttons_layout);

  m_name_edit = new QLineEdit(group_box);
  m_name_edit->setPlaceholderText("Texture name");

  m_preview_image_widget =
      new TexturePreviewWidget(TEXTURE_VIEW_TEXTURE_PREVIEW_MAX_SIZE, TEXTURE_VIEW_TEXTURE_PREVIEW_MAX_SIZE, group_box);

  m_texture_type_combo = new QComboBox(group_box);
  m_texture_type_combo->addItems({"Image Texture", "Color Texture"});

  m_texture_color_label  = new QLabel("Color ", group_box);
  m_texture_color_button = new QPushButton(group_box);
  m_texture_color_button->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);
  m_texture_color_button->setAutoFillBackground(true);

  m_format_label = new QLabel("Format:", group_box);
  m_format_combo = new QComboBox(group_box);
  m_format_combo->addItems({"Grayscale", "RGB", "RGBA"});

  m_path_label    = new QLabel("Path ", group_box);
  m_path_edit     = new QLineEdit(group_box);
  m_browse_button = new QPushButton("Browse", group_box);

  auto* path_layout = new QHBoxLayout;
  path_layout->addWidget(m_path_edit);
  path_layout->addWidget(m_browse_button);
  path_layout->setContentsMargins(0, 0, 0, 0);

  auto* path_widget = new QWidget(group_box);
  path_widget->setLayout(path_layout);

  m_flip_vertically_label    = new QLabel("Flip Vertically ", group_box);
  m_flip_vertically_checkbox = new QCheckBox(group_box);
  m_flip_vertically_checkbox->setChecked(false);

  m_color_space_combo = new QComboBox(group_box);
  m_color_space_combo->addItems({"Linear", "sRGB"});
  m_color_space_combo->setCurrentIndex(0);

  m_filtering_combo = new QComboBox(group_box);
  m_filtering_combo->addItems({"Bilinear", "Nearest"});
  m_filtering_combo->setCurrentIndex(0);

  m_wrapping_combo = new QComboBox(group_box);
  m_wrapping_combo->addItems({"Repeat", "Mirrored repeat", "Clamp to edge", "Clamp to border"});

  m_border_color_label  = new QLabel("Border ", group_box);
  m_border_color_button = new QPushButton(group_box);
  m_border_color_button->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);
  m_border_color_button->setAutoFillBackground(true);

  auto* form_layout = new QFormLayout;
  form_layout->addRow("Type ", m_texture_type_combo);
  form_layout->addRow(m_format_label, m_format_combo);
  form_layout->addRow(m_texture_color_label, m_texture_color_button);
  form_layout->addRow(m_path_label, path_widget);
  form_layout->addRow(m_flip_vertically_label, m_flip_vertically_checkbox);
  form_layout->addRow("Color Space ", m_color_space_combo);
  form_layout->addRow("Filtering ", m_filtering_combo);
  form_layout->addRow("Wrapping ", m_wrapping_combo);
  form_layout->addRow(m_border_color_label, m_border_color_button);

  m_texture_details_widget = new QWidget(group_box);
  m_texture_details_widget->setLayout(form_layout);

  auto* texture_parameters_layout = new QVBoxLayout;
  texture_parameters_layout->addWidget(m_name_edit);
  texture_parameters_layout->addWidget(m_preview_image_widget, 0, Qt::AlignHCenter);
  texture_parameters_layout->addWidget(m_texture_details_widget);

  auto* content_layout = new QVBoxLayout;
  content_layout->addLayout(tree_layout);
  content_layout->addLayout(texture_parameters_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addLayout(content_layout);
  group_box->setLayout(main_layout);

  auto* outer_layout = new QVBoxLayout;
  outer_layout->addWidget(group_box);
  outer_layout->addStretch();
  setLayout(outer_layout);

  m_texture_color_button->setVisible(false);
  m_texture_color_label->setVisible(false);
  m_format_label->setVisible(false);
  m_format_combo->setVisible(false);
  m_preview_image_widget->setVisible(false);
  m_name_edit->setVisible(false);
  m_texture_details_widget->setVisible(false);
  m_remove_button->setEnabled(false);
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void TextureView::connectSignals() {
  connect(m_texture_list, &QTreeWidget::itemSelectionChanged, this, &TextureView::onTextureSelected);
  connect(m_texture_list, &QTreeWidget::itemChanged, this, &TextureView::onTextureItemChanged);
  connect(m_name_edit, &QLineEdit::editingFinished, this, &TextureView::onTextureNameEdited);
  connect(m_add_button, &QPushButton::clicked, this, &TextureView::onAddButtonClicked);
  connect(m_remove_button, &QPushButton::clicked, this, &TextureView::textureRemoved);
  connect(m_texture_type_combo, &QComboBox::currentTextChanged, this, &TextureView::onTextureTypeChanged);
  connect(m_format_combo, &QComboBox::currentTextChanged, this, &TextureView::textureFormatChanged);
  connect(m_texture_color_button, &QPushButton::clicked, this, &TextureView::onTextureColorButtonClicked);
  connect(m_path_edit, &QLineEdit::textEdited, this, &TextureView::texturePathChanged);
  connect(m_browse_button, &QPushButton::clicked, this, &TextureView::onBrowseButtonClicked);
  connect(m_color_space_combo, &QComboBox::currentTextChanged, this, &TextureView::colorSpaceChanged);
  connect(m_filtering_combo, &QComboBox::currentTextChanged, this, &TextureView::filteringChanged);
  connect(m_wrapping_combo, &QComboBox::currentTextChanged, this, &TextureView::onWrappingModeChanged);
  connect(m_border_color_button, &QPushButton::clicked, this, &TextureView::onBorderColorButtonClicked);
  connect(m_flip_vertically_checkbox, &QCheckBox::toggled, this, &TextureView::flipVerticallyChanged);
}

void TextureView::updateTextureTypeVisibility(const QString& type) {
  const bool is_color = (type == "Color Texture");

  m_texture_color_label->setVisible(is_color);
  m_texture_color_button->setVisible(is_color);
  m_format_label->setVisible(is_color);
  m_format_combo->setVisible(is_color);

  m_path_label->setVisible(!is_color);
  m_path_edit->setVisible(!is_color);
  m_browse_button->setVisible(!is_color);
  m_flip_vertically_checkbox->setVisible(!is_color);
}

void TextureView::onAddButtonClicked() { emit textureAdded(QString("New Texture")); }

void TextureView::onTextureTypeChanged(const QString& type) {
  updateTextureTypeVisibility(type);

  emit textureTypeChanged(type);
}

void TextureView::updateBorderColorVisibility(const QString& mode) {
  const bool show_border_color = (mode == "Clamp to border");

  m_border_color_label->setVisible(show_border_color);
  m_border_color_button->setVisible(show_border_color);
}

void TextureView::onWrappingModeChanged(const QString& mode) {
  updateBorderColorVisibility(mode);

  emit wrappingModeChanged(mode);
}

void TextureView::onTextureSelected() {
  auto selected_items = m_texture_list->selectedItems();
  if(!selected_items.isEmpty()) {
    const QTreeWidgetItem* item         = selected_items.first();
    const QString          texture_name = item->text(0);
    m_name_edit->setText(texture_name);
    m_name_edit->setVisible(true);
    m_preview_image_widget->setVisible(true);
    m_texture_details_widget->setVisible(true);
    m_remove_button->setEnabled(false);
    emit textureSelected(texture_name);
  } else {
    m_name_edit->setVisible(false);
    m_preview_image_widget->setVisible(false);
    m_texture_details_widget->setVisible(false);
    m_remove_button->setEnabled(false);
    emit textureUnselected();
  }
}

void TextureView::onTextureNameEdited() {
  const QString new_name = m_name_edit->text().trimmed();
  if(!new_name.isEmpty()) {
    emit textureNameEdited(new_name);
  }
}

void TextureView::onTextureItemChanged(QTreeWidgetItem* item, int column) {
  if(column == 0) {
    const QString new_name = item->text(0).trimmed();
    if(!new_name.isEmpty()) {
      emit textureNameEdited(new_name);
    }
  }
}

void TextureView::onBrowseButtonClicked() {
  const QString file_name =
      QFileDialog::getOpenFileName(this, "Select Texture File", "", "Images (*.png *.jpg *.jpeg *.bmp *.tga)");
  if(!file_name.isEmpty()) {
    m_path_edit->setText(file_name);
    emit texturePathChanged(file_name);
  }
}

void TextureView::onTextureColorButtonClicked() {
  const QColor color = QColorDialog::getColor(Qt::white, this, "Select Texture Color");
  if(color.isValid()) {
    QPalette palette = m_texture_color_button->palette();
    palette.setColor(QPalette::Button, color);
    m_texture_color_button->setPalette(palette);
    emit textureColorChanged(color);
  }
}

void TextureView::onBorderColorButtonClicked() {
  const QColor color = QColorDialog::getColor(Qt::black, this, "Select Border Color");
  if(color.isValid()) {
    QPalette palette = m_border_color_button->palette();
    palette.setColor(QPalette::Button, color);
    m_border_color_button->setPalette(palette);
    emit borderColorChanged(color);
  }
}

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void TextureView::onTextureAdded(const QString& name) {
  m_texture_list->blockSignals(true);
  auto* item = new QTreeWidgetItem();
  item->setText(0, name);
  item->setFlags(item->flags() | Qt::ItemIsEditable);

  m_texture_list->addTopLevelItem(item);
  m_texture_list->blockSignals(false);
  m_texture_list->setCurrentItem(item);
}

void TextureView::onTextureRemoved(const QString& /*name*/) {
  auto selected_items = m_texture_list->selectedItems();
  if(!selected_items.isEmpty()) {
    QTreeWidgetItem* item  = selected_items.first();
    const int        index = m_texture_list->indexOfTopLevelItem(item);
    if(index != -1) {
      QTreeWidgetItem* removed_item = m_texture_list->takeTopLevelItem(index);
      delete removed_item;
    }
  }
}
// NOLINTEND(cppcoreguidelines-owning-memory)

void TextureView::onTextureRenamed(const QString& new_name, const QString& /*old_name*/) {
  const QSignalBlocker blocker(m_texture_list);
  const QSignalBlocker name_blocker(m_name_edit);
  auto                 selected_items = m_texture_list->selectedItems();
  if(!selected_items.isEmpty()) {
    QTreeWidgetItem* item = selected_items.first();
    item->setText(0, new_name);
    m_name_edit->setText(new_name);
  }
}

void TextureView::onTextureImageChanged(const unsigned char* preview_data, const ImageProperties& preview_properties) {
  m_preview_image_widget->setTexturePreview(preview_data, preview_properties);

  if(preview_properties.width != 1 || preview_properties.height != 1) {
    return;
  }

  const int channels = preview_properties.channels;

  QColor color;
  switch(channels) {
  case 1:
    color = QColor(preview_data[0], preview_data[0], preview_data[0]);
    break;
  case 3:
    color = QColor(preview_data[0], preview_data[1], preview_data[2]);
    break;
  case 4:
  default:
    color = QColor(preview_data[0], preview_data[1], preview_data[2], preview_data[3]);
    break;
  }
  setTextureColorButton(color);
}

void TextureView::setColorSpace(const QString& color_space) {
  const QSignalBlocker blocker(m_color_space_combo);
  const int            index = m_color_space_combo->findText(color_space);
  if(index != -1) {
    m_color_space_combo->setCurrentIndex(index);
  }
}

void TextureView::setTextureType(const QString& type) {
  const QSignalBlocker blocker(m_texture_type_combo);
  const int            index = m_texture_type_combo->findText(type);
  if(index != -1) {
    m_texture_type_combo->setCurrentIndex(index);
  }
  updateTextureTypeVisibility(type);
}

void TextureView::setTextureFormat(const QString& format) {
  const QSignalBlocker blocker(m_format_combo);
  const int            index = m_format_combo->findText(format);
  if(index != -1) {
    m_format_combo->setCurrentIndex(index);
  }
}

void TextureView::setTextureColorButton(const QColor& color) {
  const QSignalBlocker blocker(m_texture_color_button);
  QPalette             palette = m_texture_color_button->palette();
  palette.setColor(QPalette::Button, color);
  m_texture_color_button->setPalette(palette);
}

void TextureView::setTexturePath(const QString& path) {
  const QSignalBlocker blocker(m_path_edit);
  m_path_edit->setText(path);
}

void TextureView::setTextureFiltering(const QString& filtering) {
  const QSignalBlocker blocker(m_filtering_combo);
  const int            index = m_filtering_combo->findText(filtering);
  if(index != -1) {
    m_filtering_combo->setCurrentIndex(index);
  }
}

void TextureView::setTextureWrappingMode(const QString& mode) {
  const QSignalBlocker blocker(m_wrapping_combo);
  const int            index = m_wrapping_combo->findText(mode);
  if(index != -1) {
    m_wrapping_combo->setCurrentIndex(index);
  }
  updateBorderColorVisibility(mode);
}

void TextureView::setTextureBorderColor(const QColor& color) {
  const QSignalBlocker blocker(m_border_color_button);
  QPalette             palette = m_border_color_button->palette();
  palette.setColor(QPalette::Button, color);
  m_border_color_button->setPalette(palette);
}

void TextureView::setFlipVertically(bool flip) {
  const QSignalBlocker blocker(m_flip_vertically_checkbox);
  m_flip_vertically_checkbox->setChecked(flip);
}
