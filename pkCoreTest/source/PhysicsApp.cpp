#include <iostream>

#include "PhysicsApp.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkRendererManager.h"
#include "pkTextureManager.h"
#include "pkVector3.h"
#include "pkVector2.h"

using pkEngineSDK::Logger;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_Logger;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_sceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::Light;
using pkEngineSDK::Math;
using pkEngineSDK::Model;
using pkEngineSDK::uint32;
using pkEngineSDK::RendererManager;
using pkEngineSDK::TextureManager;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;

using std::make_shared;

void
PhysicsApp::initSpring(Vector3 _pos, float _length, float _stiffness)
{
  SPtr<Actor> anchor = g_sceneManager().instantiate();
  anchor->addComponent(newModel("sprite.fbx"));
  
  SPtr<Actor> weight = g_sceneManager().instantiate();
  weight->addComponent(newModel("sprite.fbx"));

  m_spring = make_shared<Spring>();
  m_spring->m_anchor = anchor;
  m_spring->m_weight = weight;
  m_spring->m_elasticity = _stiffness;
  m_spring->m_length = _length;
  m_spring->m_mass = 1.0f;
  m_spring->m_gravity = 3.0f;

  anchor->setPosition(_pos);
  weight->setPosition(_pos.x, _pos.y + _length, _pos.y);
}

void
PhysicsApp::onInit()
{
  TextureManager& tm = g_TextureManager().instance();

  m_type = PHYSICS_TYPE::kEuler;

  // create one model instance
  m_projectileCount = 100;
  m_projDuration = 10.0f;
  float projSpeed = 80.0f;

  /**
   * Cannon creation
   */
  m_cannon = std::make_shared<Cannon>();
  m_cannon->m_actor = g_sceneManager().instantiate();
  m_cannon->m_actor->addComponent(newModel("sphere.obj"));
  m_cannon->m_actor->move(Vector3(-0.0f, 0.0f, 0.0f));
  m_cannon->m_actor->setPosition(Vector3(0.0f, 7.0f, 0.0f));
  m_cannon->m_actor->setRotation(0.0f, 0.0f, -1.5708f);

  for (uint32_t i = 0; i < m_projectileCount; ++i) {
    // make new instance of a projectile
    SPtr<Projectile> proj = make_shared<Projectile>();
    // get the last instance
    proj->m_actor = g_sceneManager().instantiate();
    // start the projectile
    proj->start();
    // set projectile speed
    proj->m_speed = projSpeed;
    proj->m_maxSpeed = projSpeed;
    // set the projectile lifetime
    proj->m_lifeTimer = m_projDuration;
    // assign a new model component to the game object.
    proj->m_actor->addComponent(newModel("sphere.obj"));
    // add the game object to the vector of projectiles
    m_projectiles.push_back(proj);
  }

  /**
   * Instantiate obstacle
   */
  // SPtr<Actor> obst = g_sceneManager().instantiate();
  // obst->m_transform = Matrix4::IDENTITY;
  // obstacle.start(Vector3(-5, -3, 0), 1, 0.9f, obst);
  // obstacle.m_actor->addComponent(newModel("sphere.obj"));

  /**
   * Instantiate spring
   */
  // initSpring(Vector3(4.0f, 0.0f, 0.0f), 3, 1);

  /**
   * @brief Create IK
   */
  m_ik = make_shared<InverseKinematics>();
  for (uint32 i = 0; i < 20; ++i) {
    SPtr<Actor> ikRoot = g_sceneManager().instantiate();
    ikRoot->addComponent(newModel("sphere.obj"));
    ikRoot->getComponent<Model>()->getMeshes()[0]->material->setDiffuse(tm.createTexture("blue.png"));

    m_ik->insertNodeLocal(Vector3(i * -0.5, i + g_TimeManager().m_fixedDeltaTime, 0), ikRoot);

    SPtr<Obstacle> obs = make_shared<Obstacle>();
    obs->m_actor = ikRoot;
    obs->m_bounciness = 0.9f;
    obs->m_sphere.m_origin = ikRoot->getPosition3();
    obs->m_sphere.m_radius = 0.5f;
    obstacles.push_back(obs);
  }

  m_fireDirection = Vector2(0.0f);
}


void
PhysicsApp::onUpdate()
{
  float deltaTime = g_TimeManager().m_deltaTime;
  // fire the projectile
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kF) &&
    !m_firing)
  {
    fireProjectile();
    m_firing = true;
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kF))
  {
    m_firing = false;
  }
  // move left or right
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA))
  {
    m_fireDirection.x -= 1.0f * deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD))
  {
    m_fireDirection.x += 1.0f * deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) &&
    !m_changingType) {
    m_changingType = true;
    // change simulation type
    if (m_type == PHYSICS_TYPE::kEuler) {
      m_type = PHYSICS_TYPE::kVerlet;
    }
    else {
      m_type = PHYSICS_TYPE::kEuler;
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kB)) {
    g_RenderManager().compileShaders();
  }
