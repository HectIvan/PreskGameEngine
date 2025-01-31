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

class Device;
class Texture;

class Mesh
{
 public:
  Mesh() = default;
  virtual ~Mesh()
  {
    clean();
  }

  /**
  * Create the mesh.
  * 
  * @param _vertex
  * Vertex data.
  * 
  * @param _index
  * Index data.
  * 
  * @param _vertexcount
  * How many vertex make up the mesh.
  * 
  * @param _numIndex
  * How many index make up the mesh.
  * 
  * @param _textures
  * Textures data.
  **/
  Mesh(Vector<SimpleVertex> _vertex,
         Vector<uint32> _index,
         uint32 _vertexCount,
         uint32 _numIndex,
         Vector<Texture*> _textures);

  /**
  * Clean the mesh of any data
  **/
  void
  clean();

  Transform transform = Transform(Matrix4(0),
                                   Matrix4(0),
                                   Matrix4(0));

 public:
  // stores the index and vertex info
  Vector<SimpleVertex> vertexVector;
  Vector<uint32> indexVector;
  // Vector<Texture*> textures;
  SPtr<Material> material;
  uint32 vertexCount;
  uint32 numIndex;

  String materialPath;
};
}