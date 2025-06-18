#ifndef QT_VIEW_MATERIALVIEW_HPP
#define QT_VIEW_MATERIALVIEW_HPP

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qtreewidget.h>
#include <qwidget.h>

#include "Qt/TexturePreviewWidget.hpp"
#include "Qt/TextureSelectorWidget.hpp"

class MaterialView : public QWidget {
  Q_OBJECT
public:
  explicit MaterialView(QWidget* parent);

  TextureSelectorWidget* getDiffuseTextureSelector() const { return m_diffuse_texture_selector; }
  TextureSelectorWidget* getNormalTextureSelector() const { return m_normal_texture_selector; }

  void setDiffuseTexture(const QString& texture_name);
  void setNormalTexture(const QString& texture_name);

  // Slots
  void onMaterialAdded(const QString& material_name);
  void onMaterialRenamed(const QString& old_name, const QString& new_name);
  void onMaterialRemoved(const QString& material_name);
  void onDiffuseTextureImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);
  void onNormalTextureImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);

signals:
  void materialSelected(const QString& material_name);
  void materialUnselected();
  void materialNameEdited(const QString& new_name);
  void materialAdded(const QString& new_name);
  void materialRemoved();
  void diffuseTextureSelected(const QString& texture_name);
  void normalTextureSelected(const QString& texture_name);
  void diffuseTextureImageCreated(const QString& file_path);
  void normalTextureImageCreated(const QString& file_path);

private:
  // Slots
  void onAddButtonClicked();
  void onMaterialSelected();
  void onMaterialNameEdited();
  void onMaterialItemChanged(QTreeWidgetItem* item, int column);
  void onDiffuseTextureSelected(const QString& texture_name);
  void onNormalTextureSelected(const QString& texture_name);

  // Members
  QTreeWidget* m_material_list = nullptr;
  QPushButton* m_add_button    = nullptr;
  QPushButton* m_remove_button = nullptr;

  QLineEdit* m_name_edit = nullptr;

  TexturePreviewWidget*  m_diffuse_image_texture_widget = nullptr;
  TextureSelectorWidget* m_diffuse_texture_selector     = nullptr;

  TexturePreviewWidget*  m_normal_image_texture_widget = nullptr;
  TextureSelectorWidget* m_normal_texture_selector     = nullptr;

  QWidget* m_material_details_widget = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_MATERIALVIEW_HPP