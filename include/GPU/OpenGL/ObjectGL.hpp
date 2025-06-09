
/**
 * @file ObjectGL.hpp
 * @brief Header file for the ObjectGL class, which represents a 3D object in OpenGL.
 */
#ifndef GPU_OPENGL_OBJECTGL_HPP
#define GPU_OPENGL_OBJECTGL_HPP

// GCOVR_EXCL_START

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
class ObjectGL : public IObjectGPU {
private:
  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;
  unsigned int m_EBO = 0;

  const MaterialGL* m_material;

  void        uploadVertices() const;
  void        uploadIndices() const;
  static void setupVertexAttributes();

public:
  /**
   * @brief Constructor for ObjectGL.
   * @param object The Object3D to be represented in OpenGL.
   * @param material Pointer to the MaterialGL associated with this object.
   */
  ObjectGL(const Object3D& object, const MaterialGL* material);

  ObjectGL(const ObjectGL&)            = delete;
  ObjectGL& operator=(const ObjectGL&) = delete;
  ObjectGL(ObjectGL&&)                 = delete;
  ObjectGL& operator=(ObjectGL&&)      = delete;

  /**
   * @brief Uploads the data to the GPU using OpenGL.
   */
  void uploadToGPU() const override;

  /**
   * @brief Gets the Vertex Array Object ID.
   * @return The Vertex Array Object ID.
   */
  unsigned int getVAO() const { return m_VAO; }

  /**
   * @brief Binds the Vertex Array Object for rendering.
   */
  void bindVAO() const;

  /**
   * @brief Binds the textures of the material to the appropriate texture units.
   */
  void bindMaterial() const;

  /**
   * @brief Unbinds the Vertex Array Object.
   */
  static void unbind();

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