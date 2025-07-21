#ifndef GUI_WIDGETS_SCENETREEVIEW_HPP
#define GUI_WIDGETS_SCENETREEVIEW_HPP

#include <QStandardItemModel>
#include <QTreeView>

class Scene;

/**
 * @class SceneTreeView
 * @brief A tree view widget for displaying and managing the scene hierarchy, including objects and lights.
 * 
 * This class provides functionality to initialize the view from a Scene object, add and remove objects and lights,
 * and handle selection changes. It emits signals when a camera, object, or light is selected, and allows for
 * renaming and deleting items in the scene.
 */
class SceneTreeView : public QTreeView {
  Q_OBJECT
public:
  /**
   * @brief Constructor for the SceneTreeView class.
   * @param parent The parent widget for this tree view.
   */
  explicit SceneTreeView(QWidget* parent = nullptr);

  /**
   * @brief Initializes the tree view from a Scene object.
   * @param scene The Scene object to initialize the view with.
   *
   * This method clears the current model and populates it with the objects and lights from the provided Scene.
   */
  void initializeFromScene(Scene* scene);
  
  /**
   * @brief Adds a 3D object to the scene tree.
   * @param name The name of the object to be added.
   */
  void addObject(const QString& name);

  /**
   * @brief Removes a 3D object from the scene tree.
   * @param name The name of the object to be removed.
   */
  void removeObject(const QString& name);

  /**
   * @brief Adds a light to the scene tree.
   * @param name The name of the light to be added.
   */
  void addLight(const QString& name);

  /**
   * @brief Removes a light from the scene tree.
   * @param name The name of the light to be removed.
   */
  void removeLight(const QString& name);

public slots:
  void selectObject(const QString& name);
  void clearSelection();
  void deleteSelectedItem();

signals:
  void cameraSelected();
  void objectSelected(const QString& name);
  void lightSelected(const QString& name);
  void selectionCleared();

protected:
  void keyPressEvent(QKeyEvent* event) override;

private slots:
  void onSelectionChanged(const QItemSelection& selected, const QItemSelection&);
  void onItemRenamed(QStandardItem* item);

private:
  enum class ItemType { Root, Camera, Object, Light };
  struct ItemRole {
    static constexpr int TypeRole = Qt::UserRole + 1;
    static constexpr int NameRole = Qt::UserRole + 2;
  };

  QStandardItemModel* m_model       = nullptr;
  QStandardItem*      m_objectsRoot = nullptr;
  QStandardItem*      m_lightsRoot  = nullptr;
  QStandardItem*      m_cameraItem  = nullptr;
  Scene*              m_scene       = nullptr;

  static QStandardItem* findItemByName(QStandardItem* parent, const QString& name);
};

#endif // GUI_WIDGETS_SCENETREEVIEW_HPP
