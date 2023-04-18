#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Framework.hh"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <algorithm>
#include <map>

// #define ENABLE_VT_3D_BLENDING

static std::map<std::string, GLuint> texturesCtl;
static std::map<std::string, GLuint> shadersCtl;

static GLuint
loadShader(const std::string &name)
{
    if (shadersCtl.contains(name))
    {
        return shadersCtl[name];
    }
    GLuint vsh, fsh, prog;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream vs("assets/shaders/" + name + ".vsh");
    std::ifstream fs("assets/shaders/" + name + ".fsh");
    std::stringstream vsc, fsc;
    vsc << vs.rdbuf();
    vs.close();
    fsc << fs.rdbuf();
    fs.close();
    std::string vsrc, fsrc;
    vsrc = vsc.str();
    fsrc = fsc.str();
    const char *vscrc = vsrc.c_str();
    const char *fscrc = fsrc.c_str();
    glShaderSource(vsh, 1, &vscrc, NULL);
    glShaderSource(fsh, 1, &fscrc, NULL);
    glCompileShader(vsh);
    glCompileShader(fsh);
    prog = glCreateProgram();
    glAttachShader(prog, vsh);
    glAttachShader(prog, fsh);
    glLinkProgram(prog);
    glDeleteShader(vsh);
    glDeleteShader(fsh);
    shadersCtl[name] = prog;
    return prog;
}

