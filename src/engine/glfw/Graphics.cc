#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Framework.hh"
#include "engine/virtual/Window.hh"
#include "gameplay/view/Camera.hh"
#include "support/Resource.hh"
#include "util/Util.hh"
#include <climits>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include "spdlog/spdlog.h"
#include <ft2build.h>
#include <set>
#include FT_FREETYPE_H

using namespace spdlog;

#define VT_ORTHO_NZ 0.1
#define VT_ORTHO_FZ 2.0

#define VT_SHADER_DELM "// ---"

// #define ENABLE_MESH_SORTING
#ifdef ENABLE_MESH_SORTING
#warning "The blending system has known flaws and vulnerabilities. It will also reduce FPS significantly."
#endif

static std::unordered_map<std::string, GLuint> texturesCtl;
static std::unordered_map<std::string, GLuint> shadersCtl;

struct Glyph
{
    unsigned int texID;
    vec2 size;
    vec2 bearing;
    unsigned int advance;
};

static std::vector<std::string> faces = {"en.ttf", "zh.otf", "jp.ttf"};
static std::vector<FT_Face> alterFaces;
static std::unordered_map<wchar_t, Glyph> glyphBuf;
static std::set<wchar_t> missingChar;
static FT_Library ftlib;

static unsigned int bgTex = 0;

/**
 * @brief Loads the glyph of specified character.
 *
 * This function will try to load the character in a pre-defined font faces chain. If one
 * failed it will try the next one. If none is available, it will quit. Otherwise, a glyph is
 * inserted into the glyph buffer.
 *
 * @param c The character to load glyph for.
 * @return Whether the glyph is successfully loaded.
 */
static bool loadCharGlyph(wchar_t c)
{
    if (missingChar.contains(c))
    {
        return false;
    }
    if (glyphBuf.contains(c))
    {
        return true;
    }
    else
    {
        // Generate a new glyph
        bool loaded = false;
        for (auto &f : alterFaces)
        {
            if (FT_Load_Glyph(f, FT_Get_Char_Index(f, c), FT_LOAD_RENDER) || f->glyph->glyph_index == 0)
            {
                // Try next
                continue;
            }
            loaded = true;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            unsigned int tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                f->glyph->bitmap.width,
                f->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                f->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Glyph gp;
            gp.texID = tex;
            gp.size[0] = f->glyph->bitmap.width;
            gp.size[1] = f->glyph->bitmap.rows;
            gp.bearing[0] = f->glyph->bitmap_left;
            gp.bearing[1] = f->glyph->bitmap_top;
            gp.advance = f->glyph->advance.x;
            glyphBuf[c] = gp;
            return true;
        }
        if (!loaded)
        {
            warn("Missing font for char unicode " + std::to_string((long)c) + ". Check font files.");
            missingChar.insert(c);
        }
    }
    return false;
}

static void loadFont()
{
    if (FT_Init_FreeType(&ftlib))
    {
        error("Could not initialize FreeType library. Is it missing, or not compatible with current platform?");
        return;
    }
    for (auto &f : faces)
    {
        FT_Face face;
        if (FT_New_Face(ftlib, getAppResource("fonts/" + f).c_str(), 0, &face))
        {
            error("Could not load font file '" + f + "'. Is this font file corrupted, or not supported by FreeType?");
            return;
        }
        info("Loaded font file '" + f + "'");
        FT_Set_Pixel_Sizes(face, 0, 96);
        alterFaces.push_back(face);
    }
}

static void cleanFont()
{
    for (auto &p : glyphBuf)
    {
        glDeleteTextures(1, &p.second.texID);
    }
    for (auto &f : alterFaces)
    {
        FT_Done_Face(f);
    }
    FT_Done_FreeType(ftlib);
}

