#ifndef _ASSIGNMENT2_H_
#define _ASSIGNMENT2_H_

#include "BaseApplication.h"
#include "ScoreManager.h"
#include "Simulator.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "GuiManager.h"
#include <SDL_mixer.h>
#include <SDL.h>


class Assignment2 : public BaseApplication
{
  Ball* ball;
  Wall* *walls;
  Paddle* paddle;
  ScoreManager* scoreMgr;
  Simulator* simulator;
  GuiManager* guiMgr;

public:
  Assignment2();
  virtual ~Assignment2();

protected:
  virtual void createFrameListener(void);
  
  virtual void createScene();
  virtual void createCamera(void);
  virtual void createViewports(void);
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

   virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


private:
  bool processUnbufferedInput(const Ogre::FrameEvent& fe);

};

#endif
