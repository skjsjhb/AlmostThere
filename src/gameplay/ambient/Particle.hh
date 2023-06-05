#ifndef GAMEPLAY_AMBIENT_PARTICLE
#define GAMEPLAY_AMBIENT_PARTICLE

#include <string>
#include <glm/glm.hpp>

class DrawList;

class Particle {
public:
    Particle(const glm::vec3 &initPos, const glm::vec3 &initV,
             const glm::vec3 &g, const glm::vec3 &norm, double genTime, double lifeTime);

    bool tick(double absTime);

    void draw(DrawList &d);

protected:
    glm::vec3 initPos, initV, gravity, normal;
    glm::vec3 currentPos, currentV;
    double genTime, lifeTime;
};

#endif /* GAMEPLAY_AMBIENT_PARTICLE */
