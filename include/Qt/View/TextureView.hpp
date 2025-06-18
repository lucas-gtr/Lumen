#ifndef QT_VIEW_TEXTUREVIEW_HPP
#define QT_VIEW_TEXTUREVIEW_HPP

#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtreewidget.h>
#include <qwidget.h>

#include "Core/CommonTypes.hpp"
#include "Qt/TexturePreviewWidget.hpp"

class TextureView : public QWidget {
  Q_OBJECT

public:
  explicit TextureView(QWidget* parent);

  void setColorSpace(const QString& color_space);
  void setTextureType(const QString& type);
  void setTextureFormat(const QString& format);
  void setTextureColorButton(const QColor& color);
  void setTexturePath(const QString& path);
  void setTextureFiltering(const QString& filtering);
  void setTextureWrappingMode(const QString& mode);
  void setTextureBorderColor(const QColor& color);
  void setFlipVertically(bool flip);

  // Slots
  void onTextureAdded(const QString& name);
  void onTextureRenamed(const QString& new_name, const QString& old_name);
  void onTextureRemoved(const QString& name);
  void onTextureImageChanged(const unsigned char* preview_data, const ImageProperties& preview_properties);

signals:
  void textureSelected(const QString& name);
  void textureUnselected();
  void textureNameEdited(const QString& new_name);
  void textureAdded(const QString& new_name);
  void textureRemoved();
  void textureTypeChanged(const QString& type);
  void textureFormatChanged(const QString& format);
  void textureColorChanged(const QColor& color);
  void texturePathChanged(const QString& path);
  void colorSpaceChanged(const QString& color_space);
  void filteringChanged(const QString& filtering);
  void wrappingModeChanged(const QString& mode);
  void borderColorChanged(const QColor& color);
  void flipVerticallyChanged(bool flip);

private:
  // Slots
  void onAddButtonClicked();
  void onTextureTypeChanged(const QString& type);
  void onWrappingModeChanged(const QString& mode);
  void onTextureSelected();
  void onTextureNameEdited();
  void onTextureItemChanged(QTreeWidgetItem* item, int column);
  void onBrowseButtonClicked();
  void onTextureColorButtonClicked();
  void onBorderColorButtonClicked();

  // Members
  QTreeWidget* m_texture_list  = nullptr;
  QPushButton* m_add_button    = nullptr;
  QPushButton* m_remove_button = nullptr;

  QLineEdit*            m_name_edit            = nullptr;
  TexturePreviewWidget* m_preview_image_widget = nullptr;

  QComboBox* m_texture_type_combo = nullptr;

  QLabel*      m_texture_color_label  = nullptr;
  QPushButton* m_texture_color_button = nullptr;
  QLabel*      m_format_label         = nullptr;
  QComboBox*   m_format_combo         = nullptr;

  QLabel*      m_path_label    = nullptr;
  QLineEdit*   m_path_edit     = nullptr;
  QPushButton* m_browse_button = nullptr;

  QComboBox* m_color_space_combo = nullptr;

  QComboBox* m_filtering_combo = nullptr;

  QComboBox* m_wrapping_combo = nullptr;

  QLabel*      m_border_color_label  = nullptr;
  QPushButton* m_border_color_button = nullptr;

  QLabel*    m_flip_vertically_label    = nullptr;
  QCheckBox* m_flip_vertically_checkbox = nullptr;

  QWidget* m_texture_details_widget = nullptr;

  void updateTextureTypeVisibility(const QString& type);
  void updateBorderColorVisibility(const QString& mode);

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_TEXTUREVIEW_HPP
