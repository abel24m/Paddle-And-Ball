#include "Wall.h"

WallParams SideWallParams[] =
{
	{ "bottom", "BottomEntity", "squash_bottom", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z,
        new btBoxShape(btVector3(btScalar(63.), btScalar(20.), btScalar(63.))), btVector3(0, -77.5, 0) }, 
    { "top", "TopEntity", "squash_top", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::NEGATIVE_UNIT_Z,
        new btBoxShape(btVector3(btScalar(63.), btScalar(20.), btScalar(63.))), btVector3(0, 77.5, 0) },
	{ "left", "LeftEntity", "squash_top", Ogre::Vector3::UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y,
        new btBoxShape(btVector3(btScalar(20.), btScalar(63.), btScalar(63.))), btVector3(-77.5, 0, 0) },
	{ "right", "RightEntity", "squash_back", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y,
        new btBoxShape(btVector3(btScalar(20.), btScalar(63.), btScalar(63.))), btVector3(77.5, 0, 0) },
	{ "back", "BackEntity", "squash_left", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Y,
        new btBoxShape(btVector3(btScalar(63.), btScalar(63.), btScalar(20.))), btVector3(0, 0, 77.5) },
	{ "front", "FrontEntity", "squash_right", Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Y,
        new btBoxShape(btVector3(btScalar(63.), btScalar(63.), btScalar(20.))), btVector3(0, 0, -77.5) }
};

Wall::Wall(int s)
{
    side = s;
    plane = new Ogre::Plane(SideWallParams[side].init, -62.5);
}

void Wall::initOgre(Ogre::SceneManager* mSceneMgr)
{
    WallParams params = SideWallParams[side];

    /* Add planes for walls */
    Ogre::MeshManager::getSingleton().createPlane(
        params.name, 
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        *plane, 126, 126, 20, 20, true, 1, 1, 1, params.normal);

    /* Attach planes to SceneNodes to create walls*/
    Ogre::Entity* entBottom = mSceneMgr->createEntity(params.entityName, params.name);
    mSceneMgr->getRootSceneNode()->createChildSceneNode(params.name)->attachObject(entBottom);

    /* Add texture to the walls */
    entBottom->setMaterialName(params.materialName);
    entBottom->setCastShadows(true);
}

void Wall::initBullet(btDiscreteDynamicsWorld* dynWorld)
{
    WallParams params = SideWallParams[side];

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(params.origin);
    
    btScalar mass(0.);
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    	params.groundShape->calculateLocalInertia(mass, localInertia);
    
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, params.groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    body->setRestitution(1.0f);
    body->setDamping(0.0f, 0.0f);
    body->setFriction(0.0f);
    
    //add the body to the dynamics world
    dynWorld->addRigidBody(body);
}
