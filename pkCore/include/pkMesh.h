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
#include "pkTransform.h"

namespace pkEngineSDK
{

class Device;

class Mesh
{
  Mesh() = default;
  virtual ~Mesh() = default;

  Mesh(Vector<SimpleVertex> vertex,
       Vector<uint32> index,
       uint32 vertexCount,
       uint32 numIndex,
       Vector<Texture> textures);

  void
  draw(Device* _pDevice);

  void
  Clean();

  Transform mTransform = Transform(Matrix4(0),
                                   Matrix4(0),
                                   Matrix4(0));
  // stores the index and vertex info
  Vector<SimpleVertex> m_vertexVector;
  Vector<uint32> m_indexVector;
  Vector<Texture> m_textures;
  aiMaterial* mMaterial;
  uint32 mVertexCount;
  uint32 mNumIndex;
};
}