#ifndef ENGINE_VIRTUAL_GRAPHICS
#define ENGINE_VIRTUAL_GRAPHICS

#include <utility>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <list>
#include <memory>
#include <glm/glm.hpp>

class Point {
public:
    Point() : p(0.0f), st(0.0f) {};

    explicit Point(const glm::vec3 &x) : p(x), st(0.0f) {};

    Point(const glm::vec3 &x, const glm::vec2 &s) : p(x), st(s) {};

    [[nodiscard]] glm::vec3 getPosition() const { return p; }

    [[nodiscard]] glm::vec2 getTexCoord() const { return st; }

    float operator[](int ind) const {
        if (ind <= 2) {
            return p[ind];
        } else if (ind <= 4) {
            return st[ind - 3];
        }
        return 0;
    }

protected:
    glm::vec3 p;  // Position
    glm::vec2 st; // Texture coord
};

struct DrawContext {
    glm::mat4 viewMat, projMat;
};

struct DrawParam {
    std::string shader;
    std::string texture;
    bool external = false; // Whether the specified assets shoule be intepreted as external assets

    // Arguments for shaders
    std::vector<float> args;
    DrawContext ctx;
};

class DrawObject {
public:
    DrawObject() = default;

    explicit DrawObject(DrawParam p) : params(std::move(p)) {};

    virtual ~DrawObject() = default;

    virtual void draw() const {};

protected:
    DrawParam params;
};

class Triangle : public DrawObject {
public:
    Triangle(const Point &p1, const Point &p2, const Point &p3, const DrawParam &p) : DrawObject(p) {
        pt[0] = p1;
        pt[1] = p2;
        pt[2] = p3;
    };

    void draw() const override;

protected:
    std::array<Point, 3> pt;
};

class Rect : public DrawObject {
public:
    Rect(const Point &p1, const Point &p2, const Point &p3, const Point &p4,
         const DrawParam &p) : DrawObject(p), pt({Triangle(p1, p2, p3, p), Triangle(p3, p2, p4, p)}) {};

    void draw() const override;

protected:
    std::array<Triangle, 2> pt;
};

class TriangleStrip : public DrawObject {
public:
    TriangleStrip(const std::vector<Point> &pt, const DrawParam &p) : DrawObject(p) { pts = pt; };

    void draw() const override;

protected:
    std::vector<Point> pts;
};

class DisplayText : public DrawObject {
public:
    DisplayText(const glm::vec2 coord, float size, std::wstring st, glm::vec4 col, const DrawParam &p)
            : DrawObject(p), pos(coord), fSize(size), color(col), text(std::move(st)) {};

    void draw() const override;

protected:
    glm::vec2 pos;
    float fSize;
    glm::vec4 color = {1, 1, 1, 1};
    std::wstring text;
};

class DrawList {
public:
    std::list<std::unique_ptr<DrawObject>> objects;

    void add(std::unique_ptr<DrawObject> &&objs) { objects.push_back(std::move(objs)); }

    void clear() { objects.clear(); };
};

extern void vtInitGraphics();

extern void vtDeInitGraphics();

extern int vtGetGraphicsError();

extern void vtSetBackground(const std::string &img);

extern void vtDrawList(DrawList &buf);

#endif /* ENGINE_VIRTUAL_GRAPHICS */
