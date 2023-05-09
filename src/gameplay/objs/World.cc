#include "World.hh"

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
    controller->tick(absTime);
    vec3 p, d, u;
    glm_vec3_copy(controller->currentStatus.pos, p);
    glm_vec3_copy(controller->currentStatus.up, u);
    glm_vec3_copy(controller->currentStatus.normal, d);
    int wx, wy;
    vtGetWindowSize(wx, wy);
    setState(p, d, u, fov, wx / (double)wy); // Keep other aspects
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

void World::castMouseRay(const vec2 coord, vec3 rayIn)
{
    vec4 ndc;
    ndc[0] = (2.0f * coord[0]) / screenSize[0] - 1.0f;
    ndc[1] = 1 - (2.0f * coord[1]) / screenSize[1];
    ndc[2] = -1;
    ndc[3] = 1;

    mat4 invProj, invView;
    activeCamera->getProjectionMatrixInv(invProj);
    activeCamera->getViewMatrixInv(invView);

    vec4 eyeRay;
    glm_mat4_mulv(invProj, ndc, eyeRay);
    eyeRay[2] = -1;
    eyeRay[3] = 0;
    vec4 fRay;
    glm_mat4_mulv(invView, eyeRay, fRay);

    glm_vec3_copy(fRay, rayIn);
    glm_normalize(rayIn);
}