#ifndef QT_CONTROLLER_TEXTURECONTROLLER_HPP
#define QT_CONTROLLER_TEXTURECONTROLLER_HPP

#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>

#include "Qt/View/TextureView.hpp"
#include "Surface/Texture.hpp"
#include "Surface/TextureManager.hpp"

class TextureController : public QObject {
  Q_OBJECT

public:
  explicit TextureController(TextureView* view, TextureManager* texture_manager);

  std::string createTextureFromFile(const std::string& file_name, ColorSpace color_space, bool flip_vertically);
  void        updateViewFromModel();

  // Slots
  void onTextureAdded(const QString& name);

signals:
  void textureAdded(const QString& name);
  void textureRenamed(const QString& new_name, const QString& old_name);
  void textureRemoved(const QString& name);
  void textureImageChanged(const unsigned char* image_data, const ImageProperties& preview_properties);

private:
  // Slots
  void onTextureSelected(const QString& name);
  void onTextureUnselected();
  void onTextureNameEdited(const QString& new_name);
  void onTextureRemoved();
  void onTextureTypeChanged(const QString& type);
  void onTextureFormatChanged(const QString& format);
  void onTextureColorChanged(const QColor& color);
  void onTexturePathChanged(const QString& path);
  void onColorSpaceChanged(const QString& color_space);
  void onFilteringChanged(const QString& filtering);
  void onWrappingModeChanged(const QString& mode);
  void onBorderColorChanged(const QColor& color);
  void onFlipVerticallyChanged(bool flip);

  // Members
  TextureView*    m_view;
  TextureManager* m_texture_manager;

  Texture*    m_current_texture = nullptr;
  std::string m_current_texture_name;

  void connectSignals();
};

#endif // QT_CONTROLLER_TEXTURECONTROLLER_HPP
