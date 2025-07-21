/**
 * @file ViewportSettingsWidget.hpp
 * @brief Header file for the ViewportSettingsWidget class, which provides a user interface for
 *        configuring viewport settings in the OpenGL engine.
 */
#ifndef GUI_WIDGETS_VIEWPORTSETTINGSWIDGET_HPP
#define GUI_WIDGETS_VIEWPORTSETTINGSWIDGET_HPP

#include <QWidget>

#include "GPU/OpenGL/EngineGL.hpp"

namespace Ui {
class ViewportSettingsWidget;
}

/**
 * @class ViewportSettingsWidget
 * @brief A widget for configuring viewport settings in the OpenGL engine.
 *
 * This class provides a user interface for adjusting camera rotation speed, movement speed,
 * zoom speed, shadow map size, dynamic shadows, tone mapping, and exposure settings.
 * It allows users to modify these settings interactively and applies them to the OpenGL engine.
 */
class ViewportSettingsWidget : public QWidget {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the ViewportSettingsWidget class.
   * @param parent The parent widget for this viewport settings widget.
   */
  explicit ViewportSettingsWidget(QWidget* parent = nullptr);

  /**
   * @brief Sets the OpenGL engine for this widget.
   * @param engine The EngineGL instance to set.
   */
  void updateWidget();

  ~ViewportSettingsWidget() override; ///< Default destructor for the ViewportSettingsWidget class.

public slots:
  void setEngine(EngineGL* engine);

signals:
  void lightBakeRequested();

private slots:
  void onCameraRotationSpeedChanged(double value);
  void onCameraMovementSpeedChanged(double value);
  void onCameraZoomSpeedChanged(double value);
  void onShadowMapSizeChanged(int value);
  void onDynamicShadowsChanged(bool checked);
  void onToneMappingChanged(const QString& tone_mapping);
  void onExposureChanged(double value);

private:
  Ui::ViewportSettingsWidget* ui;

  EngineGL* m_engine = nullptr;
};

#endif // GUI_WIDGETS_VIEWPORTSETTINGSWIDGET_HPP
