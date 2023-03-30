#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Framework.hh"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <map>

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

static GLuint loadTexture(const std::string &name)
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    texturesCtl[name] = tex;
    return tex;
}

static GLuint vbo, ebo, vao;

void vtGraphicsInit()
{
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindVertexArray(0);
}

void vtGraphicsCleanUp()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void vtDraw(DrawContext &ctx)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 proj, view;
    ctx.cam.getProjectionMatrix(proj);
    ctx.cam.getViewMatrix(view);
    for (auto &p : ctx.polygons)
    {
        switch (p.renderPreset)
        {
        case RECT:
        {
            auto sd = loadShader(p.shader);
            auto tx = loadTexture(p.texture);
            float vertex[20];

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    vertex[5 * i + j] = p.points[i][j];
                }
            }

            // Set texture mapping
            vertex[4] = vertex[13] = vertex[14] = vertex[18] = 1;
            vertex[3] = vertex[8] = vertex[9] = vertex[19] = 0;

            unsigned int indicies[6] = {0, 1, 2, 2, 1, 3};
            glBindVertexArray(vao);
            glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertex, GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glUseProgram(sd);
            glUniformMatrix4fv(glGetUniformLocation(sd, "view"), 1, GL_FALSE, (float *)view);
            glUniformMatrix4fv(glGetUniformLocation(sd, "proj"), 1, GL_FALSE, (float *)proj);
            glUniform1f(glGetUniformLocation(sd, "time"), vtGetTime());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tx);
            glUniform1i(glGetUniformLocation(sd, "baseTex"), 0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            break;
        }
        default:
            break;
        }
        // Draw them
    }
};
int vtGetGraphicsError()
{
    return glGetError();
}