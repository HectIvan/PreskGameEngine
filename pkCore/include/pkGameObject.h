/************************************************************************/
/**
* @pkGameObject pkGameObject.h
* @Hector Ivan Muñoz Ceballos
* @date 13/11/2024
* @Game object file for the Presk Game Engine.
*
* This file will contain the Game object used by the engine
*
* @bug No known bugs.
*/
/************************************************************************/
#pragma once

/*********************************************/
/**
* Includes
**/
/*********************************************/
#include "pkActor.h"
#include "pkModel.h"

namespace pkEngineSDK
{

class PK_CORE_EXPORT GameObject : public Actor
{
 public:
  GameObject() = default;
  virtual ~GameObject()
  {
    clear();
  }

  /**
   * @brief Inserts a new model into the model vector.
   * @param _model Insert the new model.
   */
  void
  insertModel(SPtr<Model> _pModel);

  /**
   * @brief Cleans the GameObject.
   */
  void
  clear();

 public:
  Vector<SPtr<Model>> models;
};
}