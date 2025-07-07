
/**
 * @file ObjectGL.hpp
 * @brief Header file for the ObjectGL class, which represents a 3D object in OpenGL.
 */
#ifndef GPU_OPENGL_OBJECTGL_HPP
#define GPU_OPENGL_OBJECTGL_HPP

// GCOVR_EXCL_START

#include <qopenglfunctions_3_3_core.h>

#include "GPU/IObjectGPU.hpp"
#include "GPU/OpenGL/MaterialGL.hpp"
#include "SceneObjects/Object3D.hpp"

/**
 * @class ObjectGL
 * @brief Represents a 3D object in OpenGL, encapsulating its vertex data and material.
 *
 * This class manages the OpenGL Vertex Array Object (VAO), Vertex Buffer Object (VBO),
 * and Element Buffer Object (EBO) for rendering a 3D object. It also handles the binding
 * of the material textures to the appropriate texture units.
 */
class ObjectGL : public IObjectGPU, protected QOpenGLFunctions_3_3_Core {
private:
  unsigned int m_vao = 0;
  unsigned int m_vbo = 0;
  unsigned int m_ebo = 0;

  MaterialGL* m_material;

  void uploadVertices();
  void uploadIndices();
  void setupVertexAttributes();

  bool m_is_selected = false;

public:
  /**
   * @brief Constructor for ObjectGL.
   * @param object The Object3D to be represented in OpenGL.
   * @param material Pointer to the MaterialGL associated with this object.
   */
  ObjectGL(Object3D* object, MaterialGL* material);

  ObjectGL(const ObjectGL&)            = delete;
  ObjectGL& operator=(const ObjectGL&) = delete;
  ObjectGL(ObjectGL&&)                 = delete;
  ObjectGL& operator=(ObjectGL&&)      = delete;

  /**
   * @brief Sets the material for this OpenGL object.
   * @param material Pointer to the MaterialGL to set.
   */
  void setMaterial(MaterialGL* material);

  /**
   * @brief Uploads the data to the GPU using OpenGL.
   */
  void uploadToGPU() override;

  /**
   * @brief Gets the Vertex Array Object ID.
   * @return The Vertex Array Object ID.
   */
  unsigned int getVAO() const { return m_vao; }

  /**
   * @brief Sets the selection state of the object.
   * @param selected True if the object is selected, false otherwise.
   */
  void setSelected(bool selected) { m_is_selected = selected; }

  /**
   * @brief Checks if the object is selected.
   * @return True if the object is selected, false otherwise.
   */
  bool isSelected() const { return m_is_selected; }

  /**
   * @brief Binds the Vertex Array Object for rendering.
   */
  void bindVAO();

  /**
   * @brief Binds the textures of the material to the appropriate texture units.
   */
  void bindMaterial();

  /**
   * @brief Unbinds the Vertex Array Object.
   */
  void unbind();

  /**
   * @brief Releases the OpenGL resources used by the buffer.
   */
  void release() override;

  /**
   * @brief Destructor for ObjectGL.
   */
  ~ObjectGL() override;
};

// GCOVR_EXCL_STOP

#endif // GPU_OPENGL_OBJECTGL_HPP