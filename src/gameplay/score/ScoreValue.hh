#ifndef GAMEPLAY_SCORE_SCOREVALUE
#define GAMEPLAY_SCORE_SCOREVALUE

#include "gameplay/objs/NoteDef.hh"

enum ScoreGrade {
  PF,
  AT,
  AC,
  MD,
  TC,
  LT
};

#define SCORE_GRADE_SIZE 6

// The value parameters of scores

extern unsigned int NOTE_SCORE_VALUE[NOTE_TYPE_SIZE][SCORE_GRADE_SIZE];

extern unsigned int COMBO_SCORE_VALUE[3][2];

extern unsigned int EXPERT_COMBO_SCORE_VALUE[3][2];

#endif /* GAMEPLAY_SCORE_SCOREVALUE */
