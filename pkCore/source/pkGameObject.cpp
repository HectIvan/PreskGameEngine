#include "pkGameObject.h"

namespace pkEngineSDK
{

void
GameObject::insertModel(SPtr<Model> _pModel)
{
  models.push_back(_pModel);
}

void
GameObject::clear()
{
  // clean the game object
  models.clear();
  m_components.clear();
  m_children.clear();
  m_parent = nullptr;
  m_transform = Matrix4::IDENTITY;
}
}