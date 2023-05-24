#include "Camera.hh"

#include "util/Util.hh"
#include "engine/virtual/Window.hh"

#define PERSPECTIVE_NEAR 1.0f
#define PERSPECTIVE_FAR 100.0f

void Camera::setState(vec3 pos_, vec3 direction_, vec3 up_, double fov_, double aspect_)
{
    glm_vec3_copy(pos_, pos);
    glm_vec3_copy(direction_, direction);
    glm_vec3_copy(up_, up);
    fov = fov_;
    aspect = aspect_;

    // Calculate and cache matrices
    glm_look(pos, direction, up, _cViewMatrix);
    glm_mat4_inv(_cViewMatrix, _cViewMatrixInv);

    glm_perspective(fov, aspect, PERSPECTIVE_NEAR, PERSPECTIVE_FAR, _cProjectionMatrix);
    glm_mat4_inv(_cProjectionMatrix, _cProjectionMatrixInv);
}

void Camera::tick(double absTime)
{
    TickObject::tick(absTime);
    auto stat = controller->getState();
    int wx, wy;
    vtGetWindowSize(wx, wy);
    setState(stat.pos, stat.normal, stat.up, fov, wx / (double)wy);
}

std::shared_ptr<Camera> Camera::createCamera(std::weak_ptr<CameraObject> o)
{
    auto pt = std::make_shared<Camera>();
    pt->controller = std::make_shared<ObjController>(*o.lock());
    return pt;
}

void Camera::getViewMatrix(mat4 viewIn)
{
    glm_mat4_copy(_cViewMatrix, viewIn);
}

void Camera::getViewMatrixInv(mat4 viewIn)
{
    glm_mat4_copy(_cViewMatrixInv, viewIn);
}

void Camera::getProjectionMatrix(mat4 projIn)
{
    glm_mat4_copy(_cProjectionMatrix, projIn);
}

void Camera::getProjectionMatrixInv(mat4 projIn)
{
    glm_mat4_copy(_cProjectionMatrixInv, projIn);
}

void Camera::getPosition(vec3 posIn)
{
    glm_vec3_copy(pos, posIn);
}

void Camera::getDir(vec3 dirIn)
{
    glm_vec3_copy(direction, dirIn);
}