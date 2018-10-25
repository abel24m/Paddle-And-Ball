#ifndef _SCOREMANAGER_H_
#define _SCOREMANAGER_H_

#include <stdint.h>

class ScoreManager
{
    uint32_t lives, total_score;
    int32_t round_start_time;

public:
  ScoreManager();
  ~ScoreManager();

};

#endif
