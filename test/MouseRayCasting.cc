#include "TestTools.hh"

#include "gameplay/view/View.hh"
#include <memory>

int main()
{
    // Create a camera looking 'down' towards -z
    View v;
    vec3 e = {0, 0, -1};
    vec3 pos = {0, 0, 1}, d = {0, 0, -1}, u = {1, 0, 0};
    v.screenSize[0] = 100;
    v.screenSize[1] = 100;
    auto c = std::make_shared<Camera>();
    v.camera = c;
    v.camera.lock()->setState(pos, d, u, glm_rad(90), v.screenSize[0] / v.screenSize[1]);

    // When mouse is at the center, ray should be the same as camera direction
    // In this case -z
    vec2 coord = {50, 50};
    float *ray = static_cast<float *>(malloc(3 * sizeof(float)));
    castMouseRay(v, coord, ray);
    for (int i = 0; i < 3; i++)
    {
        WANT(ray[i] == e[i]);
    }

    // When mouse is at the top center, ray should have a component in +x direction
    // In this case FOV is 90 so it should be x = sqrt(2) / 2 and z = -sqrt(2) / 2
    coord[1] = 0;
    castMouseRay(v, coord, ray);
    vec3 e2 = {sqrt(2) / 2, 0, 0};
    e2[2] = -e2[0];
    for (int i = 0; i < 3; i++)
    {
        WANT(static_cast<int>(ray[i] * 10000) == static_cast<int>(e2[i] * 10000));
    }

    // When mouse is at the top left corner, ray should also have a component in +y direction
    // In this case all 3 axes should have the same length, i.e. sqrt(3) / 3
    coord[0] = 0;
    castMouseRay(v, coord, ray);
    vec3 e3 = {0, 0, -sqrt(3) / 3};
    e3[0] = e3[1] = -e3[2];
    for (int i = 0; i < 3; i++)
    {
        WANT(static_cast<int>(ray[i] * 10000) == static_cast<int>(e3[i] * 10000));
    }

    // Let's also check the view matrix and projection matrix
    // Since if all above have passed the inv matrix should be OK
    // So we just check it with a simple point and verify that they are inversed
    mat4 p, xv, pd, vd, ivp, ivv;
    v.camera.lock()->getProjectionMatrix(p);
    v.camera.lock()->getProjectionMatrixInv(ivp);
    v.camera.lock()->getViewMatrix(xv);
    v.camera.lock()->getViewMatrixInv(ivv);
    vec4 o = {0, 0, -1, 0};
    vec4 t;
    glm_mat4_mulv(xv, o, t);
    glm_mat4_mulv(p, t, o);
    glm_normalize(o);
    WANT(o[0] == 0 && o[1] == 0);

    glm_mat4_inv(p, pd);
    glm_mat4_inv(xv, vd);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            WANT(pd[i][j] == ivp[i][j] && vd[i][j] == ivv[i][j]);
        }
    }

    // Ура!
    free(ray);
    TEND;
}
