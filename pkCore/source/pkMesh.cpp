#include "pkMesh.h"
#include "pkDevice.h"

namespace pkEngineSDK
{
  PkMesh::PkMesh(Vector<SimpleVertex> _vertex,
                 Vector<uint32> _index,
                 uint32 _vertexCount,
                 uint32 _numIndex,
                 Vector<Texture*> _textures)
  {
    vertexVector = _vertex;
    indexVector = _index;
    vertexCount = _vertexCount;
    numIndex = _numIndex;
    textures = _textures;
  }

  void
  PkMesh::clean()
  {
    // Empty the vertex vector
    vertexVector.clear();
    // Empty the index vector
    indexVector.clear();
    // Empty the textures vector
    textures.clear();
    // m_material = nullptr;
    vertexCount = 0;
    numIndex = 0;
  }
}