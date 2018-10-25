#include "Simulator.h"
#include <iostream>
#include <cmath>

using namespace std;
Mix_Chunk* paddleSound;
Mix_Chunk* whiffSound;
Mix_Chunk* bounceSound;

Simulator::Simulator(ScoreManager* sm, Ball* ba, Wall* wa[], Paddle* pa)
{
   scoreMgr = sm;
   ball = ba;
   walls = wa;
   paddle = pa;
   endGame = false;

   btDefaultCollisionConfiguration* collConfig = new btDefaultCollisionConfiguration();
   btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collConfig);
   btBroadphaseInterface* opc = new btDbvtBroadphase();
   btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
   dynWorld = new btDiscreteDynamicsWorld(dispatcher, opc, solver, collConfig);
   
   dynWorld->setGravity(btVector3(0,-1,0));

   /* Add sound */
  SDL_Init(SDL_INIT_AUDIO);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  paddleSound = Mix_LoadWAV("res/paddle.wav");
  whiffSound = Mix_LoadWAV("res/whiff.wav");
  bounceSound = Mix_LoadWAV("res/wallBounce.wav");

  for (int index = 0; index < 6; index++)
    walls[index]->initBullet(dynWorld);


  ball->AddPhysics(new btSphereShape(btScalar(1.)), btVector3(0, 0, 0), dynWorld);
  paddle->AddPhysics(new btBoxShape(btVector3(14, 14, 3)), btVector3(0, 0, -50), dynWorld);

  dynWorld->getCollisionObjectArray()[6]->setUserIndex(1);
  dynWorld->getCollisionObjectArray()[7]->setUserIndex(2);
  dynWorld->getCollisionObjectArray()[4]->setUserIndex(3);
}

struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback
{
    bool gameOver = false;
    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
      //Mix_HaltChannel(-1);
      
      if(colObj0Wrap->getCollisionObject()->getUserIndex() == 1 && colObj1Wrap->getCollisionObject()->getUserIndex() == 2)
      {
        Mix_PlayChannel(-1, paddleSound, 0);
        Mix_ExpireChannel(-1, 100);
        ((btRigidBody*)(colObj0Wrap->getCollisionObject()))->applyCentralImpulse(((btRigidBody*)(colObj0Wrap->getCollisionObject()))->getLinearVelocity().normalized()*.05/*btVector3(0, 0, 1)*/);
      }
      else if(colObj0Wrap->getCollisionObject()->getUserIndex() == 1 && colObj1Wrap->getCollisionObject()->getUserIndex() == 3) 
      {
        Mix_PlayChannel(-1, whiffSound, 0);
        Mix_ExpireChannel(-1, 300);
        btTransform trans;
        btRigidBody* body = ((btRigidBody*)(colObj0Wrap->getCollisionObject()));
        trans = body->getWorldTransform();
        body->translate(btVector3(trans.getOrigin().getX() * -1, trans.getOrigin().getY() * -1, trans.getOrigin().getZ() * -1));
        body->getMotionState()->setWorldTransform(trans);
        body->setLinearVelocity(btVector3(0, 0, 0));
        body->applyCentralImpulse(btVector3(5, 10, 10));
        gameOver = true;
      }
      else
      {
        Mix_PlayChannel(-1, bounceSound, 0);
        Mix_ExpireChannel(-1, 100);
      }
    }
};

Simulator::~Simulator()
{
    
}


void Simulator::stepSimulation()
{
  dynWorld->stepSimulation(.1f / 60.f, 10);

  btCollisionObject* obj = dynWorld->getCollisionObjectArray()[6];
  btRigidBody* body = btRigidBody::upcast(obj);
  btTransform trans;
  if (body && body->getMotionState())
  	body->getMotionState()->getWorldTransform(trans);
  ball->x = float(trans.getOrigin().getX());
  ball->y = float(trans.getOrigin().getY());
  ball->z = float(trans.getOrigin().getZ());

  MyContactResultCallback callback;
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[0]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[1]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[2]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[3]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[5]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[7]), callback);
  dynWorld->contactPairTest(body, btRigidBody::upcast(dynWorld->getCollisionObjectArray()[4]), callback);
  endGame = callback.gameOver;

  obj = dynWorld->getCollisionObjectArray()[7];
  body = btRigidBody::upcast(obj);

  trans = obj->getWorldTransform();

  if(abs(float(trans.getOrigin().getX())) > 45)
  {
    if((float(trans.getOrigin().getX()) > 45 && paddle->xTrans > 0) || (float(trans.getOrigin().getX()) < -45 && paddle->xTrans < 0))
      paddle->xTrans = 0;
  }
  if(abs(float(trans.getOrigin().getY())) > 45)
  {
    if((float(trans.getOrigin().getY()) > 45 && paddle->yTrans > 0) || (float(trans.getOrigin().getY()) < -45 && paddle->yTrans < 0))
      paddle->yTrans = 0;
  }
  btQuaternion rot = trans.getRotation();
  rot.setEuler(paddle->deg, 0, 0);
  body->translate(btVector3(paddle->xTrans, paddle->yTrans, 0));
  trans.setRotation(rot);
  body->getMotionState()->setWorldTransform(trans);
  trans = obj->getWorldTransform();

  
  paddle->x = float(trans.getOrigin().getX());
  paddle->y = float(trans.getOrigin().getY());
  paddle->z = float(trans.getOrigin().getZ());


  //print positions of all objects
  for (int j = dynWorld->getNumCollisionObjects() - 1; j >= 0; j--)
  {
  	btCollisionObject* obj = dynWorld->getCollisionObjectArray()[j];
  	btRigidBody* body = btRigidBody::upcast(obj);
  	btTransform trans;
  	if (body && body->getMotionState())
  		body->getMotionState()->getWorldTransform(trans);
  	else
    {
      //collision stuff
      
  		trans = obj->getWorldTransform();
    }
  	//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
  }
}
