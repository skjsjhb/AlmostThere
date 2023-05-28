#include "Hashi.hh"

#include "gameplay/base/Game.hh"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define HASHI_LENGTH_SCALE 10.0

void Hashi::performJudge()
{
    if (isFake)
    {
        return;
    }
    auto time = game.mapTimer.getTime();
    auto &ref = controller->getReference();
    switch (judgeStage)
    {
    case JUDGED:
        isActive = false;
        return;
    default:
        if (time > ref.endTime && time < ref.endTime + ref.length)
        {
            if (lastSuccJudge == -1)
            {
                lastSuccJudge = time;
                return;
            }
            if (isPressed())
            {
                judgedLength += (time - lastSuccJudge);
            }
            lastSuccJudge = time;
        }
        else if (time > ref.endTime + ref.length)
        {
            judgeStage = JUDGED;
            auto comRate = judgedLength / ref.length;
            if (comRate > 0.95)
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, PF));
            }
            else if (comRate > 0.85)
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, AT));
            }
            else if (comRate > 0.70)
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, AC));
            }
            else if (comRate > 0.45)
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, MD));
            }
            else if (comRate > 0)
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, TC));
            }
            else
            {
                game.score.addRecord(NoteScoreEntry::create(HASHI, LT));
            }
        }
    }
}

void Hashi::draw()
{
    auto stat = controller->getState();
    if (!isActive || stat.len <= 0)
    {
        return;
    }
    glm::vec3 btmPoints[6], headPoints[6]; // CCW start from right
    auto right = glm::normalize(glm::cross(stat.up, stat.normal)) * float(sizew);
    Polygon ps;
    for (int i = 0; i < 6; i++)
    {
        btmPoints[i] = stat.pos + right;
        ps.points.push_back(btmPoints[i]);
        if (i != 5)
        {
            right = glm::rotate(right, glm::radians(60.0f), stat.normal);
        }
    }

    auto upLength = glm::normalize(stat.normal) * float(stat.len * HASHI_LENGTH_SCALE);

    // In the past, long Hashi notes requires cutting in order to render them
    // correctly. Now Hashi are opaque and there is no need to calculate this
    // anymore.
    for (int i = 0; i < 6; i++)
    {
        // Get corresponding point on the head face
        headPoints[i] = btmPoints[i] + upLength;
        ps.points.push_back(headPoints[i]);
    }

    ps.renderPreset = PRISM_FULL;
    ps.shader = "3d/hashi";
    ps.texture = "hashi-hat";
    ps.subTexture = "hashi-side";
    game.drawContext.polygons.push_back(ps);
}
