#ifndef _BALL_H_
#define _BALL_H_

#include <stdint.h>
#include <btBulletDynamicsCommon.h>

class Ball
{
public:
  float x, y, z;

  Ball();
  void AddPhysics(btCollisionShape* colShape, btVector3 origin, btDiscreteDynamicsWorld* dynWorld);
  ~Ball();

};

#endif
