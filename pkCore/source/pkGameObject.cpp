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
  // clean all models
  models.clear();
}
}