#include "Slot.hh"
#include "Note.hh"

#include "gameplay/base/Game.hh"

#include <cstdlib>

#define SLOT_SIZE 1.2

static std::vector<std::string> slotTexName = {"circle", "eureka", "line"};

void Slot::draw()
{
    if (!isActive)
    {
        return;
    }
    auto stat = controller->getState();
    Polygon p;
    p.renderPreset = RECT;
    p.shader = "3d/rect";
    p.texture = slotTexName[variant];

    auto dw = glm::normalize(glm::cross(stat.up, stat.normal)) * float(SLOT_SIZE);
    auto dh = stat.up * float(variant == CIRCLE ? SLOT_SIZE : SLOT_SIZE / 2);
    auto ctr = stat.pos - dh;

    auto rt = ctr + dw + dh;
    auto lt = ctr - dw + dh;
    auto lb = ctr - dw - dh;
    auto rb = ctr + dw - dh;

    p.points = {lt, lb, rt, rb};
    game.drawContext.polygons.push_back(p);
}

std::shared_ptr<Slot> Slot::createSlot(std::weak_ptr<SlotObject> o, Game &g)
{
    auto st = std::make_shared<Slot>(g);
    auto opt = o.lock();
    st->controller = std::make_shared<ObjController>(*opt);
    st->variant = opt->slotType;
    return st;
}