static GLuint
loadShader(const std::string &name)
{
    if (shadersCtl.contains(name))
    {
        return shadersCtl[name];
    }
    info("Compiling new shader '" + name + "'");
    GLuint vsh, fsh, prog;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream cbf(getAppResource("shaders/" + name));
    if (cbf.fail())
    {
        error("Could not find shader files. Loading shader '" + name + "'");
        return INT_MAX;
    }

    std::stringstream ssrc;
    ssrc << cbf.rdbuf();
    auto cbsrc = splitStr(ssrc.str(), VT_SHADER_DELM, 2);
    if (cbsrc.size() < 2)
    {
        warn("Missing either head or body of shader '" + name + "', compilation might fail.");
    }
    std::string vsrc, fsrc;
    vsrc = cbsrc[0];
    fsrc = cbsrc[1];
    const char *vscrc = vsrc.c_str();
    const char *fscrc = fsrc.c_str();
    glShaderSource(vsh, 1, &vscrc, NULL);
    glShaderSource(fsh, 1, &fscrc, NULL);
    glCompileShader(vsh);
    glCompileShader(fsh);
    int success;
    char infoLog[1024];
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vsh, 1024, NULL, infoLog);
        error("Shader compilation error detected. In VERT '" + name + "':");
        error(infoLog);
        return INT_MAX;
    };
    glGetShaderiv(fsh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fsh, 1024, NULL, infoLog);
        error("Shader compilation error detected. In FRAG '" + name + "':");
        error(infoLog);
        return INT_MAX;
    };

    prog = glCreateProgram();
    glAttachShader(prog, vsh);
    glAttachShader(prog, fsh);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(prog, 1024, NULL, infoLog);
        error("Shader program linkage error detected. In '" + name + "':");
        error(infoLog);
        return INT_MAX;
    };
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
    info("Loading new texture '" + name + "'");
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        error("Could not load texture '" + name + "'. Is this file missing, or is stb_image corrupted?");
        return INT_MAX;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    if (enableMipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (nrChannels == 3)
    {
        // Sample as vec4 (RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    if (enableMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    texturesCtl[name] = tex;
    return tex;
}

static GLuint vbo, vao, textVBO, textVAO, bgVBO, bgVAO;

float bgDrawVert[6][4] = {
    {-1, 1, 0, 1}, {-1, -1, 0, 0}, {1, 1, 1, 1}, {1, 1, 1, 1}, {-1, -1, 0, 0}, {1, -1, 1, 0}};

void vtGraphicsInit()
{
    info("Initializing graphics buffers.");
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Initializing text buffers.");

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Initializing background buffers.");

    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);

    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, bgDrawVert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Loading fonts.");
    loadFont();
}

void vtGraphicsCleanUp()
{
    info("Deleting graphics buffers.");
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    info("Cleaning up shader and texture cache.");
    for (auto &p : shadersCtl)
    {
        glDeleteProgram(p.second);
    }
    for (auto &t : texturesCtl)
    {
        glDeleteTextures(1, &t.second);
    }
    shadersCtl.clear();
    texturesCtl.clear();
    info("Cleaning up fonts.");
    glDeleteBuffers(1, &textVBO);
    glDeleteVertexArrays(1, &textVAO);
    cleanFont();
}

struct Mesh
{
    vec3 vert[3];
    vec2 texCoord[3];
    vec3 normal;
    bool orthoProj = false;
    float distanceToCam;
    unsigned int texture, shader;
    float args[VT_SD_ARGS];
};

#ifdef ENABLE_MESH_SORTING

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
#endif

static float getMeshDistanceProj(vec3 vert[], vec3 camPos, vec3 camDir)
{
#ifdef ENABLE_MESH_SORTING
    vec3 ctr;
    getAbsCenter(vert, ctr);
    vec3 a, x;
    glm_vec3_sub(ctr, camPos, a);
    glm_vec3_copy(camDir, x);
    glm_vec3_normalize(x);
    return glm_vec3_dot(a, x);
#else
    return 0;
#endif
}

// UI projection matrix
static mat4 uiProj;

void vtSetBufferSize(int w, int h)
{
    glm_ortho(0, w, 0, h, VT_ORTHO_NZ, VT_ORTHO_FZ, uiProj);
}

void vtSetBackground(const std::string &img)
{
    if (img.size() == 0)
    {
        bgTex = 0;
        return;
    }
    // Update texture
    bgTex = loadTexture(img, false);
}

