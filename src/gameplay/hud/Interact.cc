#include "Interact.hh"
#include "engine/virtual/UIHook.hh"

#define INT_FINAL_BTM 150
#define INT_FINAL_RIGHT 145
#define INT_FINAL_SIZE 90

#define INT_TEX_OFFSET 20

#define INT_AUX_BTM 360
#define INT_AUX_RIGHT 145
#define INT_AUX_SIZE 70

#define SK_TEXT_SIZE 0.25

void SkillBtn::draw(DrawContext &ctx)
{
    Shape sp = {
        .shader = shader,
        .texture = texture,
        .points = {
            {posx - size, posy + size}, {posx - size, posy - size}, {posx + size, posy + size}, {posx + size, posy - size}},
        .args = {stat, blinkScale}};

    Typography tp;
    tp.xAlign = tp.yAlign = CENTER;
    tp.size = SK_TEXT_SIZE;
    tp.pos[0] = posx;
    tp.pos[1] = posy - size - INT_TEX_OFFSET;
    tp.color[0] = tp.color[1] = tp.color[2] = tp.color[3] = 1;
    tp.text = std::wstring(text.begin(), text.end());
    ctx.shapes.push_back(sp);
    ctx.typos.push_back(tp);
}

void SkillBtn::hook(UIHookXT xt)
{
    HookSt s = {
        .xbegin = static_cast<int>(posx - size),
        .xend = static_cast<int>(posx + size),
        .ybegin = static_cast<int>(posy - size),
        .yend = static_cast<int>(posy + size),
        .xt = xt,
    };
    uhid = vtAddUIHook(s);
}

void SkillBtn::dehook()
{
    if (uhid != 0)
    {
        vtDeUIHook(uhid);
    }
}

Interact::Interact(Player *p)
{
    auto pst = p->getAssets();

    auxBtn.posx = 1600 - INT_AUX_RIGHT;
    auxBtn.posy = INT_AUX_BTM;
    auxBtn.size = INT_AUX_SIZE;
    auxBtn.shader = "skill-btn";
    auxBtn.texture = pst.auxTex;

    finalBtn.posx = 1600 - INT_FINAL_RIGHT;
    finalBtn.posy = INT_FINAL_BTM;
    finalBtn.size = INT_FINAL_SIZE;
    finalBtn.shader = "skill-btn";
    finalBtn.texture = pst.finalTex;

    player = p;
    auxBtn.hook([this]() -> void
                { this->player->activateSkill(SK_AUX); });
    finalBtn.hook([this]() -> void
                  { this->player->activateSkill(SK_FINAL); });
};

Interact::~Interact()
{
    auxBtn.dehook();
    finalBtn.dehook();
}

static std::string getSkillProc(double pct)
{
    std::string cstr;
    if (pct == 2 || pct == -1)
    {
        cstr = "[ACT]";
    }
    else if (pct == 1)
    {
        cstr = "[OK!]";
    }
    else
    {
        auto icr = std::to_string(static_cast<int>(pct * 100));
        if (icr.size() == 1)
        {
            icr = "0" + icr;
        }
        cstr = "[" + icr + "%]";
    }
    return cstr;
}

void Interact::tick(double absTime)
{
    auto auxProc = player->getSkillStat(SK_AUX);
    auxBtn.stat = auxProc;
    auxBtn.text = player->getSkillName(SK_AUX) + " " + getSkillProc(auxProc);

    auto finalProc = player->getSkillStat(SK_FINAL);
    finalBtn.stat = finalProc;
    finalBtn.text = player->getSkillName(SK_FINAL) + " " + getSkillProc(finalProc);

    auxBtn.blinkScale = finalBtn.blinkScale = absTime;
}

void Interact::draw(DrawContext &ctx)
{
    auxBtn.draw(ctx);
    finalBtn.draw(ctx);
    // TODO: draw interaction area
}