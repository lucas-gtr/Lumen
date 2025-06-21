#include <iostream>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>

#include "Core/Config.hpp"
#include "Qt/Controller/MaterialController.hpp"
#include "Qt/Controller/TextureController.hpp"
#include "Qt/View/MaterialView.hpp"
#include "Surface/Material.hpp"
#include "Surface/MaterialManager.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

MaterialController::MaterialController(MaterialView* view, MaterialManager* material_manager,
                                       TextureManager* texture_manager, TextureController* texture_controller)
    : QObject(view), m_view(view), m_material_manager(material_manager), m_texture_manager(texture_manager),
      m_texture_controller(texture_controller) {
  m_view->getDiffuseTextureSelector()->connectToTextureManager(texture_manager);
  m_view->getNormalTextureSelector()->connectToTextureManager(texture_manager);
  connectSignals();

  emit diffuseTextureImageChanged(TextureManager::defaultDiffuseTexture()->getPreviewData(),
                                  TextureManager::defaultDiffuseTexture()->getPreviewProperties());
  emit normalTextureImageChanged(TextureManager::defaultNormalTexture()->getPreviewData(),
                                 TextureManager::defaultNormalTexture()->getPreviewProperties());
}

void MaterialController::connectSignals() {
  connect(m_view, &MaterialView::materialSelected, this, &MaterialController::onMaterialSelected);
  connect(m_view, &MaterialView::materialUnselected, this, &MaterialController::onMaterialUnselected);
  connect(m_view, &MaterialView::materialNameEdited, this, &MaterialController::onMaterialNameEdited);
  connect(m_view, &MaterialView::materialAdded, this, &MaterialController::onMaterialAdded);
  connect(m_view, &MaterialView::materialRemoved, this, &MaterialController::onMaterialRemoved);
  connect(m_view, &MaterialView::diffuseTextureSelected, this, &MaterialController::onDiffuseTextureSelected);
  connect(m_view, &MaterialView::normalTextureSelected, this, &MaterialController::onNormalTextureSelected);

  connect(this, &MaterialController::materialAdded, m_view, &MaterialView::onMaterialAdded);
  connect(this, &MaterialController::materialRenamed, m_view, &MaterialView::onMaterialRenamed);
  connect(this, &MaterialController::materialRemoved, m_view, &MaterialView::onMaterialRemoved);
  connect(this, &MaterialController::diffuseTextureImageChanged, m_view, &MaterialView::onDiffuseTextureImageChanged);
  connect(this, &MaterialController::normalTextureImageChanged, m_view, &MaterialView::onNormalTextureImageChanged);

  connect(m_view, &MaterialView::diffuseTextureImageCreated, this, &MaterialController::createDiffuseTextureFromFile);
  connect(m_view, &MaterialView::normalTextureImageCreated, this, &MaterialController::createNormalTextureFromFile);
}

void MaterialController::onMaterialSelected(const QString& name) {
  m_current_material_name = name.toStdString();
  m_current_material      = m_material_manager->getMaterial(m_current_material_name);
  updateViewFromModel();
}

void MaterialController::onMaterialUnselected() {
  m_current_material = nullptr;
  m_current_material_name.clear();
}

void MaterialController::onMaterialNameEdited(const QString& new_name) {
  if(m_material_manager->renameMaterial(m_current_material, new_name.toStdString())) {
    emit materialRenamed(QString::fromStdString(m_current_material_name), new_name);
    m_current_material_name = new_name.toStdString();
  } else {
    emit materialRenamed(QString::fromStdString(m_current_material_name),
                         QString::fromStdString(m_current_material_name));
  }
}

void MaterialController::onMaterialAdded(const QString& new_name) {
  const std::string material_name = m_material_manager->getAvailableMaterialName(new_name.toStdString());
  m_material_manager->addMaterial(material_name);
  m_current_material_name = material_name;
  m_current_material      = m_material_manager->getMaterial(material_name);
  emit materialAdded(QString::fromStdString(material_name));
}

