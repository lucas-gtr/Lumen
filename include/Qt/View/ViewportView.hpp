#ifndef QT_VIEW_VIEWPORTVIEW_HPP
#define QT_VIEW_VIEWPORTVIEW_HPP

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qwidget.h>

class ViewportView : public QWidget {
  Q_OBJECT

public:
  explicit ViewportView(QWidget* parent);

  void setCameraRotationSpeed(double speed);
  void setCameraZoomSpeed(double speed);
  void setCameraMoveSpeed(double speed);
  void setShadowMapSize(int size);
  void setDynamicLighting(bool enabled);
  void setDynamicShadowMap(bool enabled);
  void setToneMapping(const QString& mode);
  void setExposure(double exposure);

signals:
  void cameraRotationSpeedChanged(double speed);
  void cameraZoomSpeedChanged(double speed);
  void cameraMoveSpeedChanged(double speed);
  void shadowMapSizeChanged(int size);
  void dynamicLightChanged(bool enabled);
  void dynamicShadowMapChanged(bool enabled);
  void bakeLights();
  void toneMappingChanged(const QString& mode);
  void exposureChanged(double exposure);

private:
  // Slots
  void onToneMappingChanged(const QString& mode);
  void onDynamicLightToggled(bool enabled);

  // Members
  QDoubleSpinBox* m_camera_rotation_speed_spin = nullptr;
  QDoubleSpinBox* m_camera_zoom_speed_spin     = nullptr;
  QDoubleSpinBox* m_camera_move_speed_spin     = nullptr;
  QSpinBox*       m_shadow_map_size_spin       = nullptr;
  QCheckBox*      m_dynamic_light_check        = nullptr;
  QCheckBox*      m_dynamic_shadow_map_check   = nullptr;
  QPushButton*    m_bake_lights_button         = nullptr;
  QComboBox*      m_tone_mapping_combo         = nullptr;
  QLabel*         m_exposure_label             = nullptr;
  QDoubleSpinBox* m_exposure_spin              = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_VIEWPORTVIEW_HPP
