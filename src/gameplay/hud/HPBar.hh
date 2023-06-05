#ifndef GAMEPLAY_HUD_HP_HPBAR
#define GAMEPLAY_HUD_HP_HPBAR

#include <list>

class DrawList;

class Game;

enum HPBarVariant {
    HP_NORMAL,
    HP_LOW,
};

class HPDrainSection {
public:
    HPDrainSection(double valueBegin, double valuePct, double t, HPBarVariant va);

    void draw(Game &g, double absTime);

    bool shouldUnload(double absTime) const;

protected:
    unsigned int xbegin, xend;
    double beginTime;
    HPBarVariant variant;
};

class HPBar {
public:
    HPBar(Game &g, HPBarVariant va = HP_NORMAL, unsigned int max = 100, unsigned int cur = 100) : game(g), variant(va),
                                                                                                  hpCurrent(cur),
                                                                                                  hpMax(max) {};

    /**
     * @brief Sets the new HP value.
     * @param animation Whether to play drain animation if HP decreases.
     * @param nv New HP value.
     */
    void setHP(unsigned int nv, bool animation = true);

    void setVariant(HPBarVariant v) { variant = v; };

    void draw();

protected:
    Game &game;
    HPBarVariant variant;
    unsigned hpCurrent, hpMax;
    std::list<HPDrainSection> hpDrainEffects;
    double healEffectBeginTime = -1;
};

#endif /* GAMEPLAY_HUD_HP_HPBAR */