void MaterialController::onMaterialRemoved() {
  if(m_current_material == nullptr) {
    return;
  }

  m_material_manager->removeMaterial(m_current_material_name);
  m_current_material = nullptr;
  emit materialRemoved(QString::fromStdString(m_current_material_name));
}

void MaterialController::onDiffuseTextureSelected(const QString& texture_name) {
  updateDiffuseTexture(texture_name.toStdString());
}

void MaterialController::onNormalTextureSelected(const QString& texture_name) {
  updateNormalTexture(texture_name.toStdString());
}

void MaterialController::createDiffuseTextureFromFile(const QString& file_path) {
  const std::string new_texture_name =
      m_texture_controller->createTextureFromFile(file_path.toStdString(), ColorSpace::Linear, true);
  m_view->setDiffuseTexture(QString::fromStdString(new_texture_name));
  updateDiffuseTexture(new_texture_name);
}

void MaterialController::createNormalTextureFromFile(const QString& file_path) {
  const std::string new_texture_name =
      m_texture_controller->createTextureFromFile(file_path.toStdString(), ColorSpace::sRGB, true);
  m_view->setNormalTexture(QString::fromStdString(new_texture_name));
  updateNormalTexture(new_texture_name);
}

void MaterialController::updateDiffuseTexture(const std::string& texture_name) {
  if(texture_name == DEFAULT_DIFFUSE_TEXTURE_NAME) {
    resetDiffuseTextureToDefault();
    return;
  }
  Texture* texture = m_texture_manager->getTexture(texture_name);
  m_current_material->setDiffuseTexture(texture);

  emit diffuseTextureImageChanged(texture->getPreviewData(), texture->getPreviewProperties());
}

void MaterialController::updateNormalTexture(const std::string& texture_name) {
  if(texture_name == DEFAULT_NORMAL_TEXTURE_NAME) {
    resetNormalTextureToDefault();
    return;
  }
  Texture* texture = m_texture_manager->getTexture(texture_name);
  m_current_material->setNormalTexture(texture);

  emit normalTextureImageChanged(texture->getPreviewData(), texture->getPreviewProperties());
}

void MaterialController::resetDiffuseTextureToDefault() {
  m_current_material->setDiffuseTexture(TextureManager::defaultDiffuseTexture());
  emit diffuseTextureImageChanged(TextureManager::defaultDiffuseTexture()->getPreviewData(),
                                  TextureManager::defaultDiffuseTexture()->getPreviewProperties());
}

void MaterialController::resetNormalTextureToDefault() {
  m_current_material->setNormalTexture(TextureManager::defaultNormalTexture());
  emit normalTextureImageChanged(TextureManager::defaultNormalTexture()->getPreviewData(),
                                 TextureManager::defaultNormalTexture()->getPreviewProperties());
}

void MaterialController::updateViewFromModel() {
  if(m_current_material == nullptr) {
    return;
  }

  Texture* diffuse_texture = m_current_material->getDiffuseTexture();
  if(diffuse_texture != nullptr) {
    std::string diffuse_texture_name = m_texture_manager->getTextureName(diffuse_texture);
    if(diffuse_texture_name.empty()) {
      diffuse_texture_name = DEFAULT_DIFFUSE_TEXTURE_NAME;
    }
    m_view->setDiffuseTexture(QString::fromStdString(diffuse_texture_name));

    emit diffuseTextureImageChanged(diffuse_texture->getPreviewData(), diffuse_texture->getPreviewProperties());
  } else {
    std::cout << "Diffuse texture is null, setting to default." << '\n';
  }

  Texture* normal_texture = m_current_material->getNormalTexture();
  if(normal_texture != nullptr) {
    std::string normal_texture_name = m_texture_manager->getTextureName(normal_texture);
    if(normal_texture_name.empty()) {
      normal_texture_name = DEFAULT_NORMAL_TEXTURE_NAME;
    }
    m_view->setNormalTexture(QString::fromStdString(normal_texture_name));

    emit normalTextureImageChanged(normal_texture->getPreviewData(), normal_texture->getPreviewProperties());
  }
}