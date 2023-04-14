#ifndef BASE_WORLD_WORLD
#define BASE_WORLD_WORLD

#include <cglm/cglm.h>

class Camera
{

public:
    void getViewMatrix(mat4 viewIn);
    void getViewMatrixInv(mat4 viewIn);
    void getProjectionMatrix(mat4 projIn);
    void getProjectionMatrixInv(mat4 projIn);
    void setState(vec3 pos, vec3 direction, vec3 up, double fov, double aspect);
    void getPosition(vec3 pos);
    void getDir(vec3 dir);

protected:
    vec3 pos;
    vec3 direction;
    vec3 up;
    double fov;
    double aspect;

private:
    mat4 _cProjectionMatrix;    // Projection Matrix
    mat4 _cProjectionMatrixInv; // Inversed
    mat4 _cViewMatrix;          // View Matrix
    mat4 _cViewMatrixInv;       // Inversed
};

class World
{
public:
    Camera camera;
    vec2 szScrn;

    void castMouseRay(const vec2 coord, vec3 rayIn);
};

#endif /* BASE_WORLD_WORLD */
