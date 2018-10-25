#include "Assignment2.h"
#include <OgreCamera.h>
#include <OgreMeshManager.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <time.h>

using namespace std;

/*
initializing all objects ball and paddle and walls.
Also initialzing our bullet simulator.
*/
Assignment2::Assignment2()
{
  ball = new Ball();
  paddle  = new Paddle();
  
  walls = new Wall*[6];
  walls[0] = new Wall(Wall::SIDE_BOTTOM);
  walls[1] = new Wall(Wall::SIDE_TOP);
  walls[2] = new Wall(Wall::SIDE_LEFT);
  walls[3] = new Wall(Wall::SIDE_RIGHT);
  walls[4] = new Wall(Wall::SIDE_FRONT);
  walls[5] = new Wall(Wall::SIDE_BACK);

//Bullet simulator initializion
  simulator = new Simulator(scoreMgr, ball, walls, paddle);
}

Assignment2::~Assignment2()
{
  delete simulator;
}

/*
Creating the camera. We are placing right behind the paddle a pointing towards
the room the ball is bouncing it. 
*/
void Assignment2::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position camera and attach it to a SceneNode
    mCamera->setPosition(Ogre::Vector3(paddle->x,paddle->y,-150));
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
    Ogre::SceneNode* cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
    cameraNode->attachObject(mCamera);
    
	  Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup("UserDefinedMaterials");
	  Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("res", "FileSystem", "UserDefinedMaterials", true);
	  Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup("UserDefinedMaterials");
	  Ogre::ResourceGroupManager::getSingletonPtr()->loadResourceGroup("UserDefinedMaterials");
}

void Assignment2::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


/*
Creating all 3d objects in the scene. In addtion creating all gui windows
Score, timer, Sound, and quit button.
*/

void Assignment2::createScene()
{
  guiMgr = new GuiManager();

  mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
  mSceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

  /* Create a point light */
  Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setPosition(0, 0, -45);
  pointLight->setDiffuseColour(Ogre::ColourValue::White);
  pointLight->setSpecularColour(Ogre::ColourValue::White);

  /* Create the ball */
  Ogre::Entity* ballEntity = mSceneMgr->createEntity("sphere.mesh");
  ballEntity->setCastShadows(true);
  Ogre::SceneNode* ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode");
  ballNode->attachObject(ballEntity);
  ballNode->setScale(.06,.06,.06);
  ballEntity->setMaterialName("squash_ball");

  /* Create the paddle */
  Ogre::Entity* paddleEntity = mSceneMgr->createEntity("cube.mesh");
  //paddleEntity->setMaterialName("Bumpymetal");
  paddleEntity->setCastShadows(false);
  Ogre::SceneNode* paddleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaddleNode");
  paddleNode->attachObject(paddleEntity);
  paddleNode->setScale(.25, .25, .025);
  paddleEntity->setMaterialName("paddle");

  /*Create the walls*/
  for (int index = 0; index < 6; index++)
      walls[index]->initOgre(mSceneMgr);

  /*Create gui for gameplay attributes*/
  guiMgr->createRootWindow();
  guiMgr->createVolumeButton();
  guiMgr->createQuitButton();
  guiMgr->createTimerBox();
  guiMgr->createScoreBox();

}

void Assignment2::createFrameListener(void)
{

  /*This came from Basic Tutorial 7 for the GUI development*/
   Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);


}


bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& fe)
{

  //Need to capture/update each device
  mKeyboard->capture();
  mMouse->capture();

  //Need to inject timestamps to CEGUI System.
  CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);
  bool ret = BaseApplication::frameRenderingQueued(fe);
  
  /*Step simulation for bullet*/
  simulator->stepSimulation();

  /*Updating position of ball and paddle in the 3D world */
  mSceneMgr->getSceneNode("BallNode")->setPosition(Ogre::Vector3(ball->x, ball->y, ball->z));
  mSceneMgr->getSceneNode("PaddleNode")->setPosition(Ogre::Vector3(paddle->x, paddle->y, paddle->z));
  mSceneMgr->getSceneNode("PaddleNode")->yaw(Ogre::Degree(paddle->deg));
  paddle->deg = 0;

  /*This is the notification that the quit button has been pressed on GUI */
  if(guiMgr->guiShutDown){
    mShutDown = true;
  }
  /*Updating the timer in the GUI*/
  guiMgr->updateTimer();

  /*Checking to see if player has lost in order to reset game*/
  if(simulator->endGame){
    guiMgr->gameReset();
  }

  if(!processUnbufferedInput(fe))
    return false;
  return ret;
}

bool Assignment2::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
  static Ogre::Real rotate = .05;
  static Ogre::Real camMove = 20;
  static Ogre::Real move = .05;

  Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
  Ogre::Vector3 camVec = Ogre::Vector3::ZERO;
  Ogre::SceneNode* ball = mSceneMgr->getSceneNode("BallNode");

  
  /* Enables camera controls */
  if (mKeyboard->isKeyDown(OIS::KC_J))
  {    
    paddle->deg = .1;
  }
  if (mKeyboard->isKeyDown(OIS::KC_L))
  {
    paddle->deg = -.1;
  }

  if (mKeyboard->isKeyDown(OIS::KC_W))
  {
    dirVec.y += move;
    //paddle->y += move;
  }

  if (mKeyboard->isKeyDown(OIS::KC_A))
  {
    dirVec.x += move;
    //paddle->x += move;
  }

  if (mKeyboard->isKeyDown(OIS::KC_S))
  {
    dirVec.y -= move;
    //paddle->y -= move;
  }

  if (mKeyboard->isKeyDown(OIS::KC_D))
  {
    dirVec.x -= move;
    //paddle->x -= move;
  }

  paddle -> move(dirVec.x, dirVec.y);
  
  mSceneMgr->getCamera("PlayerCam")->setPosition(Ogre::Vector3(paddle->x,paddle->y,-150));


  
  mSceneMgr->getSceneNode("CameraNode")->translate(
    camVec * fe.timeSinceLastFrame,
    Ogre::Node::TS_LOCAL);
  return true;
}

//Converts OIS button pressed for GUI
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

//Call back methos for keypress
bool Assignment2::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    return true;

}
//-------------------------------------------------------------------------------------
bool Assignment2::keyReleased( const OIS::KeyEvent &arg )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
  return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mouseMoved( const OIS::MouseEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
  // Scroll wheel.
  if (arg.state.Z.rel)
      sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
  return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{  
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
  return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
  return true;
}





#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Assignment2 app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------

