/**
 * @file LightWidget.hpp
 * @brief Header file for the LightWidget class, which provides a user interface for light settings.
 */
#ifndef GUI_WIDGETS_LIGHTWIDGET_HPP
#define GUI_WIDGETS_LIGHTWIDGET_HPP

#include <QGroupBox>
#include <linalg/Vec3.hpp>

#include "Core/CommonTypes.hpp"

class Light;

namespace Ui {
  class LightWidget;
}

/**
 * @class LightWidget
 * @brief A widget for displaying and editing light settings.
 *
 * This class provides a user interface for adjusting light properties such as color, intensity,
 * direction, and angles for spot lights. It emits signals when the light settings are updated.
 */
class LightWidget : public QGroupBox {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the LightWidget class.
   * @param parent The parent widget for this light widget.
   */
  explicit LightWidget(QWidget* parent = nullptr);
  
  /**
   * @brief Sets the light for this widget.
   * @param light The Light object to set.
   */
  void setLight(Light* light);

  /**
   * @brief Updates the widget to reflect the current light settings.
   */
  void updateWidget();

  ~LightWidget() override; ///< Default destructor for the LightWidget class.

private slots:
  void onColorChanged(const ColorRGB& new_color);
  void onIntensityChanged(double value);

  void onDirectionChanged(const linalg::Vec3d& new_direction);

  void onSpotDirectionChanged(const linalg::Vec3d& new_direction);
  void onInnerAngleChanged(double value);
  void onOuterAngleChanged(double value);

private:
  Ui::LightWidget* ui;
  Light*           m_light = nullptr;
};

#endif // GUI_WIDGETS_LIGHTWIDGET_HPP
