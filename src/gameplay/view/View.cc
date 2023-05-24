#include "View.hh"

void castMouseRay(View &v, const vec2 coord, vec3 rayIn)
{
    vec4 ndc;
    ndc[0] = (2.0f * coord[0]) / v.screenSize[0] - 1.0f;
    ndc[1] = 1 - (2.0f * coord[1]) / v.screenSize[1];
    ndc[2] = -1;
    ndc[3] = 1;

    mat4 invProj, invView;
    auto cam = v.camera.lock();
    cam->getProjectionMatrixInv(invProj);
    cam->getViewMatrixInv(invView);

    vec4 eyeRay;
    glm_mat4_mulv(invProj, ndc, eyeRay);
    eyeRay[2] = -1;
    eyeRay[3] = 0;
    vec4 fRay;
    glm_mat4_mulv(invView, eyeRay, fRay);

    glm_vec3_copy(fRay, rayIn);
    glm_normalize(rayIn);
}