static GLuint loadTexture(const std::string &name, bool enableMipmap)
{
    if (texturesCtl.contains(name))
    {
        return texturesCtl[name];
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    int width, height, nrChannels;
    auto pt = "assets/textures/" + name + ".png";
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(pt.c_str(), &width, &height, &nrChannels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    if (enableMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    if (enableMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    texturesCtl[name] = tex;
    return tex;
}

static GLuint vbo, vao;

void vtGraphicsInit()
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(0);
}

void vtGraphicsCleanUp()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

struct Mesh
{
    vec3 vert[3];
    vec2 texCoord[3];
    vec3 normal;
    RenderPreset preset;
    float distanceToCam;
    unsigned int texture, shader;
    std::map<std::string, float> *valueRef;
};

static bool isAround(float a, float b)
{
    auto diff = std::abs(1 - a / b);
    return diff <= 0.01;
}

static void getAbsCenter(vec3 vert[], vec3 ctr)
{
    vec3 a, b;
    float d10 = glm_vec3_distance2(vert[1], vert[0]);
    float d21 = glm_vec3_distance2(vert[2], vert[1]);
    float d20 = glm_vec3_distance2(vert[2], vert[0]);
    float sum = (d10 + d21 + d20) / 2;
    if (isAround(sum, d10) || isAround(sum, d21) || isAround(sum, d20))
    {
        // Use hypo point
        glm_vec3_sub(vert[1], vert[0], a);
        glm_vec3_sub(vert[2], vert[0], b);
        glm_vec3_normalize(a);
        glm_vec3_normalize(b);
        if (glm_vec3_dot(a, b) < 0.01)
        {
            glm_vec3_add(vert[1], vert[2], ctr);
        }
        else
        {
            if (d10 > d20)
            {
                glm_vec3_add(vert[1], vert[0], ctr);
            }
            else
            {
                glm_vec3_add(vert[2], vert[0], ctr);
            }
        }
        glm_vec3_scale(ctr, 0.5, ctr);
    }
    else
    {
        // Use weight point
        for (int i = 0; i < 3; i++)
        {
            ctr[i] = (vert[0][i] + vert[1][i] + vert[2][i]) / 3.0;
        }
    }
}

static float getMeshDistanceProj(vec3 vert[], vec3 camPos, vec3 camDir)
{
    vec3 ctr;
    getAbsCenter(vert, ctr);
    vec3 a, x;
    glm_vec3_sub(ctr, camPos, a);
    glm_vec3_copy(camDir, x);
    glm_vec3_normalize(x);
    return glm_vec3_dot(a, x);
}

static void pickPoints(Mesh &ms, const PolygonShape &p, const std::vector<unsigned int> &ptOrder)
{
    for (int i = 0; i < 3; i++)
    {
        unsigned curInd = ptOrder[i];
        for (int j = 0; j < 3; j++)
        {
            ms.vert[i][j] = p.points[curInd][j];
        }
    }
    vec3 a, b;
    glm_vec3_sub(ms.vert[2], ms.vert[1], a);
    glm_vec3_sub(ms.vert[0], ms.vert[1], b);
    glm_vec3_cross(a, b, ms.normal);
    glm_vec3_normalize(ms.normal);
}

static void pickTex(Mesh &ms, const std::vector<std::pair<float, float>> &st)
{
    ms.texCoord[0][0] = st[0].first;
    ms.texCoord[0][1] = st[0].second;
    ms.texCoord[1][0] = st[1].first;
    ms.texCoord[1][1] = st[1].second;
    ms.texCoord[2][0] = st[2].first;
    ms.texCoord[2][1] = st[2].second;
}

static void processMeshes(PolygonShape &p, vec3 camPos, vec3 camDir, std::vector<Mesh> &meshes)
{
    auto sd = loadShader(p.shader);
    auto tx = loadTexture(p.texture, p.renderPreset != OCT);
    unsigned int stx = 0;
    if (p.subTexture.size() > 0)
    {
        stx = loadTexture(p.subTexture, false);
    }
    switch (p.renderPreset)
    {
    case RECT:
    {
        Mesh ms[2];

        // Attach points
        pickPoints(ms[0], p, {0, 1, 2});
        pickPoints(ms[1], p, {2, 1, 3});
        pickTex(ms[0], {{0, 1}, {0, 0}, {1, 1}});
        pickTex(ms[1], {{1, 1}, {0, 0}, {1, 0}});

        // Sort meshes
        ms[0].shader = ms[1].shader = sd;
        ms[0].texture = ms[1].texture = tx;
        ms[0].distanceToCam = getMeshDistanceProj(ms[0].vert, camPos, camDir);
        ms[1].distanceToCam = getMeshDistanceProj(ms[1].vert, camPos, camDir);
        ms[0].preset = ms[1].preset = RECT;
        ms[0].valueRef = ms[1].valueRef = &p.values;
        meshes.push_back(ms[0]);
        meshes.push_back(ms[1]);

        break;
    }
    case OCT:
    {
        Mesh ms[8];
        pickPoints(ms[0], p, {0, 5, 2});
        pickPoints(ms[1], p, {0, 3, 5});
        pickPoints(ms[2], p, {2, 5, 1});
        pickPoints(ms[3], p, {5, 3, 1});
        pickPoints(ms[4], p, {0, 4, 3});
        pickPoints(ms[5], p, {0, 2, 4});
        pickPoints(ms[6], p, {4, 2, 1});
        pickPoints(ms[7], p, {3, 4, 1});

        pickTex(ms[0], {{0, 1}, {0, 0}, {1, 0}});
        pickTex(ms[1], {{0, 1}, {1, 0}, {0, 0}});
        pickTex(ms[2], {{1, 0}, {0, 0}, {0, 1}});
        pickTex(ms[3], {{0, 0}, {1, 0}, {0, 1}});
        pickTex(ms[4], {{0, 1}, {0, 0}, {1, 0}});
        pickTex(ms[5], {{0, 1}, {0, 0}, {1, 0}});
        pickTex(ms[6], {{0, 0}, {1, 0}, {0, 1}});
        pickTex(ms[7], {{1, 0}, {0, 0}, {0, 1}});

        for (int i = 0; i < 8; i++)
        {
            ms[i].shader = sd;
            ms[i].texture = tx;
            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            ms[i].preset = OCT;
            ms[i].valueRef = &p.values;
            meshes.push_back(ms[i]);
        }

        break;
    }
    // Full prism draw can only be used for short ones
    case PRISM_FULL:
    {
        // The prism is a little bit trickey, we must use the center of each square, rather than triangle
        Mesh ms[20];
        std::vector<std::vector<unsigned int>> points = {
            {1, 0, 5}, {2, 1, 5}, {2, 5, 4}, {2, 4, 3}, {11, 6, 7}, {11, 7, 8}, {10, 11, 8}, {9, 10, 8}, {10, 5, 11}, {10, 4, 5}, {11, 5, 0}, {11, 0, 6}, {6, 0, 7}, {7, 0, 1}, {8, 7, 1}, {8, 1, 2}, {9, 8, 2}, {9, 2, 3}, {10, 9, 3}, {10, 3, 4}};
        for (int i = 0; i < 20; i++)
        {
            pickPoints(ms[i], p, points[i]);
        }
        std::vector<std::vector<std::pair<float, float>>> texCoords = {
            // Bottom 4 triangles
            {{0.25, 0.933}, {0, 0.5}, {0.25, 0.067}},
            {{0.75, 0.933}, {0.25, 0.933}, {0.25, 0.067}},
            {{0.75, 0.933}, {0.25, 0.067}, {0.75, 0.067}},
            {{0.75, 0.933}, {0.75, 0.067}, {1, 0.5}},

            // Top 4 triangles
            {{0.75, 0.067}, {1, 0.5}, {0.75, 0.933}},
            {{0.75, 0.067}, {0.75, 0.933}, {0.25, 0.933}},
            {{0.25, 0.067}, {0.75, 0.067}, {0.25, 0.933}},
            {{0, 0.5}, {0.25, 0.067}, {0.25, 0.933}},

            // Body
            {{0, 1}, {1, 0}, {1, 1}},
            {{0, 1}, {0, 0}, {1, 0}},

            {{0, 1}, {0, 0}, {1, 0}},
            {{0, 1}, {1, 0}, {1, 1}},

            {{0, 1}, {0, 0}, {1, 1}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

        };
        for (int i = 0; i < 20; i++)
        {
            pickTex(ms[i], texCoords[i]);
        }
        for (int i = 0; i < 20; i++)
        {
            ms[i].shader = sd;
            if (i < 8)
            {
                ms[i].texture = tx;
            }
            else
            {
                ms[i].texture = stx;
            }

            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            ms[i].preset = PRISM_FULL;
            ms[i].valueRef = &p.values;
            meshes.push_back(ms[i]);
        }
        break;
    }

    case PRISM_BTM:
    {
        // The prism is a little bit trickey, we must use the center of each square, rather than triangle
        Mesh ms[16];
        std::vector<std::vector<unsigned int>> points = {
            {1, 0, 5}, {2, 1, 5}, {2, 5, 4}, {2, 4, 3}, {10, 5, 11}, {10, 4, 5}, {11, 5, 0}, {11, 0, 6}, {6, 0, 7}, {7, 0, 1}, {8, 7, 1}, {8, 1, 2}, {9, 8, 2}, {9, 2, 3}, {10, 9, 3}, {10, 3, 4}};
        for (int i = 0; i < 16; i++)
        {
            pickPoints(ms[i], p, points[i]);
        }
        std::vector<std::vector<std::pair<float, float>>> texCoords = {
            // Bottom 4 triangles
            {{0.25, 0.933}, {0, 0.5}, {0.25, 0.067}},
            {{0.75, 0.933}, {0.25, 0.933}, {0.25, 0.067}},
            {{0.75, 0.933}, {0.25, 0.067}, {0.75, 0.067}},
            {{0.75, 0.933}, {0.75, 0.067}, {1, 0.5}},

            // Body
            {{0, 1}, {1, 0}, {1, 1}},
            {{0, 1}, {0, 0}, {1, 0}},

            {{0, 1}, {0, 0}, {1, 0}},
            {{0, 1}, {1, 0}, {1, 1}},

            {{0, 1}, {0, 0}, {1, 1}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

        };
        for (int i = 0; i < 16; i++)
        {
            pickTex(ms[i], texCoords[i]);
        }
        for (int i = 0; i < 16; i++)
        {
            ms[i].shader = sd;
            if (i < 4)
            {
                ms[i].texture = tx;
            }
            else
            {
                ms[i].texture = stx;
            }

            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            ms[i].preset = PRISM_BTM;
            ms[i].valueRef = &p.values;
            meshes.push_back(ms[i]);
        }
        break;
    }
    case PRISM_HAT:
    {
        // The prism is a little bit trickey, we must use the center of each square, rather than triangle
        Mesh ms[16];
        std::vector<std::vector<unsigned int>> points = {
            {11, 6, 7}, {11, 7, 8}, {10, 11, 8}, {9, 10, 8}, {10, 5, 11}, {10, 4, 5}, {11, 5, 0}, {11, 0, 6}, {6, 0, 7}, {7, 0, 1}, {8, 7, 1}, {8, 1, 2}, {9, 8, 2}, {9, 2, 3}, {10, 9, 3}, {10, 3, 4}};
        for (int i = 0; i < 16; i++)
        {
            pickPoints(ms[i], p, points[i]);
        }
        std::vector<std::vector<std::pair<float, float>>> texCoords = {
            // Top 4 triangles
            {{0.75, 0.067}, {1, 0.5}, {0.75, 0.933}},
            {{0.75, 0.067}, {0.75, 0.933}, {0.25, 0.933}},
            {{0.25, 0.067}, {0.75, 0.067}, {0.25, 0.933}},
            {{0, 0.5}, {0.25, 0.067}, {0.25, 0.933}},

            // Body
            {{0, 1}, {1, 0}, {1, 1}},
            {{0, 1}, {0, 0}, {1, 0}},

            {{0, 1}, {0, 0}, {1, 0}},
            {{0, 1}, {1, 0}, {1, 1}},

            {{0, 1}, {0, 0}, {1, 1}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

        };
        for (int i = 0; i < 16; i++)
        {
            pickTex(ms[i], texCoords[i]);
        }
        for (int i = 0; i < 16; i++)
        {
            ms[i].shader = sd;
            if (i < 4)
            {
                ms[i].texture = tx;
            }
            else
            {
                ms[i].texture = stx;
            }

            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            ms[i].preset = PRISM_HAT;
            ms[i].valueRef = &p.values;
            meshes.push_back(ms[i]);
        }
        break;
    }
    case PRISM_SIDE:
    {
        // The prism is a little bit trickey, we must use the center of each square, rather than triangle
        Mesh ms[12];
        std::vector<std::vector<unsigned int>> points = {
            {10, 5, 11}, {10, 4, 5}, {11, 5, 0}, {11, 0, 6}, {6, 0, 7}, {7, 0, 1}, {8, 7, 1}, {8, 1, 2}, {9, 8, 2}, {9, 2, 3}, {10, 9, 3}, {10, 3, 4}};
        for (int i = 0; i < 12; i++)
        {
            pickPoints(ms[i], p, points[i]);
        }
        std::vector<std::vector<std::pair<float, float>>> texCoords = {

            // Body
            {{0, 1}, {1, 0}, {1, 1}},
            {{0, 1}, {0, 0}, {1, 0}},

            {{0, 1}, {0, 0}, {1, 0}},
            {{0, 1}, {1, 0}, {1, 1}},

            {{0, 1}, {0, 0}, {1, 1}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

            {{1, 1}, {0, 1}, {0, 0}},
            {{1, 1}, {0, 0}, {1, 0}},

        };
        for (int i = 0; i < 12; i++)
        {
            pickTex(ms[i], texCoords[i]);
        }
        for (int i = 0; i < 12; i++)
        {
            ms[i].shader = sd;
            ms[i].texture = stx;

            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            ms[i].preset = PRISM_HAT;
            ms[i].valueRef = &p.values;
            meshes.push_back(ms[i]);
        }
        break;
    }
    default:
        break;
    }
}

static std::vector<Mesh> makeMeshes(DrawContext &ctx)
{
    std::vector<Mesh> meshOpaque, meshTrans;
    vec3 camPos, camDir;
    ctx.cam.getPosition(camPos);
    ctx.cam.getDir(camDir);

    for (auto &p : ctx.polygons)
    {
        processMeshes(p, camPos, camDir, p.isOpaque ? meshOpaque : meshTrans);
    }

#ifdef ENABLE_VT_3D_BLENDING
    std::sort(meshTrans.begin(), meshTrans.end(), [](const Mesh &m1, const Mesh &m2) -> int
              { return m1.distanceToCam > m2.distanceToCam; });
#endif
    for (auto &t : meshTrans)
    {
        meshOpaque.push_back(t);
    }
    return meshOpaque;
}

static void completeDraw(std::vector<Mesh> &meshes, DrawContext &ctx)
{
    mat4 proj, view;
    vec3 dir;
    ctx.cam.getDir(dir);
    ctx.cam.getProjectionMatrix(proj);
    ctx.cam.getViewMatrix(view);
    for (auto &m : meshes)
    {
        float vertex[15] = {0};
        for (int i = 0; i < 3; i++)
        {
            int j = 0;
            for (; j < 3; j++)
            {
                vertex[5 * i + j] = m.vert[i][j];
            }
            for (; j < 5; j++)
            {
                vertex[5 * i + j] = m.texCoord[i][j - 3];
            }
        }
        glBindVertexArray(vao);
        glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), vertex, GL_STREAM_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glUseProgram(m.shader);
        glUniformMatrix4fv(glGetUniformLocation(m.shader, "view"), 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(glGetUniformLocation(m.shader, "proj"), 1, GL_FALSE, (float *)proj);
        glUniform1f(glGetUniformLocation(m.shader, "time"), vtGetTime());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m.texture);
        glUniform1i(glGetUniformLocation(m.shader, "baseTex"), 0);

        // Assign extra values
        for (auto &v : (*m.valueRef))
        {
            glUniform1f(glGetUniformLocation(m.shader, v.first.c_str()), v.second);
        }

        if (m.preset == OCT || m.preset == PRISM_FULL || m.preset == PRISM_BTM || m.preset == PRISM_HAT || m.preset == PRISM_SIDE)
        {
            glUniform3f(glGetUniformLocation(m.shader, "aNormal"), m.normal[0], m.normal[1], m.normal[2]);
            glUniform3f(glGetUniformLocation(m.shader, "camDir"), dir[0], dir[1], dir[2]);
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
}

void vtDraw(DrawContext &ctx)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto meshes = makeMeshes(ctx);
    completeDraw(meshes, ctx);
};
int vtGetGraphicsError()
{
    return glGetError();
}