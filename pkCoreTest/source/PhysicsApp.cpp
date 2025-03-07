#include <iostream>

#include "PhysicsApp.h"
#include "pkDebug.h"
#include "pkGraphicsAPI.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkRendererManager.h"
#include "pkVector3.h"
#include "pkVector2.h"

using pkEngineSDK::Debug;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::Light;
using pkEngineSDK::Math;
using pkEngineSDK::Model;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;

using std::make_shared;

void
PhysicsApp::initSpring(Vector3 _pos, float _length, float _stiffness)
{
  m_scene.instantiate();
  SPtr<Actor> anchor = m_scene.getLastActor();
  anchor->addComponent(newModel("sprite.fbx"));
  anchor->addComponent(createMaterial());
  anchor->getComponent<Material>()->setDiffuse(createTexture("circle.png"));
  m_scene.instantiate();
  SPtr<Actor> weight = m_scene.getLastActor();
  weight->addComponent(newModel("sprite.fbx"));
  weight->addComponent(createMaterial());
  weight->getComponent<Material>()->setDiffuse(createTexture("circle.png"));

  m_spring = make_shared<Spring>();
  m_spring->m_anchor = anchor;
  m_spring->m_weight = weight;
  m_spring->m_elasticity = _stiffness;
  m_spring->m_length = _length;
  m_spring->m_mass = 1.0f;

  anchor->setPosition(_pos);
  weight->setPosition(_pos.x, _pos.y + _length, _pos.y);
}

void
PhysicsApp::onInit()
{
  m_camera.init(30,
                17,
                3.1416f / 4.0f,
                0.01f,
                1000.0f,
                Vector3(0.0f, 0.0f, -30.0f), // position
                Vector3(0.0f, 0.0f, 0.0f), // target
                Vector3(0.0f, 1.0f, 0.0f), // up vector
                pkEngineSDK::CAMERA_PROJ::kOrthographic);

  m_type = PHYSICS_TYPE::kEuler;

  // create one model instance
  m_projectileCount = 100;
  m_projDuration = 10.0f;
  float projSpeed = 80.0f;
  m_projectileMaterial = createMaterial();
  m_projectileMaterial->setDiffuse(createTexture("circle.png"));

  /**
   * Cannon creation
   */
  m_scene.instantiate();
  m_cannon = std::make_shared<Cannon>();
  m_cannon->m_actor = m_scene.m_actors[0];
  m_cannon->m_actor->addComponent(newModel("sprite.fbx"));
  m_cannon->m_actor->addComponent(createMaterial());
  m_cannon->m_actor->getComponent<Material>()->setDiffuse(createTexture("Canon.png"));
  m_cannon->m_actor->move(Vector3(-0.0f, 0.0f, 0.0f));
  m_cannon->m_actor->setPosition(Vector3(0.0f, 7.0f, 0.0f));
  m_cannon->m_actor->setRotation(0.0f, 0.0f, -1.5708f);

  for (uint32_t i = 0; i < m_projectileCount; ++i) {
    // instantiate an actor
    m_scene.instantiate();
    // make new instance of a projectile
    SPtr<Projectile> proj = make_shared<Projectile>();
    // get the last instance
    proj->m_actor = m_scene.m_actors[m_scene.m_actors.size() - 1];
    // start the projectile
    proj->start();
    // set projectile speed
    proj->m_speed = projSpeed;
    proj->m_maxSpeed = projSpeed;
    // set the projectile lifetime
    proj->m_lifeTimer = m_projDuration;
    // assign a new model component to the game object.
    proj->m_actor->addComponent(newModel("sprite.fbx"));
    proj->m_actor->addComponent(m_projectileMaterial);
    // add the game object to the vector of projectiles
    m_projectiles.push_back(proj);
  }

  m_scene.instantiate();
  SPtr<Actor> obst = m_scene.m_actors[m_scene.m_actors.size() - 1];
  obst->m_transform = Matrix4::IDENTITY;
  obstacle.start(Vector3(0), 1, 0.9f, obst);
  obstacle.m_actor->addComponent(newModel("sprite.fbx"));
  obstacle.m_actor->addComponent(createMaterial());
  obstacle.m_actor->getComponent<Material>()->setDiffuse(createTexture("obstacle.png"));

  initSpring(Vector3(4.0f, 0.0f, 0.0f), 3, 1);

  m_fireDirection = Vector2(0.0f);
}


void
PhysicsApp::onUpdate(float _deltaTime)
{
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
    m_fireDirection.x -= 1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD))
  {
    m_fireDirection.x += 1.0f * _deltaTime;
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) &&
    !m_changingType) {
    m_changingType = true;
    // change simulation type
    if (m_type == PHYSICS_TYPE::kEuler) {
      m_type = PHYSICS_TYPE::kVerlet;
      Debug::print("Verlet integration active");
    }
    else {
      m_type = PHYSICS_TYPE::kEuler;
      Debug::print("Euler integration active");
    }
  }
  Vector3 weightPos = m_spring->m_weight->m_transform.getTranslation3();
  float strength = 10.0f;
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::DOWN * _deltaTime * strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::UP * _deltaTime * strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::RIGHT * _deltaTime * strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::LEFT * _deltaTime * strength);
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC)) {
    m_changingType = false;
  }
  if (m_type == PHYSICS_TYPE::kEuler) {
    physics(_deltaTime);
  }
}

void
PhysicsApp::fixedUpdate()
{
  if (m_type == PHYSICS_TYPE::kVerlet) {
    physics(m_fixedDeltaTime);
  }
}

void
PhysicsApp::physics(float _deltaTime)
{
  // calculate the Y direction of the fire direction
  m_fireDirection.y = -1.0f + Math::abs(m_fireDirection.x);
  m_fireDirection.y = Math::clamp(m_fireDirection.y, -1.0f, 0.0f);
  m_fireDirection.x = Math::clamp(m_fireDirection.x, -1.0f, 1.0f);

  m_spring->move(_deltaTime);

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

  m_spring->move(delta);

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
    m_projectiles[i]->obstacleBounce(obstacle.m_actor->m_transform.getTranslation3(), 
                                     obstacle.m_sphere.m_radius);
  }
}

void
PhysicsApp::onRender(Scene& _scene)
{
  g_RenderManager().render(_scene);
  // // Set shaders
  // g_GraphicAPI().setVSShader(g_GraphicAPI().getVSShader());
  // g_GraphicAPI().setPSShader(g_GraphicAPI().getPSShader());
  // // set constant buffers for the pixel and vertex shaders
  // g_RenderManager().VSSetConstantBuffers();
  // g_RenderManager().PSSetConstantBuffers();
  // // render the objects
  // g_RenderManager().renderActors(_scene.m_actors);
}