//  Vector3 weightPos = m_spring->m_weight->m_transform.getTranslation3();
  float strength = 5.0f * deltaTime;
  Vector3 posIK = m_ik->getLastBone()->actorIni->getPosition3();
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    //m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::DOWN * deltaTime * strength);
    m_ik->fabrik(posIK +
                 Vector3::DOWN *
                 strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    //m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::UP * deltaTime * strength);
    m_ik->fabrik(posIK + 
                 Vector3::UP *
                 strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    //m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::RIGHT * deltaTime * strength);
    m_ik->fabrik(posIK +
                 Vector3::RIGHT *
                 strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    //m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::LEFT * deltaTime * strength);
    m_ik->fabrik(posIK + 
                 Vector3::LEFT *
                 strength);
  }

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton)) {
    Vector3 mousePos = Vector3(m_eventQueue.mousePosition.x,
                               m_eventQueue.mousePosition.y,
                               0.0f);
    float ndcX = (2.0f * mousePos.x) / m_window.getWidth() - 1.0f;
    float ndcY  = 1.0f - (2.0f * mousePos.y) / m_window.getHeight();
    float ndcZ = 0.0f;

    Vector3 ndcPos = Vector3(ndcX, ndcY, ndcZ);
    g_Logger().print(mousePos);
    m_ik->fabrik(mousePos * 0.01f);
    if (mousePos.distanceTo(m_ik->getLastBone()->actorIni->getPosition3()) < 1.0f) {
    }
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC)) {
    m_changingType = false;
  }
  if (m_type == PHYSICS_TYPE::kEuler) {
    physics(deltaTime);
  }
}

void
PhysicsApp::fixedUpdate()
{
  if (m_type == PHYSICS_TYPE::kVerlet) {
    verletMove();
  }
}

void
PhysicsApp::physics(float _deltaTime)
{
  // calculate the Y direction of the fire direction
  m_fireDirection.y = -1.0f + Math::abs(m_fireDirection.x);
  m_fireDirection.y = Math::clamp(m_fireDirection.y, -1.0f, 0.0f);
  m_fireDirection.x = Math::clamp(m_fireDirection.x, -1.0f, 1.0f);

  //m_spring->move(_deltaTime);
  //m_spring->applyForce(Vector3::UP, m_spring->m_gravity, _deltaTime);

  // apply gravity to every projectile
  for (uint32_t i = 0; i < m_projectiles.size(); ++i) {
    if (m_projectiles[i]->m_fired) {
      m_projectiles[i]->gravity(_deltaTime);
    }
    // move and bounce the player
    m_projectiles[i]->move(_deltaTime, Vector3(m_projectiles[i]->m_direction.x *
                                                     m_projectiles[i]->m_speed,
                                                     m_projectiles[i]->m_direction.y *
                                                     m_projectiles[i]->m_speed,
                                                     0.0f));
    m_projectiles[i]->screenBounce(30, 17);
    // clamp the projectile speed
    m_projectiles[i]->m_speed = Math::clamp(m_projectiles[i]->m_speed,
                                            0,
                                            m_projectiles[i]->m_maxSpeed);
    if (m_projectiles[i]->m_actor->isActive()) {
      m_projectiles[i]->projTimer(_deltaTime);
    }
  }

  checkObstacles();
}

void
PhysicsApp::verletMove()
{
  float delta = pkEngineSDK::g_TimeManager().m_fixedDeltaTime;
  // calculate the Y direction of the fire direction
  m_fireDirection.y = -1.0f + Math::abs(m_fireDirection.x);
  m_fireDirection.y = Math::clamp(m_fireDirection.y, -1.0f, 0.0f);
  m_fireDirection.x = Math::clamp(m_fireDirection.x, -1.0f, 1.0f);

  //m_spring->move(delta);

  // apply gravity to every projectile
  for (uint32_t i = 0; i < m_projectiles.size(); ++i) {
    if (m_projectiles[i]->m_fired) {
      m_projectiles[i]->gravity(delta);
    }
    // move and bounce the player
    Vector3 direction = Vector3(m_projectiles[i]->m_direction.x,
                                m_projectiles[i]->m_direction.y,
                                0.0f);
    m_projectiles[i]->moveVerlet(direction, m_projectiles[i]->m_speed);
    m_projectiles[i]->screenBounce(30, 17);
    // clamp the projectile speed
    m_projectiles[i]->m_speed = Math::clamp(m_projectiles[i]->m_speed,
      0,
      m_projectiles[i]->m_maxSpeed);
    if (m_projectiles[i]->m_actor->isActive()) {
      m_projectiles[i]->projTimer(delta);
    }
    m_projectiles[i]->m_actor->m_prevTransform = m_projectiles[i]->m_actor->m_transform;
  }

  checkObstacles();
}

void
PhysicsApp::fireProjectile()
{
  Vector3 newPos = m_cannon->m_actor->m_transform.getTranslation3();
  newPos.z += 1.0f;
  for (uint32_t i = 0; i < m_projectiles.size(); ++i) {
    if (!m_projectiles[i]->m_actor->isActive())
    {
      m_projectiles[i]->fire(newPos, m_fireDirection);
      return;
    }
  }
}

void
PhysicsApp::checkObstacles()
{
  for (uint32 i = 0; i < m_projectiles.size(); ++i) {
    for (uint32 j = 0; j < obstacles.size(); ++j) {
      m_projectiles[i]->obstacleBounce(obstacles[j]->m_actor->getPosition3(),
                                       obstacles[j]->m_sphere.m_radius);
    }
  }
}

void
PhysicsApp::onRender()
{
  g_RenderManager().render();
  // // Set shaders
  // g_GraphicAPI().setVSShader(g_GraphicAPI().getVSShader());
  // g_GraphicAPI().setPSShader(g_GraphicAPI().getPSShader());
  // // set constant buffers for the pixel and vertex shaders
  // g_RenderManager().VSSetConstantBuffers();
  // g_RenderManager().PSSetConstantBuffers();
  // // render the objects
  // g_RenderManager().renderActors(_scene.m_actors);
}
