#include "World.hh"

#include "base/util/Util.hh"

#define PERSPECTIVE_NEAR 0.1f
#define PERSPECTIVE_FAR 1000.0f

void Camera::setState(const vec3 pos_, const vec3 direction_, const vec3 up_, double fov_, double aspect_)
{
    vecmov(pos_, pos, 3);
    vecmov(direction_, direction, 3);
    vecmov(up_, up, 3);
    fov = fov_;
    aspect = aspect_;

    // Calculate and cache matrices
    glm_look(pos, direction, up, _cViewMatrix);
    glm_mat4_inv(_cViewMatrix, _cViewMatrixInv);

    glm_perspective(fov, aspect, PERSPECTIVE_NEAR, PERSPECTIVE_FAR, _cProjectionMatrix);
    glm_mat4_inv(_cProjectionMatrix, _cProjectionMatrixInv);
}

void Camera::getViewMatrix(mat4 viewIn)
{
    matmov(_cViewMatrix, viewIn, 4);
}

void Camera::getViewMatrixInv(mat4 viewIn)
{
    matmov(_cViewMatrixInv, viewIn, 4);
}

void Camera::getProjectionMatrix(mat4 projIn)
{
    matmov(_cProjectionMatrix, projIn, 4);
}

void Camera::getProjectionMatrixInv(mat4 projIn)
{
    matmov(_cProjectionMatrixInv, projIn, 4);
}

void World::castMouseRay(const vec2 coord, vec3 rayIn)
{
    vec4 ndc;
    ndc[0] = (2.0f * coord[0]) / szScrn[0] - 1.0f;
    ndc[1] = 1 - (2.0f * coord[1]) / szScrn[1];
    ndc[2] = -1;
    ndc[3] = 1;

    mat4 invProj, invView;
    camera.getProjectionMatrixInv(invProj);
    camera.getViewMatrixInv(invView);

    vec4 eyeRay;
    glm_mat4_mulv(invProj, ndc, eyeRay);
    eyeRay[2] = -1;
    eyeRay[3] = 0;

    vec4 fRay;
    glm_mat4_mulv(invView, eyeRay, fRay);
    vecmov(fRay, rayIn, 3);
    glm_normalize(rayIn);
}