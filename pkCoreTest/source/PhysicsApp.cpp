#include <iostream>

#include "PhysicsApp.h"
#include "pkLogger.h"
#include "pkGraphicsAPI.h"
#include "pkMath.h"
#include "pkModel.h"
#include "pkRendererManager.h"
#include "pkTextureManager.h"
#include "pkVector4.h"
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
using pkEngineSDK::Vector4;
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
  m_spring->m_maxDistance = 10.0f;
  m_spring->m_anchor = anchor;
  m_spring->m_weight = weight;
  m_spring->m_elasticity = _stiffness;
  m_spring->m_length = _length;
  m_spring->m_mass = 1.0f;
  m_spring->m_gravity = 9.8f;

  SPtr<Obstacle> obs = make_shared<Obstacle>();
  obs->m_actor = anchor;
  obs->m_bounciness = 0.9f;
  obs->m_sphere.m_origin = anchor->getPosition3();
  obs->m_sphere.m_radius = 0.5f;
  obstacles.push_back(obs);

  anchor->setPosition(_pos);
  weight->setPosition(_pos.x, _pos.y + _length, _pos.y);
}

void
PhysicsApp::onInit()
{
  RendererManager& rm = g_RenderManager().instance();
  TextureManager& tm = g_TextureManager().instance();
  // set light
  rm.light.Type = pkEngineSDK::LIGHT_TYPE::kDirectional;
  rm.light.SpotCutoff = 1.0f;
  rm.light.SpotExponent = 32.0f;
  rm.light.LightDir = Vector3::FORWARD * -1.0f;
  rm.light.LightPos = Vector3(0.0f, 50.0f, 0.0f);
  rm.light.LightColor = Vector3(1.0f, 1.0f, 1.0f);

  m_camera.init(30,
                17,
                3.1416f / 4.0f,
                0.01f,
                1000.0f,
                Vector3(0.0f, 0.0f, -10.0f), // position
                Vector3(0.0f, 0.0f, 0.0f), // target
                Vector3(0.0f, 1.0f, 0.0f), // up vector
                pkEngineSDK::CAMERA_PROJ::kOrthographic);

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
   * @brief Create IK
   */
  m_ik = make_shared<InverseKinematics>();
  SPtr<Actor> childActor = nullptr;
  for (uint32 i = 0; i < 4; ++i) {
    SPtr<Actor> ikRoot = g_sceneManager().instantiate(childActor);
    ikRoot->addComponent(newModel("sphere.obj"));
    ikRoot->getComponent<Model>()->getMeshes()[0]->material->setDiffuse(tm.createTexture("blue.png"));

    m_ik->insertNodeLocal(Vector3(static_cast<float>(i), 0.0f, 0.0f), ikRoot);
    childActor = ikRoot;

    SPtr<Obstacle> obs = make_shared<Obstacle>();
    obs->m_actor = ikRoot;
    obs->m_bounciness = 0.9f;
    obs->m_sphere.m_origin = ikRoot->getPosition3();
    obs->m_sphere.m_radius = 0.5f;
    obstacles.push_back(obs);
  }

  /**
   * Instantiate spring
   */
  initSpring(m_ik->getLastBone()->actorIni->getPosition3(), 1, 15);

  m_fireDirection = Vector2(0.0f);
  m_target = Vector3(4.0f, 0.0f, 0.0f);
}


