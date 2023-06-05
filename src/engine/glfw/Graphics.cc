#include "engine/virtual/Graphics.hh"
#include "support/Resource.hh"
#include "util/Util.hh"
#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include <ft2build.h>
#include <set>
#include <glm/gtc/type_ptr.hpp>
#include FT_FREETYPE_H

using namespace spdlog;

#define VT_SHADER_DELM "// ---"

#define SHADER_VAR_VIEW "aView"
#define SHADER_VAR_PROJ "aProj"
#define SHADER_VAR_TEX "aTex"
#define SHADER_VAR_COLOR "aColor"

static std::unordered_map<std::string, GLuint> texturesCtl, shadersCtl;

struct Glyph {
    unsigned int texID;
    glm::vec2 size;
    glm::vec2 bearing;
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
static bool loadCharGlyph(wchar_t c) {
    if (missingChar.contains(c)) {
        return false;
    }
    if (glyphBuf.contains(c)) {
        return true;
    } else {
        // Generate a new glyph
        for (auto &f: alterFaces) {
            if (FT_Load_Glyph(f, FT_Get_Char_Index(f, c), FT_LOAD_RENDER) || f->glyph->glyph_index == 0) {
                // Try next
                continue;
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            unsigned int tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    int(f->glyph->bitmap.width),
                    int(f->glyph->bitmap.rows),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    f->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Glyph gp{};
            gp.texID = tex;
            gp.size[0] = float(f->glyph->bitmap.width);
            gp.size[1] = float(f->glyph->bitmap.rows);
            gp.bearing[0] = float(f->glyph->bitmap_left);
            gp.bearing[1] = float(f->glyph->bitmap_top);
            gp.advance = f->glyph->advance.x;
            glyphBuf[c] = gp;
            return true;
        }
        warn("Missing font for char unicode " + std::to_string((long) c) + ". Check font files.");
        missingChar.insert(c);
    }
    return false;
}

static void loadFont() {
    if (FT_Init_FreeType(&ftlib)) {
        error("Could not initialize FreeType library. Is it missing, or not compatible with current platform?");
        return;
    }
    for (auto &f: faces) {
        FT_Face face;
        if (FT_New_Face(ftlib, getAppResource("fonts/" + f).c_str(), 0, &face)) {
            error("Could not load font file '" + f + "'. Is this font file corrupted, or not supported by FreeType?");
            return;
        }
        info("Loaded font file '" + f + "'");
        FT_Set_Pixel_Sizes(face, 0, 48);
        alterFaces.push_back(face);
    }
}

static void cleanFont() {
    for (auto &p: glyphBuf) {
        glDeleteTextures(1, &p.second.texID);
    }
    for (auto &f: alterFaces) {
        FT_Done_Face(f);
    }
    FT_Done_FreeType(ftlib);
}

static GLuint
loadShader(const std::string &name) {
    if (name.empty()) {
        return 0;
    }
    if (shadersCtl.contains(name)) {
        return shadersCtl[name];
    }
    info("Compiling new shader '" + name + "'");
    GLuint vsh, fsh, prog;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream cbf(name);
    if (cbf.fail()) {
        error("Could not find shader files. Loading shader '" + name + "'");
        return 0;
    }

    std::stringstream ssrc;
    ssrc << cbf.rdbuf();
    auto cbsrc = splitStr(ssrc.str(), VT_SHADER_DELM, 2);
    if (cbsrc.size() < 2) {
        warn("Missing either head or body of shader '" + name + "', compilation might fail.");
    }
    std::string vsrc, fsrc;
    vsrc = cbsrc[0];
    fsrc = cbsrc[1];
    const char *vscrc = vsrc.c_str();
    const char *fscrc = fsrc.c_str();
    glShaderSource(vsh, 1, &vscrc, nullptr);
    glShaderSource(fsh, 1, &fscrc, nullptr);
    glCompileShader(vsh);
    glCompileShader(fsh);
    int success;
    char infoLog[1024];
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vsh, 1024, nullptr, infoLog);
        error("Shader compilation error detected. In VERT '" + name + "':");
        error(infoLog);
        return 0;
    }
    glGetShaderiv(fsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fsh, 1024, nullptr, infoLog);
        error("Shader compilation error detected. In FRAG '" + name + "':");
        error(infoLog);
        return 0;
    }

    prog = glCreateProgram();
    glAttachShader(prog, vsh);
    glAttachShader(prog, fsh);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog, 1024, nullptr, infoLog);
        error("Shader program linkage error detected. In '" + name + "':");
        error(infoLog);
        return 0;
    }
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    shadersCtl[name] = prog;
    return prog;
}

