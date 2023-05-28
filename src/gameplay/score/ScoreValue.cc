#include "ScoreValue.hh"

unsigned int NOTE_SCORE_VALUE[NOTE_TYPE_SIZE][SCORE_GRADE_SIZE] = {
    {15, 12, 8, 0, 4, 0},   // Tapu
    {25, 20, 16, 8, 5, 0},  // Puresu
    {6, 6, 0, 0, 0, 0},     // Shizuku
    {10, 8, 6, 0, 0, 0},    // Ranku
    {5, 5, 3, 0, 0, 0},     // Kyozetsu
    {25, 20, 18, 0, 6, 0},  // Hoshi
    {30, 25, 20, 12, 8, 0}, // Hashi
    {8, 8, 5, 0, 0, 0},     // Suki
};

unsigned int COMBO_SCORE_VALUE[3][2] = {
    {0, 1},   // Begin from 0: x1
    {50, 2},  // Begin from 50: x2
    {200, 3}, // Begin from 200: x3
};

unsigned int EXPERT_COMBO_SCORE_VALUE[3][2] = {
    {0, 0},   // Begin from 0: x0
    {100, 1}, // Begin from 100: x1
    {150, 4}, // Begin from 150: x4
};
