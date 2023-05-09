#ifndef GAMEPLAY_OBJS_WORLD
#define GAMEPLAY_OBJS_WORLD

#include <cglm/cglm.h>
#include <map>
#include <string>
#include "gameplay/objs/TickObject.hh"
#include "gameplay/control/Controller.hh"

class Camera : public TickObject
{

public:
    void getViewMatrix(mat4 viewIn);
    void getViewMatrixInv(mat4 viewIn);
    void getProjectionMatrix(mat4 projIn);
    void getProjectionMatrixInv(mat4 projIn);
    void setState(vec3 pos, vec3 direction, vec3 up, double fov, double aspect);
    void getPosition(vec3 pos);
    void getDir(vec3 dir);
    void tick(double absTime) override;

protected:
    vec3 pos;
    vec3 direction;
    vec3 up;
    double fov = 90.0;
    double aspect = 1920.0 / 1080.0;

private:
    mat4 _cProjectionMatrix;    // Projection Matrix
    mat4 _cProjectionMatrixInv; // Inversed
    mat4 _cViewMatrix;          // View Matrix
    mat4 _cViewMatrixInv;       // Inversed
};

class World
{
public:
    Camera *activeCamera = nullptr;
    vec2 screenSize;
    void castMouseRay(const vec2 coord, vec3 rayIn);
};

#endif /* GAMEPLAY_OBJS_WORLD */