void
PhysicsApp::onUpdate()
{
  float deltaTime = g_TimeManager().m_deltaTime;
  // track the last ik bone for the spring
  Vector3 lastIkPos = m_ik->getLastBone()->actorIni->getPosition3Global();
  m_spring->m_anchor->setPosition(lastIkPos);
  // move the spring
  m_spring->move(deltaTime);
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
  Vector3 ikPos = m_ik->getLastBone()->actorIni->getPosition3();
  float speed = 1.0f * deltaTime;
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kA))
  {
    // m_fireDirection.x -= 1.0f * deltaTime;
    m_target += Vector3::LEFT * speed;
    if (m_ikType == IK_TYPE::kFabrik) {
      m_ik->fabrik(m_target);
    }
    else {
      m_ik->CCD(m_target, 4);
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kD))
  {
    // m_fireDirection.x += 1.0f * deltaTime;
    m_target += Vector3::RIGHT * speed;
    if (m_ikType == IK_TYPE::kFabrik) {
      m_ik->fabrik(m_target);
    }
    else {
      m_ik->CCD(m_target, 4);
    }
  }
  // move up or down
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kW))
  {
    // m_fireDirection.x -= 1.0f * deltaTime;
    m_target += Vector3::DOWN * speed;
    if (m_ikType == IK_TYPE::kFabrik) {
      m_ik->fabrik(m_target);
    }
    else {
      m_ik->CCD(m_target, 4);
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kS))
  {
    // m_fireDirection.x -= 1.0f * deltaTime;
    m_target += Vector3::UP * speed;
    if (m_ikType == IK_TYPE::kFabrik) {
      m_ik->fabrik(m_target);
    }
    else {
      m_ik->CCD(m_target, 4);
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kC) &&
    !m_changingType) {
    m_changingType = true;
    // change simulation type
    if (m_ikType == IK_TYPE::kCCD) {
      m_ikType = IK_TYPE::kFabrik;
      g_Logger().print("FABRIK Algorithm active.");
    }
    else {
      m_ikType = IK_TYPE::kCCD;
      g_Logger().print("CCD Algorithm active.");
    }
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kB)) {
    g_RenderManager().compileShaders();
  }
  // Vector3 weightPos = m_spring->m_weight->m_transform.getTranslation3();
  float strength = 1.0f * deltaTime;
  Vector3 posIK = m_ik->getLastBone()->actorIni->getPosition3();
  //m_spring->m_weight->m_transform.setTranslation(weightPos + Vector3::DOWN * deltaTime * strength);
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kUp)) {
    m_ik->m_bones[currentBone]->actorIni->m_transform *= Matrix4::rotationX(strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kDown)) {
    strength *= -1.0f;
    m_ik->m_bones[currentBone]->actorIni->m_transform *= Matrix4::rotationX(strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kRight)) {
    strength *= -1.0f;
    m_ik->m_bones[currentBone]->actorIni->m_transform *= Matrix4::rotationZ(strength);
  }
  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLeft)) {
    m_ik->m_bones[currentBone]->actorIni->m_transform *= Matrix4::rotationZ(strength);
  }

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kAdd) && !m_add) {
    ++currentBone;
    m_add = true;
    currentBone = static_cast<uint32>(Math::clamp(static_cast<float>(currentBone),
                                                  0.0f,
                                                  static_cast<float>(m_ik->m_bones.size()-1)));
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kAdd)) {
    m_add = false;
  }

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kSubtract) && !m_subtract) {
    --currentBone;
    m_subtract = true;
    currentBone = static_cast<uint32>(Math::clamp(static_cast<float>(currentBone),
                                                  0.0f,
                                                  static_cast<float>(m_ik->m_bones.size()-1)));
  }
  else if (!m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kSubtract)) {
    m_subtract = false;
  }

  if (m_eventQueue.iskeyPressed(pkEngineSDK::KEY::kLButton)) {
    Vector3 mousePos = Vector3(m_eventQueue.mousePosition.x,
                               m_eventQueue.mousePosition.y,
                               0.0f);
    float ndcX = (mousePos.x / m_window.getWidth()) * 2.0f - 1.0f;
    float ndcY = 1.0f - (mousePos.y / m_window.getHeight()) * 2.0f;

    Matrix4 viewProj = m_camera.projection * m_camera.view;
    Matrix4 invVP = viewProj.getInverse();

    Vector4 worldPos = Vector4(ndcX, ndcY, 0.0f, 1.0f) * invVP;
    mousePos.x = worldPos.x;
    mousePos.y = worldPos.y;
    mousePos.z = 0.0f;
    if (m_ikType == IK_TYPE::kFabrik) {
      m_ik->fabrik(mousePos);
    }
    else {
      m_ik->CCD(mousePos, 4);
    }
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
