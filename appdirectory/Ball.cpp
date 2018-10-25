#include "Ball.h"

Ball::Ball()
{
    x = y = z = 0;
}

void Ball::AddPhysics(btCollisionShape* colShape, btVector3 origin, btDiscreteDynamicsWorld* dynWorld)
{
    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();
    
    btScalar mass(1.0f);
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    	colShape->calculateLocalInertia(mass, localInertia);
    
    startTransform.setOrigin(origin);
    
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    //rbInfo.m_restitution = 50.0f;
    btRigidBody* body = new btRigidBody(rbInfo);
    body->applyCentralImpulse(btVector3(5, 10, 10));
    //body->setLinearVelocity(btVector3(0, 50, 0));
    body->setRestitution(1.0f);
    body->setDamping(0.0f, 0.0f);
    body->setFriction(0.0f);
    
    dynWorld->addRigidBody(body);
}

Ball::~Ball()
{

}
