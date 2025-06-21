#ifndef QT_CONTROLLER_MATERIALCONTROLLER_HPP
#define QT_CONTROLLER_MATERIALCONTROLLER_HPP

#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>

#include "Qt/Controller/TextureController.hpp"
#include "Qt/View/MaterialView.hpp"
#include "Surface/Material.hpp"
#include "Surface/MaterialManager.hpp"

class MaterialController : public QObject {
  Q_OBJECT
public:
  explicit MaterialController(MaterialView* view, MaterialManager* material_manager, TextureManager* texture_manager,
                              TextureController* texture_controller);

  void updateViewFromModel();

signals:
  void materialAdded(const QString& name);
  void materialRenamed(const QString& new_name, const QString& old_name);
  void materialRemoved(const QString& name);
  void diffuseTextureImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);
  void normalTextureImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);

private:
  // Slots
  void onMaterialSelected(const QString& name);
  void onMaterialUnselected();
  void onMaterialNameEdited(const QString& new_name);
  void onMaterialAdded(const QString& new_name);
  void onMaterialRemoved();
  void onDiffuseTextureSelected(const QString& texture_name);
  void onNormalTextureSelected(const QString& texture_name);

  void createDiffuseTextureFromFile(const QString& file_path);
  void createNormalTextureFromFile(const QString& file_path);

  // Members
  MaterialView*    m_view;
  MaterialManager* m_material_manager;

  const TextureManager* m_texture_manager;
  TextureController*    m_texture_controller;

  Material*   m_current_material = nullptr;
  std::string m_current_material_name;

  void updateDiffuseTexture(const std::string& texture_name);
  void updateNormalTexture(const std::string& texture_name);
  void resetDiffuseTextureToDefault();
  void resetNormalTextureToDefault();

  void connectSignals();
};

#endif // QT_CONTROLLER_MATERIALCONTROLLER_HPP