#include "PlayerBox.hh"
#include "HUDDef.hh"
#include "engine/virtual/Window.hh"
#include <cglm/cglm.h>
#include <locale>
#include <codecvt>

#define PB_DIV_TOP 880
#define PB_DIV_LEFT 1330
#define PB_DIV_H -110
#define PB_DIV_W 250

#define PB_N_TOP 870
#define PB_N_LEFT 100
#define PB_N_SIZE 0.32

#define PB_HP_TOP 842
#define PB_HP_LEFT 50
#define PB_SH_TOP 820
#define PB_SH_LEFT 50

#define PB_BAR_HW 5
#define PB_BAR_L 180

#define PB_SH_BAR_C                              \
    {                                            \
        220 / 256.0, 220 / 256.0, 220 / 256.0, 1 \
    }
#define PB_HP_BAR_C                              \
    {                                            \
        197 / 256.0, 255 / 256.0, 163 / 256.0, 1 \
    }
#define PB_CN_C    \
    {              \
        1, 1, 1, 1 \
    }

#define PB_PN_C          \
    {                    \
        0.8, 0.8, 0.8, 1 \
    }

PlayerBox::PlayerBox(Player *p)
{
    int uc = p->getPID() - 1;
    float mgt = PB_DIV_TOP + uc * (PB_DIV_TOP + PB_DIV_H);
    vec2 hpPos = {PB_DIV_LEFT + PB_HP_LEFT, mgt + PB_HP_TOP};
    vec2 shPos = {PB_DIV_LEFT + PB_SH_LEFT, mgt + PB_SH_TOP};
    vec4 hc = PB_HP_BAR_C, sc = PB_SH_BAR_C;
    hpBar = Bar(HORZ, hpPos, PB_BAR_L, PB_BAR_HW);
    shieldBar = Bar(HORZ, shPos, PB_BAR_L, PB_BAR_HW);
    hpBar.setColor(hc);
    shieldBar.setColor(sc);
    playerPtr = p;
}

void PlayerBox::draw(DrawContext &ctx)
{
    int w, h;
    vtGetWindowSize(w, h);

    // Draw box
    int uc = playerPtr->getPID() - 1;
    float mgt = PB_DIV_TOP + uc * (PB_DIV_TOP + PB_DIV_H);
    Shape s;
    s.points[0][0] = PB_DIV_LEFT;
    s.points[0][1] = mgt;
    s.points[1][0] = PB_DIV_LEFT;
    s.points[1][1] = mgt + PB_DIV_H;

    s.points[2][0] = PB_DIV_LEFT + PB_DIV_W;
    s.points[2][1] = mgt;
    s.points[3][0] = PB_DIV_LEFT + PB_DIV_W;
    s.points[3][1] = mgt + PB_DIV_H;
    s.shader = "opaque-tex";
    s.texture = "hud/pb-" + std::to_string(playerPtr->getPID());

    // Draw names
    Typography ctp;
    ctp.pos[0] = PB_DIV_LEFT + PB_N_LEFT;
    ctp.pos[1] = mgt + PB_N_TOP;
    ctp.yAlign = CENTER;
    ctp.xAlign = CENTER;
    auto cname = playerPtr->getCharName();
    ctp.text = std::wstring(cname.begin(), cname.end());

    vec4 cColor = PB_CN_C;
    glm_vec4_copy(cColor, ctp.color);

    ctp.size = PB_N_SIZE;

    ctx.typos.push_back(ctp);

    // Draw bar
    hpBar.setLength(playerPtr->getHealthRate());
    shieldBar.setLength(playerPtr->getShieldRate());

    hpBar.draw(ctx);
    shieldBar.draw(ctx);
    ctx.shapes.push_back(s);
}