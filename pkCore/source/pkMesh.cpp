#include "pkMesh.h"
#include "pkDevice.h" // removing this breaks with a linker error, i don't know why

namespace pkEngineSDK
{

Mesh::Mesh(Vector<SimpleVertex> _vertex,
           Vector<uint32> _index,
           uint32 _vertexCount,
           uint32 _numIndex)
{
  vertexVector = _vertex;
  indexVector = _index;
  vertexCount = _vertexCount;
  numIndex = _numIndex;
  materialPath = "";
}

void
Mesh::clean()
{
  vertexVector.clear();
  indexVector.clear();

  m_transform = Matrix4::IDENTITY;
  vertexCount = 0;
  numIndex = 0;

  material = nullptr;
  materialPath = "";

  m_name = "";
  m_active = true;
}
}