static void drawBg()
{
    if (bgTex == 0)
    {
        return;
    }

    auto sd = loadShader("bg");
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBindVertexArray(bgVAO);
    glUseProgram(sd);
    glActiveTexture(GL_TEXTURE2); // Texture 2 for bg
    glBindTexture(GL_TEXTURE_2D, bgTex);
    glUniform1i(glGetUniformLocation(sd, "img"), 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

static void drawTypography(Typography &t)
{
    auto sd = loadShader("text");
    glUseProgram(sd);
    glUniform3f(glGetUniformLocation(sd, "tColor"), t.color[0], t.color[1], t.color[2]);
    glActiveTexture(GL_TEXTURE1); // Use another texture
    glUniform1i(glGetUniformLocation(sd, "baseTex"), 1);
    glUniformMatrix4fv(glGetUniformLocation(sd, "proj"), 1, GL_FALSE, (float *)uiProj);
    glBindVertexArray(textVAO);

    // Coord Correction
    int x, y;
    vtGetCoord(t.pos[0], t.pos[1], x, y);

    float xoffset = 0, yoffset = 0;
    auto sz = t.size * vtGetScaleFactor(); // Scale text
    std::list<Glyph *> useGlyphs;
    // Calc offset
    for (auto c : t.text)
    {
        if (!loadCharGlyph(c))
        {
            continue;
        }
        auto &g = glyphBuf[c];
        useGlyphs.push_back(&g);

        // Calculate bounding box
        float h = g.size[1] * sz;
        xoffset += (g.advance >> 6) * sz;
        yoffset = h > yoffset ? h : yoffset;
    }

    if (t.xAlign == RIGHT)
    {
        x -= xoffset;
    }
    else if (t.xAlign == CENTER)
    {
        x -= xoffset / 2.0;
    }

    if (t.yAlign == RIGHT)
    {
        y -= yoffset;
    }
    else if (t.yAlign == CENTER)
    {
        y -= yoffset / 2.0;
    }
    for (auto gp : useGlyphs)
    {
        Glyph &g = *gp;
        float w = g.size[0] * sz;
        float h = g.size[1] * sz;
        xoffset += (g.advance >> 6) * sz;
        yoffset = h > yoffset ? h : yoffset;
        float xpos = x + g.bearing[0] * sz;
        float ypos = y - (g.size[1] - g.bearing[1]) * sz;
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        glBindTexture(GL_TEXTURE_2D, g.texID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (g.advance >> 6) * sz;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

static void pickPoints(Mesh &ms, const Polygon &p, const std::vector<unsigned int> &ptOrder)
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

// 2D version of 'pickPoints'. Comparing to it, this one does not calculate normal, which makes it faster
static void pickShapePoints(Mesh &ms, const Shape &p, const std::vector<unsigned int> &ptOrder)
{
    for (int i = 0; i < 3; i++)
    {
        unsigned curInd = ptOrder[i];

        // Apply coord correction
        int rx, ry;
        vtGetCoord(p.points[curInd][0], p.points[curInd][1], rx, ry);
        ms.vert[i][0] = rx;
        ms.vert[i][1] = ry;
        ms.vert[i][2] = 0;
    }
    vec3 cNormal = {0, 0, 1};
    glm_vec3_copy(cNormal, ms.normal);
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

// Prism indicies data
static std::vector<std::vector<std::pair<float, float>>> PRISM_TEX_COORDS = {
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

static std::vector<std::vector<unsigned int>> PRISM_VERTEX = {
    {1, 0, 5}, {2, 1, 5}, {2, 5, 4}, {2, 4, 3}, {11, 6, 7}, {11, 7, 8}, {10, 11, 8}, {9, 10, 8}, {10, 5, 11}, {10, 4, 5}, {11, 5, 0}, {11, 0, 6}, {6, 0, 7}, {7, 0, 1}, {8, 7, 1}, {8, 1, 2}, {9, 8, 2}, {9, 2, 3}, {10, 9, 3}, {10, 3, 4}};

static inline void copyArgs(const float *si, float *di)
{
    for (int i = 0; i < VT_SD_ARGS; ++i)
    {
        di[i] = si[i];
    }
}

static void procPolygonMesh(Polygon &p, vec3 camPos, vec3 camDir, std::vector<Mesh> &meshes)
{
    auto sd = loadShader(p.shader);
    unsigned int tx = 0;
    if (p.texture.size() > 0)
    {
        tx = loadTexture(getAppResource("textures/" + p.texture + ".png"), true);
    }
    if (sd == INT_MAX || tx == INT_MAX)
    {
        // Problem loading, skip render
        warn("Skipped mesh process due to previous errors. Check log output for details.");
        return;
    }
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

        for (int i : {0, 1})
        {
            ms[i].shader = sd;
            ms[i].texture = tx;
            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }

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
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }

        break;
    }
    // Full prism draw can only be used for short ones
    case PRISM_FULL:
    {
        // The prism is a little bit tricky, we must use the center of each square, rather than triangle
        Mesh ms[20];
        std::vector<std::vector<unsigned int>> points = PRISM_VERTEX;
        std::vector<std::vector<std::pair<float, float>>> texCoords = PRISM_TEX_COORDS;
        for (int i = 0; i < 20; i++)
        {
            pickPoints(ms[i], p, points[i]);
            pickTex(ms[i], texCoords[i]);
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
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }
        break;
    }

    case PRISM_BTM:
    {
        Mesh ms[16];
        std::vector<std::vector<unsigned int>> points = PRISM_VERTEX;
        std::vector<std::vector<std::pair<float, float>>> texCoords = PRISM_TEX_COORDS;
        for (int i = 0; i < 4; i++)
        {
            points.erase(points.begin() + 4); // Remove elements at index 4
            texCoords.erase(texCoords.begin() + 4);
        }
        for (int i = 0; i < 16; i++)
        {
            pickPoints(ms[i], p, points[i]);
            pickTex(ms[i], texCoords[i]);
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
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }
        break;
    }
    case PRISM_HAT:
    {
        Mesh ms[16];
        std::vector<std::vector<unsigned int>> points = PRISM_VERTEX;
        std::vector<std::vector<std::pair<float, float>>> texCoords = PRISM_TEX_COORDS;
        for (int i = 0; i < 4; i++)
        {
            points.erase(points.begin());
            texCoords.erase(texCoords.begin());
        }
        for (int i = 0; i < 16; i++)
        {
            pickPoints(ms[i], p, points[i]);
            pickTex(ms[i], texCoords[i]);
            ms[i].shader = sd;
            if (i < 4)
            {
                // Attach btm and hat
                ms[i].texture = tx;
            }
            else
            {
                // Attach side texture
                ms[i].texture = stx;
            }

            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }
        break;
    }
    case PRISM_SIDE:
    {
        Mesh ms[12];
        std::vector<std::vector<unsigned int>> points = PRISM_VERTEX;
        std::vector<std::vector<std::pair<float, float>>> texCoords = PRISM_TEX_COORDS;
        for (int i = 0; i < 8; i++)
        {
            points.erase(points.begin());
            texCoords.erase(texCoords.begin());
        }

        for (int i = 0; i < 12; i++)
        {
            pickPoints(ms[i], p, points[i]);
            pickTex(ms[i], texCoords[i]);
            ms[i].shader = sd;
            ms[i].texture = stx;
            ms[i].distanceToCam = getMeshDistanceProj(ms[i].vert, camPos, camDir);
            copyArgs(p.args, ms[i].args);
            meshes.push_back(ms[i]);
        }
        break;
    }
    default:
        warn("Unknown mesh preset: " + std::to_string(p.renderPreset) + ", skipped.");
        break;
    }
}

static void procShapeMesh(Shape &s, mat4 p, std::vector<Mesh> &meshes)
{
    auto sd = loadShader(s.shader);
    auto tx = 0;
    if (s.texture.size() > 0)
    {
        tx = loadTexture(getAppResource("textures/" + s.texture + ".png"), false);
    }
    if (sd == INT_MAX || tx == INT_MAX)
    {
        warn("Skipped mesh process due to previous errors. Check log output for details.");
        return;
    }
    Mesh ms[2];
    pickShapePoints(ms[0], s, {0, 1, 2});
    pickShapePoints(ms[1], s, {2, 1, 3});
    pickTex(ms[0], {{0, 1}, {0, 0}, {1, 1}});
    pickTex(ms[1], {{1, 1}, {0, 0}, {1, 0}});

    for (int i : {0, 1})
    {
        ms[i].shader = sd;
        ms[i].texture = tx;
        ms[i].distanceToCam = 0;
        ms[i].orthoProj = true;
        copyArgs(s.args, ms[i].args);
        meshes.push_back(ms[i]);
    }
}

static std::pair<std::vector<Mesh>, std::vector<Mesh>> makeMeshes(DrawContext &ctx)
{
    std::vector<Mesh> meshOpaque, meshTrans;
    vec3 camPos, camDir;
    auto gcam = ctx.cam.lock();
    gcam->getPosition(camPos);
    gcam->getDir(camDir);

    for (auto &p : ctx.polygons)
    {
        // Make sure transparent objects draw last
        procPolygonMesh(p, camPos, camDir, p.isOpaque ? meshOpaque : meshTrans);
    }

#ifdef ENABLE_MESH_SORTING
    std::sort(meshTrans.begin(), meshTrans.end(), [](const Mesh &m1, const Mesh &m2) -> int
              { return m1.distanceToCam > m2.distanceToCam; });
#endif
    // Insert ui shapes after sorting
    for (auto &s : ctx.shapes)
    {
        procShapeMesh(s, uiProj, meshTrans);
    }
    return std::pair(meshOpaque, meshTrans);
}

static void completeDraw(std::vector<Mesh> &meshes, DrawContext &ctx)
{
    mat4 proj, view;
    vec3 dir;
    auto gcam = ctx.cam.lock();
    gcam->getDir(dir);
    gcam->getProjectionMatrix(proj);
    gcam->getViewMatrix(view);
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

        glUseProgram(m.shader);
        if (m.orthoProj)
        {
            // Ortho projection
            glUniformMatrix4fv(glGetUniformLocation(m.shader, "proj"), 1, GL_FALSE, (float *)uiProj);
        }
        else
        {
            // Perspective projection
            glUniformMatrix4fv(glGetUniformLocation(m.shader, "view"), 1, GL_FALSE, (float *)view);
            glUniformMatrix4fv(glGetUniformLocation(m.shader, "proj"), 1, GL_FALSE, (float *)proj);
        }
        glUniform1f(glGetUniformLocation(m.shader, "time"), vtGetTime());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m.texture);
        glUniform1i(glGetUniformLocation(m.shader, "baseTex"), 0);

        // Assign extra values
        glUniform1fv(glGetUniformLocation(m.shader, "args"), VT_SD_ARGS, m.args);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
}

static std::list<std::vector<Mesh>> DRAW_BUFFER_OPAQUE, DRAW_BUFFER_ALPHA;

void vtProcessMeshes(DrawContext &ctx)
{
    glGetError(); // Dispose previous
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBg();
    if (ctx.cam.expired())
    {
        warn("Camera not set in this draw context. Skipped draw request.");
        return;
    }
    auto meshes = makeMeshes(ctx);
    DRAW_BUFFER_OPAQUE.push_back(meshes.first);
    DRAW_BUFFER_ALPHA.push_back(meshes.second);
};

void vtCompleteDraw(DrawContext &ctx)
{

    // Meshes
    for (auto &p : DRAW_BUFFER_OPAQUE)
    {
        completeDraw(p, ctx);
    }
    for (auto &p : DRAW_BUFFER_ALPHA)
    {
        completeDraw(p, ctx);
    }
    // Typography
    for (auto &t : ctx.typos)
    {
        drawTypography(t);
    }
    DRAW_BUFFER_OPAQUE.clear();
    DRAW_BUFFER_ALPHA.clear();

    auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        warn("GL error detected: " + std::to_string(err) + ". Check draw instructions.");
    }
}

int vtGetGraphicsError()
{
    return glGetError();
}
