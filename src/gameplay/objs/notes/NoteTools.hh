#ifndef GAMEPLAY_OBJS_NOTES_NOTETOOLS
#define GAMEPLAY_OBJS_NOTES_NOTETOOLS

#include "gameplay/score/ScoreValue.hh"
#include <glm/glm.hpp>

struct View;
struct GameRules;
struct InputBuffer;

/**
 * @brief Verify whether a note has been pressed in 3D space.
 *
 * This method uses ray casting to calculate ray and judging it by
 * using an angle threshold.
 */
bool isPressed3D(const glm::vec3 &pos, View &v, const InputBuffer &ibuf);

bool isPressed2D(const glm::vec3 origin[4], View &v, const InputBuffer &ibufs);

/**
 * @brief Generate 4 corner points using given position and size.
 *
 * The generated points follows the order LT, LB, RT, RB
 *
 * @param pos Position (center) vector.
 * @param up Up vector.
 * @param norm Normal vector.
 * @param size Half width of the rect.
 * @param[out] output Generated points.
 */
void createRect(const glm::vec3 &pos, const glm::vec3 &up, const glm::vec3 &norm, double sizew, double sizeh, glm::vec3 output[4]);

/**
 * @brief Get the grade of time-based notes using the time and rules defined.
 *
 * @note Applicable for Tapu/Puresu/Hoshi, other notes have their own judge method.
 * @param absTime Current time.
 * @param rules Game rules for judging.
 * @return Judged grade.
 */
ScoreGrade getTimingGrade(double absTime, double noteTime, const GameRules &rules);

#endif /* GAMEPLAY_OBJS_NOTES_NOTETOOLS */
