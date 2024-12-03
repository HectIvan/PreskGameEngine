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
    m_vertexVector = _vertex;
    m_indexVector = _index;
    m_vertexCount = _vertexCount;
    m_numIndex = _numIndex;
    m_textures = _textures;
  }

  void
  PkMesh::clean()
  {
    // Empty the vertex vector
    while (!m_vertexVector.empty())
    {
      m_vertexVector.pop_back();
    }
    // Empty the index vector
    while (!m_indexVector.empty())
    {
      m_indexVector.pop_back();
    }
    // Empty the textures vector
    while (!m_textures.empty())
    {
      m_textures.pop_back();
    }
    m_material = nullptr;
    m_vertexCount = 0;
    m_numIndex = 0;
  }
}