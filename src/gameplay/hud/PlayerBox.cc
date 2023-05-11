#include "PlayerBox.hh"
#include "HUDDef.hh"
#include "engine/virtual/Window.hh"
#include <cglm/cglm.h>
#include <locale>
#include <codecvt>

#define PLAYER_BOX_MARGIN 0.05
#define PLAYER_BOX_WIDTH 0.35
#define PLAYER_BOX_HEIGHT 0.2
#define PLAYER_BOX_BAR_LEN_RATIO 0.9
#define PLAYER_BOX_BAR_VERT_PADDING 0.6
#define PLAYER_BOX_BAR_DELM_WIDTH 0.2
#define PLAYER_BOX_NAME_PADDING 0.3
#define PLAYER_BOX_BAR_HORZ_PADDING 0.05

#define PLAYER_BOX_NAME_FONT_SIZE 0.4
#define PLAYER_BOX_PNAME_FONT_SIZE 0.2
#define PLAYER_BOX_NAME_HORZ_PADDING 0.05
#define PLAYER_BOX_NAME_VERT_PADDING 0.2
#define PLAYER_BOX_NAME_DELM 0.2

#define PLAYER_BOX_SHIELD_COLOR                  \
    {                                            \
        220 / 256.0, 220 / 256.0, 220 / 256.0, 1 \
    }
#define PLAYER_BOX_HP_COLOR                      \
    {                                            \
        197 / 256.0, 255 / 256.0, 163 / 256.0, 1 \
    }
#define PLAYER_BOX_NAME_COLOR \
    {                         \
        1, 1, 1, 1            \
    }

#define PLAYER_BOX_PNAME_COLOR \
    {                          \
        0.8, 0.8, 0.8, 0.8     \
    }

PlayerBox::PlayerBox(Player *p)
{
    float absMargin = -1 + PLAYER_BOX_BAR_HORZ_PADDING * PLAYER_BOX_WIDTH + PLAYER_BOX_MARGIN;
    float yMarginHP = 1 - PLAYER_BOX_MARGIN - PLAYER_BOX_BAR_VERT_PADDING * PLAYER_BOX_HEIGHT;
    float yMarginShield = yMarginHP - PLAYER_BOX_BAR_DELM_WIDTH * PLAYER_BOX_HEIGHT;
    vec2 hpPos = {absMargin, yMarginHP},
         shieldPos = {absMargin, yMarginShield};

    vec4 hpColor = PLAYER_BOX_HP_COLOR, shieldColor = PLAYER_BOX_SHIELD_COLOR;
    hpBar = Bar(HORZ, hpPos, PLAYER_BOX_BAR_LEN_RATIO * PLAYER_BOX_WIDTH, hpColor);
    shieldBar = Bar(HORZ, shieldPos, PLAYER_BOX_BAR_LEN_RATIO * PLAYER_BOX_WIDTH, shieldColor);
    playerPtr = p;
}

void PlayerBox::draw(DrawContext &ctx)
{
    int w, h;
    vtGetWindowSize(w, h);
    // Draw names
    Typography ctp, ptp;
    ctp.pos[0] = ptp.pos[0] = (PLAYER_BOX_NAME_HORZ_PADDING * PLAYER_BOX_WIDTH + PLAYER_BOX_MARGIN) * w * 0.5;
    ctp.pos[1] = (2 - PLAYER_BOX_MARGIN - PLAYER_BOX_NAME_VERT_PADDING * PLAYER_BOX_HEIGHT) * h * 0.5;
    ptp.pos[1] = ctp.pos[1] - (PLAYER_BOX_NAME_DELM * PLAYER_BOX_HEIGHT) * h * 0.5;

    ctp.text = std::wstring(playerPtr->charName.begin(), playerPtr->charName.end());
    ptp.text = std::wstring(playerPtr->playerName.begin(), playerPtr->playerName.end());

    vec4 cColor = PLAYER_BOX_NAME_COLOR, pColor = PLAYER_BOX_PNAME_COLOR;
    glm_vec4_copy(cColor, ctp.color);
    glm_vec4_copy(pColor, ptp.color);

    ctp.size = PLAYER_BOX_NAME_FONT_SIZE;
    ptp.size = PLAYER_BOX_PNAME_FONT_SIZE;

    ctx.typos.push_back(ctp);
    ctx.typos.push_back(ptp);

    // Draw bar
    hpBar.setLength(playerPtr->health / (double)playerPtr->maxHealth);
    shieldBar.setLength(playerPtr->shield.curHP / (double)playerPtr->shield.maxHP);

    hpBar.draw(ctx);
    shieldBar.draw(ctx);
}