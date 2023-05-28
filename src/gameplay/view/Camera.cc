#include "Camera.hh"

#include "util/Util.hh"
#include "gameplay/map/MapDef.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/objs/TickObject.hh"

#define PERSPECTIVE_NEAR 0.1f
#define PERSPECTIVE_FAR 100.0f

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/string_cast.hpp>

void Camera::setState(glm::vec3 pos_, glm::vec3 direction_, glm::vec3 up_, double fov_, double aspect_)
{
    pos = pos_;
    up = up_;
    direction = direction_;
    fov = fov_;
    aspect = aspect_;
    // Calculate and cache matrices
    _cViewMatrix = glm::lookAt(pos, pos + direction, up);
    _cViewMatrixInv = glm::inverse(_cViewMatrix);

    _cProjectionMatrix = glm::perspective(glm::radians(float(fov)), float(aspect), PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
    _cProjectionMatrixInv = glm::inverse(_cProjectionMatrix);
}

void Camera::tick()
{
    TickObject::tick();
    auto stat = controller->getState();
    int wx, wy;
    vtGetWindowSize(wx, wy);

    setState(stat.pos, stat.normal, stat.up, fov, wx / (double)wy);
}

std::shared_ptr<Camera> Camera::createCamera(std::weak_ptr<CameraObject> o, Game &g)
{
    auto pt = std::make_shared<Camera>(g);
    pt->controller = std::make_shared<ObjController>(*o.lock());
    return pt;
}

glm::mat4 Camera::getViewMatrix()
{
    return _cViewMatrix;
}

glm::mat4 Camera::getViewMatrixInv()
{
    return _cViewMatrixInv;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return _cProjectionMatrix;
}

glm::mat4 Camera::getProjectionMatrixInv()
{
    return _cProjectionMatrixInv;
}

glm::vec3 Camera::getPosition()
{
    return pos;
}

glm::vec3 Camera::getDir()
{
    return direction;
}
