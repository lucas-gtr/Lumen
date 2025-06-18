/**
 * @file TextureSelectorWidget.hpp
 * @brief Header file for the TextureSelectorWidget class, which provides a UI for selecting textures.
 */
#ifndef QT_TEXTURESELECTORWIDGET_HPP
#define QT_TEXTURESELECTORWIDGET_HPP

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>

#include "Surface/TextureManager.hpp"

/**
 * @class TextureSelectorWidget
 * @brief A widget for selecting textures from a TextureManager.
 *
 * This widget provides a dropdown to select textures and a button to browse for new textures.
 * It connects to a TextureManager to receive updates about available textures.
 * It emits signals when a texture is selected or when a new texture creation is requested.
 */
class TextureSelectorWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the TextureSelectorWidget.
   * @param default_texture_name The name of the default texture to select initially.
   * @param parent The parent widget.
   */
  explicit TextureSelectorWidget(const std::string_view& default_texture_name, QWidget* parent);

  /**
   * @brief Connects the widget to the TextureManager.
   * @param texture_manager The TextureManager to connect to.
   */
  void connectToTextureManager(TextureManager* texture_manager);

  /**
   * @brief Sets the current texture in the selector.
   * @param texture_name The name of the texture to set as current.
   */
  void setCurrentTexture(const QString& texture_name);

signals:
  void textureSelected(const QString& texture_name);
  void textureCreationRequested(const QString& file_path);

private:
  // Slots
  void onTextureAdded(const std::string& texture_name);
  void onTextureRemoved(const std::string& texture_name);
  void onTextureRenamed(const std::string& old_name, const std::string& new_name);
  void onBrowseTextureButtonClicked();

  // Members
  QComboBox*   m_texture_selector      = nullptr;
  QPushButton* m_browse_texture_button = nullptr;

  std::string m_default_texture_name;

  void setupUI();
  void connectSignals();
};

#endif // QT_TEXTURESELECTORWIDGET_HPP