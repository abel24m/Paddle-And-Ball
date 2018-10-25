#include "GuiManager.h"

GuiManager::GuiManager()
{
  mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
  CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
  volume = true;
  guiShutDown = false;
  createRootWindow();
}

GuiManager::~GuiManager()
{

}

/*Creating the root window for the GUI. The parent Window*/
void GuiManager::createRootWindow(){
 	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	rootWindow = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
  	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWindow);
}

/*Creating volume button on the buttom right side of the screen*/
void GuiManager::createVolumeButton(){
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  volumeButton = wmgr.createWindow("AlfiskoSkin/Button", "CEGUI/VolumeButton");
  //Setting the text to ON initializtion
  volumeButton->setText("Sound: ON");
  volumeButton->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
  volumeButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85,0),CEGUI::UDim(0.95,0)));
  volumeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiManager::soundOnOff, this));
  rootWindow->addChild(volumeButton);
}

/*Create the quit button on the bottom left side of the screen*/
void GuiManager::createQuitButton(){
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  quitButton = wmgr.createWindow("AlfiskoSkin/Button", "CEGUI/quitButton");
  quitButton->setText("QUIT");
  quitButton->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));
  quitButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0,0),CEGUI::UDim(0.95,0)));
  quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiManager::quit, this));
  rootWindow->addChild(quitButton);
}

/*Create timer box that will be placed on the top left of the screen
Also intialize start time and initialize the highscore*/
void GuiManager::createTimerBox(){
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  timerBox = wmgr.createWindow("AlfiskoSkin/Button", "CEGUI/Timer");
  timerBox->setText("0:00");
  timerBox->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
  initializeStartTime();
  initializeHighScore();
  rootWindow->addChild(timerBox);
}

/*Updaitng the timer box by finding the different of the current time and the start time*/
void GuiManager::updateTimer(){
  current_time = clock()-start_time;
  int seconds = (int)floor(((float) current_time)/CLOCKS_PER_SEC);
  int minutes = seconds / 60;
  seconds = seconds % 60;

  //Convertime time to string 
  string sec = to_string(seconds);
  string min = to_string(minutes);
  string time = "";

  if (seconds >= 10){
      time = min + ":" + sec;
  }else
      time = min + ":0" + sec;

  timerBox->setText(time);
}

/*Creating Scorebox which holds the value of the high score
initialized at 0:00. Placed on the top right corner*/
void GuiManager::createScoreBox(){
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  scoreBox = wmgr.createWindow("AlfiskoSkin/Button", "CEGUI/HighScore");
  scoreBox->setText("High Score: 0:00" );
  scoreBox->setSize(CEGUI::USize(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.05, 0)));
  scoreBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75,0),CEGUI::UDim(0.0,0)));
  rootWindow->addChild(scoreBox);
}

/*Reset tiemr back to 0:00 and check if player score is new High Score*/
void GuiManager::gameReset(){
  //Checking if score is greater than high score
  if (high_score < current_time){
    high_score = current_time;
    //conversiong of time to string 
    int seconds = (int)floor(((float) high_score)/CLOCKS_PER_SEC);
    int minutes = seconds / 60;
    seconds = seconds % 60;
    string sec = to_string(seconds);
    string min = to_string(minutes);
    string time = "";
    if (seconds >= 10){
        time = min + ":" + sec;
    }else
        time = min + ":0" + sec;

    scoreBox->setText("High Score: " + time);
  }
  start_time = clock();
}
//simple initializion of start time
void GuiManager::initializeStartTime(){
	start_time = clock();
}
//simple initializaion of high score
void GuiManager::initializeHighScore(){
	high_score = clock() - clock();
}

//Function that is called when sound button is pressed. Volume is alternated from 
//ON and OFF and text is changed also. According to the state
bool GuiManager::soundOnOff(const CEGUI::EventArgs &e){
	if(volume){
		Mix_Volume(-1,0);
		volumeButton->setText("Sound:OFF");
		volume = false;
	}else{
		Mix_Volume(-1,128);
		volumeButton->setText("Sound:ON");
		volume = true;
	}
	return true;
}

/*Setting the boolean that lets the application know that the quit button has been presseed*/
void GuiManager::quit(const CEGUI::EventArgs &e){
	guiShutDown = true;
}









