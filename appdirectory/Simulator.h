#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <btBulletDynamicsCommon.h>
#include "ScoreManager.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include <SDL_mixer.h>
#include <SDL.h>

class Simulator
{
  Ball* ball;
  Wall* *walls;
  Paddle* paddle;
  ScoreManager* scoreMgr;
  btDiscreteDynamicsWorld* dynWorld;

public:
  Simulator(ScoreManager* scoreMgr, Ball* ball, Wall* walls[], Paddle* paddle);
  // static bool gameOver;
  bool endGame;
  ~Simulator();

  void stepSimulation(void);
  void gameOver();
  // static void GameOver();

};

#endif