static GLuint loadTexture(const std::string &name) {
    if (name.empty()) {
        return 0;
    }
    if (texturesCtl.contains(name)) {
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
    if (data == nullptr) {
        error("Could not load texture '" + name + "'. Is this file missing, or is stb_image corrupted?");
        return 0;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (nrChannels == 3) {
        // Sample as vec4 (RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    texturesCtl[name] = tex;
    return tex;
}

static GLuint vbo, vao, textVBO, textVAO, bgVBO, bgVAO;

float bgDrawVert[6][4] = {
        {-1, 1,  0, 1},
        {-1, -1, 0, 0},
        {1,  1,  1, 1},
        {1,  1,  1, 1},
        {-1, -1, 0, 0},
        {1,  -1, 1, 0}};

void vtInitGraphics() {
    info("Initializing graphics buffers.");
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Initializing text buffers.");

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Initializing background buffers.");

    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);

    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, bgDrawVert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    info("Loading fonts.");
    loadFont();
}

void vtDeInitGraphics() {
    info("Deleting graphics buffers.");
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    info("Cleaning up shader and texture cache.");
    for (auto &p: shadersCtl) {
        glDeleteProgram(p.second);
    }
    for (auto &t: texturesCtl) {
        glDeleteTextures(1, &t.second);
    }
    shadersCtl.clear();
    texturesCtl.clear();
    info("Cleaning up fonts.");
    glDeleteBuffers(1, &textVBO);
    glDeleteVertexArrays(1, &textVAO);
    cleanFont();
}

// UI projection matrix
void vtSetBackground(const std::string &img) {
    if (img.empty()) {
        bgTex = 0;
        return;
    }
    // Update texture
    bgTex = loadTexture(img);
}

static void drawBg() {
    if (bgTex == 0) {
        return;
    }

    auto sd = loadShader(getAppResource("shaders/ui/background"));
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBindVertexArray(bgVAO);
    glUseProgram(sd);
    glActiveTexture(GL_TEXTURE2); // Texture 2 for bg
    glBindTexture(GL_TEXTURE_2D, bgTex);
    glUniform1i(glGetUniformLocation(sd, SHADER_VAR_TEX), 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int vtGetGraphicsError() {
    return glGetError();
}

void Triangle::draw() const {
    // The drawing of triangle uses buffer set 0 (default VBO and VAO)
    // With vertex data structure (xyzst)[3]
    float vertex[15] = {0};
    for (int i = 0; i < 3; ++i) {
        auto p = pt[i];
        for (int j = 0; j < 5; ++j) {
            vertex[5 * i + j] = p[j];
        }
    }
    auto sd = loadShader(params.external ? params.shader : getAppResource("shaders/" + params.shader));
    if (sd == 0) {
        warn("Invalid shader usage detected. Object might fail to draw.");
    }
    glUseProgram(sd);
    glUniformMatrix4fv(glGetUniformLocation(sd, SHADER_VAR_VIEW), 1, GL_FALSE, glm::value_ptr(params.ctx.viewMat));
    glUniformMatrix4fv(glGetUniformLocation(sd, SHADER_VAR_PROJ), 1, GL_FALSE, glm::value_ptr(params.ctx.projMat));

    unsigned int tx = 0;
    if (!params.texture.empty()) {
        tx = loadTexture(params.external ? params.texture : getAppResource("textures/" + params.texture + ".png"));
    }
    if (tx != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tx);
    }

    glUniform1i(glGetUniformLocation(sd, SHADER_VAR_TEX), 0);

    // Assign extra values
    float arg[params.args.size()];
    for (int i = 0; i < int(params.args.size()); ++i) {
        arg[i] = params.args[i];
    }
    glUniform1fv(glGetUniformLocation(sd, "args"), int(params.args.size()), arg);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rect::draw() const {
    pt[0].draw();
    pt[1].draw();
}

void TriangleStrip::draw() const {
    if (pts.size() < 3 || pts.size() % 3 != 0) {
        return;
    }
    for (int i = 0; i < int(pts.size() / 3); ++i) {
        Triangle t(pts[3 * i], pts[3 * i + 1], pts[3 * i + 2], params);
        t.draw();
    }
}

void DisplayText::draw() const {
    auto sd = loadShader(getAppResource("shaders/ui/text"));
    glUseProgram(sd);
    glUniform4f(glGetUniformLocation(sd, SHADER_VAR_COLOR), color.r, color.g, color.b, color.a);
    glActiveTexture(GL_TEXTURE1); // Use another texture
    glUniform1i(glGetUniformLocation(sd, SHADER_VAR_TEX), 1);
    glUniformMatrix4fv(glGetUniformLocation(sd, SHADER_VAR_PROJ), 1, GL_FALSE, glm::value_ptr(params.ctx.projMat));
    // View matrix is omitted
    glBindVertexArray(textVAO);

    // Coord Correction is no longer necessary
    auto x = pos.x;
    auto y = pos.y;
    for (auto gp: text) {
        if (!loadCharGlyph(gp)) {
            warn("Missing font glyph for char id " + std::to_string(int(gp)));
            continue;
        }
        Glyph &g = glyphBuf[gp];
        float w = g.size[0] * fSize;
        float h = g.size[1] * fSize;
        float xpos = x + g.bearing[0] * fSize;
        float ypos = y - (g.size[1] - g.bearing[1]) * fSize;
        float vertices[6][4] = {
                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos,     ypos,     0.0f, 1.0f},
                {xpos + w, ypos,     1.0f, 1.0f},

                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos,     1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, g.texID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += float(g.advance >> 6) * fSize;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void vtDrawList(DrawList &buf) {
    glGetError(); // Dispose previous
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawBg();
    glClear(GL_DEPTH_BUFFER_BIT); // Background depth reset
    for (auto &b: buf.objects) {
        b->draw();
    }
}
