#ifndef _WALL_H_
#define _WALL_H_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

struct WallParams
{
	Ogre::String name, entityName, materialName;
	Ogre::Vector3 init, normal;
	btCollisionShape* groundShape;
	btVector3 origin;
};

class Wall 
{
	int side;
	btCollisionShape* groundShape;
	btVector3 origin;
	btDiscreteDynamicsWorld* dynWorld;
	Ogre::Plane* plane;

public:
	static const int SIDE_BOTTOM = 0;
	static const int SIDE_TOP = 1;
	static const int SIDE_LEFT = 2;
	static const int SIDE_RIGHT = 3;
	static const int SIDE_BACK = 4;
	static const int SIDE_FRONT = 5;

	Wall(int s);
	~Wall();
	void initOgre(Ogre::SceneManager* mSceneMgr);
	void initBullet(btDiscreteDynamicsWorld* dynWorld);
};

#endif
