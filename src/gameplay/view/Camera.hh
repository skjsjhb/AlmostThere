#ifndef GAMEPLAY_VIEW_CAMERA
#define GAMEPLAY_VIEW_CAMERA

#include "gameplay/objs/TickObject.hh"
#include <cglm/cglm.h>

class CameraObject;

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

    // A camera is an always-loaded object
    virtual bool shouldTick(double absTime) const { return true; };

    virtual double getTickTime() { return 0; };

    static std::shared_ptr<Camera> createCamera(std::weak_ptr<CameraObject> o);

protected:
    vec3 pos;
    vec3 direction;
    vec3 up;
    double fov = 90.0;
    double aspect = 1920.0 / 1080.0;
    mat4 _cProjectionMatrix;    // Projection Matrix
    mat4 _cProjectionMatrixInv; // Inversed
    mat4 _cViewMatrix;          // View Matrix
    mat4 _cViewMatrixInv;       // Inversed
};

#endif /* GAMEPLAY_VIEW_CAMERA */
