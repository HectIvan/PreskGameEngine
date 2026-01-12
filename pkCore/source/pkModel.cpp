/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkModel.h"

namespace pkEngineSDK
{
Model::Model()
{
  boneCounter = 0;
  m_vertexB = make_shared<VertexBuffer>();
  m_indexB = make_shared<IndexBuffer>();
}

void
Model::update(Actor& _owner)
{
}

void
Model::clean()
{
  vertex.clear();
  index.clear();

  boneMap.clear();
  meshes.clear();
  boneCounter = 0;

  m_vertexB = make_shared<VertexBuffer>();
  m_indexB = make_shared<IndexBuffer>();
}
void
Model::setVerticesIndices()
{
  for (uint32 i = 0; i < meshes.size(); ++i) {
    vertex.insert(vertex.end(),
                  meshes[i]->vertexVector.begin(),
                  meshes[i]->vertexVector.end());

    index.insert(index.end(),
                 meshes[i]->indexVector.begin(),
                 meshes[i]->indexVector.end());
  }
}
}