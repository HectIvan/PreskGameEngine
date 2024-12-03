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
#include <assimp/texture.h>
#include<assimp/material.h>

#include "pkPrerequisitesCore.h"
#include "pkSimpleVertex.h"
#include "pkTexture.h"
#include "pkTransform.h"

namespace pkEngineSDK
{

class Device;

class PkMesh
{
 public:
  PkMesh() = default;
  virtual ~PkMesh() = default;

  PkMesh(Vector<SimpleVertex> _vertex,
         Vector<uint32> _index,
         uint32 _vertexCount,
         uint32 _numIndex,
         Vector<Texture*> _textures);

  /**
  * Render the mesh.
  *
  * @param _pDevice
  * Device that will be used to render.
  **/
  virtual void
  render(Device* _pDevice) = 0;

  /**
  * Clean the mesh of any data
  **/
  void
  clean();

  Transform mTransform = Transform(Matrix4(0),
                                   Matrix4(0),
                                   Matrix4(0));

 public:
  // stores the index and vertex info
  Vector<SimpleVertex> m_vertexVector;
  Vector<uint32> m_indexVector;
  Vector<Texture*> m_textures;
  aiMaterial* m_material;
  uint32 m_vertexCount;
  uint32 m_numIndex;
};
}