#ifndef QT_VIEW_OBJECTVIEW_HPP
#define QT_VIEW_OBJECTVIEW_HPP

#include <qboxlayout.h>
#include <qwidget.h>

#include "Qt/MaterialSelectorWidget.hpp"

class ObjectView : public QWidget {
  Q_OBJECT
public:
  explicit ObjectView(QWidget* parent);

  MaterialSelectorWidget* getMaterialSelectorWidget() const { return m_material_selector; }
  void                    setCurrentMaterial(const QString& material_name);

signals:
  void materialSelected(const QString& material_name);

private:
  MaterialSelectorWidget* m_material_selector = nullptr;

  void setupUI();
  void connectSignals();
};

#endif // QT_VIEW_OBJECTVIEW_HPP