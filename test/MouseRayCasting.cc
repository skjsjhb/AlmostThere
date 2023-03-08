#include "TestTools.hh"

#include "gameplay/objs/World.hh"

int main()
{
    // Create a camera looking 'down' towards -z
    World wd;
    vec3 e = {0, 0, -1};
    vec3 pos = {0, 0, 1}, d = {0, 0, -1}, u = {1, 0, 0};
    wd.szScrn[0] = 100;
    wd.szScrn[1] = 100;
    wd.camera.setState(pos, d, u, glm_rad(90), wd.szScrn[0] / wd.szScrn[1]);

    // When mouse is at the center, ray should be the same as camera direction
    // In this case -z
    vec2 coord = {50, 50};
    float *ray = (float *)malloc(3 * sizeof(float));
    wd.castMouseRay(coord, ray);
    for (int i = 0; i < 3; i++)
    {
        WANT(ray[i] == e[i]);
    }

    // When mouse is at the top center, ray should have a component in +x direction
    // In this case FOV is 90 so it should be x = sqrt(2) / 2 and z = -sqrt(2) / 2
    coord[1] = 0;
    wd.castMouseRay(coord, ray);
    vec3 e2 = {sqrt(2) / 2, 0, 0};
    e2[2] = -e2[0];
    for (int i = 0; i < 3; i++)
    {
        WANT((int)(ray[i] * 10000) == (int)(e2[i] * 10000));
    }

    // When mouse is at the top left corner, ray should also have a component in +y direction
    // In this case all 3 axes should have the same length, i.e. sqrt(3) / 3
    coord[0] = 0;
    wd.castMouseRay(coord, ray);
    vec3 e3 = {0, 0, -sqrt(3) / 3};
    e3[0] = e3[1] = -e3[2];
    for (int i = 0; i < 3; i++)
    {
        WANT((int)(ray[i] * 10000) == (int)(e3[i] * 10000));
    }

    // Let's also check the view matrix and projection matrix
    // Since if all above have passed the inv matrix should be OK
    // So we just check it with a simple point and verify that they are inversed
    mat4 p, v, pd, vd, ivp, ivv;
    wd.camera.getProjectionMatrix(p);
    wd.camera.getProjectionMatrixInv(ivp);
    wd.camera.getViewMatrix(v);
    wd.camera.getViewMatrixInv(ivv);
    vec4 o = {0, 0, -1, 0};
    vec4 t;
    glm_mat4_mulv(v, o, t);
    glm_mat4_mulv(p, t, o);
    glm_normalize(o);
    WANT(o[0] == 0 && o[1] == 0);

    glm_mat4_inv(p, pd);
    glm_mat4_inv(v, vd);
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
