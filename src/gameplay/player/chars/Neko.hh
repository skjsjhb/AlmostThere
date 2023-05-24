#ifndef GAMEPLAY_PLAYER_CHARS_NEKO
#define GAMEPLAY_PLAYER_CHARS_NEKO

#include "../Player.hh"

/**
 * @brief The implementation of character 'Neko'.
 * @author skjsjhb
 */

class NekoChar : public Player
{
public:
    NekoChar();
    virtual void damage(unsigned int amount, bool real = false) override;
    virtual void tick(Game &g) override;
    virtual PlayerAssets getAssets() const override;

protected:
    unsigned int lastTickScore;
    unsigned int finalNoteCount;
    double lastShieldCharge = 0;
};

#endif /* GAMEPLAY_PLAYER_CHARS_NEKO */
