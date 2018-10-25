#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_


#include "BaseApplication.h"

#include <stdint.h>
#include <chrono>
#include <time.h>
#include <stdlib.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <SDL_mixer.h>
#include <SDL.h>

using namespace std;

class GuiManager
{

private:
	  CEGUI::OgreRenderer* mRenderer;
	  
	  CEGUI::Window* volumeButton;
	  CEGUI::Window* quitButton;
	  CEGUI::Window* rootWindow;
	  CEGUI::Window* timerBox;
	  CEGUI::Window* scoreBox;

	  bool volume;

	  clock_t start_time;
	  clock_t current_time;
	  clock_t high_score;

      virtual bool soundOnOff(const CEGUI::EventArgs &e);
	  virtual void quit(const CEGUI::EventArgs &e);

protected:
	 

public:
  GuiManager();
  ~GuiManager();

  bool guiShutDown; 
  void createRootWindow();
  void createVolumeButton();
  void createQuitButton();
  void createTimerBox();
  void updateTimer();
  void createScoreBox();
  void gameReset();
  void initializeStartTime();
  void initializeHighScore();
  void guiFrameListener(OIS::ParamList pl);



};

#endif