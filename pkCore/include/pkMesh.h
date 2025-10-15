/************************************************************************/
/**
* @pkMesh pkMesh.h
* @Hector Ivan Muñoz Ceballos
* @date 20/11/2024
* @Mesh file for the Presk Game Engine.
*
* This file will contain the Mesh class used for the engine
*
* @bug No bug known.
*
* @HectIvan 20/11/2024
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkPrerequisitesCore.h"
#include "pkSimpleVertex.h"
#include "pkMaterial.h"
#include "pkTransform.h"

namespace pkEngineSDK
{

class Mesh
{
 public:
  Mesh() = default;
  virtual ~Mesh() {
    clean();
  }

  /**
   * @brief Create a mesh.
   * @param _vertex Vertex data.
   * @param _index Index data.
   * @param _vertexcount How many vertex make up the mesh.
   * @param _numIndex How many index make up the mesh.
   * @param _textures Textures data.
   */
  Mesh(Vector<SimpleVertex> _vertex,
         Vector<uint32> _index,
         uint32 _vertexCount,
         uint32 _numIndex);

  /**
   * @brief Clean the mesh of any data
   */
  void
  clean();

  /**
   * @brief Set mesh name.
   * @param _name Name of the mesh.
   */
  void
  setName(String _name) { m_name = _name; }

  /**
   * @brief Get the name of the mesh.
   * @return Reference to the name of the mesh.
   */
  String&
  getName() { return m_name; }

  /**
   * @brief Set if the mesh is active or not.
   * @param _active Mesh active.
   */
  void
  setActive(bool _active) { m_active = _active; }

  /**
   * @brief Get if the mesh is active.
   * @return Mesh active.
   */
  bool&
  getActive() { return m_active; }

 public:
  // stores the index and vertex info
  Vector<SimpleVertex> vertexVector;
  Vector<uint32> indexVector;

  Matrix4 m_transform = Matrix4::IDENTITY;
  uint32 vertexCount;
  uint32 numIndex;

  SPtr<Material> material;

  String materialPath;

 private:
  String m_name;
  bool m_active = true;
};
}