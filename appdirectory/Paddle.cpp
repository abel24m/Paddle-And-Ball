#include "Paddle.h"
#include <cmath>

Paddle::Paddle()
{
    x = y = z = xTrans = yTrans = 0;
    deg = 0;
}

void Paddle::AddPhysics(btCollisionShape* padShape, btVector3 origin, btDiscreteDynamicsWorld* dynWorld)
{
    btTransform paddleTransform;
    paddleTransform.setIdentity();
    paddleTransform.setOrigin(origin);
    
    btScalar mass(0.);
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        padShape->calculateLocalInertia(mass, localInertia);
    
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(paddleTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, padShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setRestitution(0.8f);
    body->setDamping(0.0f, 0.0f);
    body->setFriction(0.0f);
    
    //add the body to the dynamics world
    dynWorld->addRigidBody(body);
}

void Paddle::move(float xMov, float yMov)
{
    xTrans = xMov;
    yTrans = yMov;
    /*if(abs(x) < 47.5)
        xTrans = xMov;
    else
    {
        if((x >= 47.5 && xMov < 0) || (x <= -47.5 && xMov > 0))
            xTrans = xMov;
        else
            xTrans = 0;
    }
    if(abs(y) < 47.5)
        yTrans = yMov;
    else
    {
        if((y >= 47.5 && yMov < 0) || (y <= -47.5 && yMov > 0))
            yTrans = yMov;
        else
            yTrans = 0;
    }*/
}