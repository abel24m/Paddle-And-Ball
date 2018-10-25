#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <btBulletDynamicsCommon.h>

class Paddle 
{
	btCollisionShape* padShape;
	btVector3 origin;
	btDiscreteDynamicsWorld* dynWorld;

	public:
  		float x, y, z, xTrans, yTrans, deg;
		Paddle();
		void AddPhysics(btCollisionShape* padShape, btVector3 origin, btDiscreteDynamicsWorld* dynWorld);
		~Paddle();
		void move(float xMov, float yMov);
};

#endif