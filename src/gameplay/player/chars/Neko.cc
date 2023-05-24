#include "Neko.hh"

#define NEKO_AUX_CD 30
#define NEKO_AUX_DUR 6
#define NEKO_FINAL_DUR 25
#define NEKO_SHIELD_CHARGE_INT 1.0
#define NEKO_SHIELD_CHARGE_RATE 5

NekoChar::NekoChar()
{
    charName = "NEKO";
    auxName = "NOTE SENSE";
    finalName = "NEKO UP!";
    finalReqPt = 40;
    finalChargedPt = 0;
    health = maxHealth = 100;
    shield = maxShield = 125;
}

void NekoChar::damage(unsigned int amount, bool real)
{
    // Damage reduction
    Player::damage(static_cast<int>(amount * 0.85 + 0.5), real);
    // TODO: input latency reduction
}

void NekoChar::tick(Game &g)
{
    nowTime = g.mapTimer.getTime();
    // Prevent Neko from downed
    if (stat == DOWNED && nowTime <= finalEnd)
    {
        stat = NORMAL;
    }

    // Activate skills
    if (auxShouldActivate)
    {
        auxActive = true;
        auxShouldActivate = false;
        auxEnd = nowTime + NEKO_AUX_DUR;

        if (!isDummy)
        {
            g.score.rules.judgeTime.allowBreak += 0.1;
            g.score.rules.judgeTime.range += 0.025;
            g.score.rules.judgeTime.good += 0.045;
            g.score.rules.judgeTime.almost += 0.050;
        }
    }
    else if (auxShouldDeactivate)
    {
        auxActive = false;
        auxShouldDeactivate = false;
        auxNextReadyTime = nowTime + NEKO_AUX_CD;
        if (!isDummy)
        {
            g.score.rules.judgeTime.allowBreak -= 0.1;
            g.score.rules.judgeTime.range -= 0.025;
            g.score.rules.judgeTime.good -= 0.045;
            g.score.rules.judgeTime.almost -= 0.050;
        }
    }
    else
    {
        if (nowTime > auxEnd && auxActive)
        {
            auxShouldDeactivate = true;
        }
    }

    if (finalShouldActivate)
    {
        finalActive = true;
        finalShouldActivate = false;
        finalNoteCount = g.score.getNoteCount();
        finalEnd = nowTime + NEKO_FINAL_DUR;
        // Cooldown aux now
        auxNextReadyTime = nowTime;
        lastTickScore = g.score.getCurrentScore();
    }
    else if (finalShouldDeactivate)
    {
        finalActive = false;
        finalShouldDeactivate = false;
        // TODO: recharge final based on hits
        auto d = g.score.getNoteCount() - finalNoteCount;
        if (d > 0)
        {
            chargeFinal(std::min(static_cast<int>(d / 10.0), 20));
        }
    }
    else
    {
        if (nowTime > finalEnd && finalActive)
        {
            finalShouldDeactivate = true;
        }
        else
        {
            // Add score
            auto ns = g.score.getCurrentScore();
            auto ds = ns - lastTickScore;
            if (ds > 0)
            {
                g.score.addBonusScore(static_cast<int>(ds * 0.25 + 0.5));
            }
            lastTickScore = ns;
        }
    }

    // Charge shield
    if (shield < maxShield)
    {
        if (lastShieldCharge == 0)
        {
            lastShieldCharge = nowTime;
        }
        else if (nowTime - lastShieldCharge > NEKO_SHIELD_CHARGE_INT)
        {
            lastShieldCharge = nowTime;
            chargeShield(NEKO_SHIELD_CHARGE_RATE);
        }
    }
    else
    {
        lastShieldCharge = 0;
    }
}

PlayerAssets NekoChar::getAssets() const
{
    return {.auxTex = "chars/neko/a", .finalTex = "chars/neko/final"};
}
