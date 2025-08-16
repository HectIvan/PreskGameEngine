#include "pkSprite.h"

namespace pkEngineSDK
{
Sprite::Sprite()
{
  /*********************************************/
  /**
  * Manual creation of a sprite mesh
  **/
  /*********************************************/
  // bsae vertex
  SimpleVertex vertex;
  // first vertex
  vertex.pos = Vector3(-1.0f, -1.0f, 0.0f);
  vertex.Tex = Vector2(1.0f, 1.0f);
  vertex.normal = Vector3(1.0f, 0.0f, 0.0f);
  vertexVector.push_back(vertex);
  // second vertex
  vertex.pos = Vector3(-1.0f, 1.0f, 0.0f);
  vertex.Tex = Vector2(0.0f, 1.0f);
  vertex.normal = Vector3(1.0f, 0.0f, 0.0f);
  vertexVector.push_back(vertex);
  // third vertex
  vertex.pos = Vector3(1.0f, 1.0f, 0.0f);
  vertex.Tex = Vector2(0.0f, 0.0f);
  vertex.normal = Vector3(1.0f, 0.0f, 0.0f);
  vertexVector.push_back(vertex);
  // fourth vertex
  vertex.pos = Vector3(1.0f, -1.0f, 0.0f);
  vertex.Tex = Vector2(1.0f, 0.0f);
  vertex.normal = Vector3(1.0f, 0.0f, 0.0f);
  vertexVector.push_back(vertex);

  // face 1
  indexVector.push_back(0);
  indexVector.push_back(2);
  indexVector.push_back(4);
  // face 2
  indexVector.push_back(4);
  indexVector.push_back(3);
  indexVector.push_back(1);
}
}