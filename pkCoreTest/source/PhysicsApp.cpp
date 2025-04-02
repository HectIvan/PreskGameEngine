#include <iostream>

#include "PhysicsApp.h"
#include "pkDebug.h"
#include "pkGraphicsAPI.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkRendererManager.h"
#include "pkTextureManager.h"
#include "pkVector3.h"
#include "pkVector2.h"

using pkEngineSDK::g_Logger;
using pkEngineSDK::g_GraphicAPI;
using pkEngineSDK::g_RenderManager;
using pkEngineSDK::g_TimeManager;
using pkEngineSDK::g_sceneManager;
using pkEngineSDK::g_TextureManager;
using pkEngineSDK::Light;
using pkEngineSDK::Math;
using pkEngineSDK::Model;
using pkEngineSDK::TextureManager;
using pkEngineSDK::uint32;
using pkEngineSDK::Vector3;
using pkEngineSDK::Vector2;
using pkEngineSDK::Matrix4;
using pkEngineSDK::Key;

using std::make_shared;

void
PhysicsApp::initSpring(Vector3 _pos, float _length, float _stiffness)
{
  // get the texture manager
  TextureManager& tm = g_TextureManager().instance();
  g_sceneManager().instantiate();
  SPtr<Actor> anchor = g_sceneManager().getLastActor();
  anchor->addComponent(newModel("sprite.fbx"));
  anchor->addComponent(createMaterial());
  anchor->getComponent<Material>()->setDiffuse(tm.createTexture("circle.png"));
  g_sceneManager().instantiate();
  SPtr<Actor> weight = g_sceneManager().getLastActor();
  weight->addComponent(newModel("sprite.fbx"));
  weight->addComponent(createMaterial());
  weight->getComponent<Material>()->setDiffuse(tm.createTexture("circle.png"));

  m_spring = make_shared<Spring>();
  m_spring->m_anchor = anchor;
  m_spring->m_weight = weight;
  m_spring->m_elasticity = _stiffness;
  m_spring->m_length = _length;
  m_spring->m_mass = 1.0f;
  m_spring->m_gravity = 9.81f;

  anchor->setPosition(_pos);
  weight->setPosition(_pos.x, _pos.y + _length, _pos.y);
}

void
PhysicsApp::onInit()
{
  // get the texture manager
  TextureManager& tm = g_TextureManager().instance();
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
  m_projectileMaterial->setDiffuse(tm.createTexture("circle.png"));

  /**
   * Cannon creation
   */
  m_scene.instantiate();
  m_cannon = std::make_shared<Cannon>();
  m_cannon->m_actor = m_scene.m_actors[0];
  m_cannon->m_actor->addComponent(newModel("sprite.fbx"));
  m_cannon->m_actor->addComponent(createMaterial());
  m_cannon->m_actor->getComponent<Material>()->setDiffuse(tm.createTexture("Canon.png"));
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
    proj->m_actor->addComponent(createMaterial());
    proj->m_actor->getComponent<Material>()->setDiffuse(tm.createTexture("circle.png"));
    // add the game object to the vector of projectiles
    m_projectiles.push_back(proj);
  }

  m_scene.instantiate();
  SPtr<Actor> obst = m_scene.m_actors[m_scene.m_actors.size() - 1];
  obst->m_transform = Matrix4::IDENTITY;
  obstacle.start(Vector3(0), 1, 0.9f, obst);
  obstacle.m_actor->addComponent(newModel("sprite.fbx"));
  obstacle.m_actor->addComponent(createMaterial());
  obstacle.m_actor->getComponent<Material>()->setDiffuse(tm.createTexture("obstacle.png"));

  /**
   * Instantiate spring
   */
  // initSpring(Vector3(4.0f, 0.0f, 0.0f), 3, 1);

  /**
   * @brief Create IK
   */
  m_ik = make_shared<InverseKinematics>();
  for (uint32 i = 0; i < 4; ++i)
  {
    SPtr<Actor> ikRoot = g_sceneManager().instantiate();
    ikRoot->addComponent(newModel("sprite.fbx"));
    ikRoot->addComponent(createMaterial());
    ikRoot->getComponent<Material>()->setDiffuse(tm.createTexture("circle.png"));

    m_ik->insertNodeLocal(Vector3(i * 2, i * g_TimeManager().m_fixedDeltaTime, 0), ikRoot);
  }

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
      g_Logger().print("Verlet integration active");
    }
    else {
      m_type = PHYSICS_TYPE::kEuler;
      g_Logger().print("Euler integration active");
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kB)) {
    g_RenderManager().compileShaders();
  float strength = 10.0f;
  float spd = 3;
  Vector3 ikLastPos = m_ik->getLastBone()->actorIni->m_transform.getTranslation3();
  /**
   * Inverse kinematics input
   */
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    Vector3 speed = Vector3(0.0f, -spd, 0.0f) * g_TimeManager().m_deltaTime;
    m_ik->fabrik(ikLastPos + speed);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    Vector3 speed = Vector3(0.0f, spd, 0.0f) * g_TimeManager().m_deltaTime;
    m_ik->fabrik(ikLastPos + speed);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    Vector3 speed = Vector3(spd, 0.0f, 0.0f) * g_TimeManager().m_deltaTime;
    m_ik->fabrik(ikLastPos + speed);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    Vector3 speed = Vector3(0.0f, -spd, 0.0f) * g_TimeManager().m_deltaTime;
    m_ik->fabrik(ikLastPos + speed);
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

  m_spring->move(_deltaTime);
  m_spring->applyForce(Vector3::UP, m_spring->m_gravity, _deltaTime);

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
