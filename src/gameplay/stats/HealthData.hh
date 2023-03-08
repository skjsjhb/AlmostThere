#ifndef GAMEPLAY_STATS_HEALTHDATA
#define GAMEPLAY_STATS_HEALTHDATA

struct HealthData
{
    unsigned int health, maxHealth;
    unsigned int shield, maxShield;
    unsigned int magic, maxMagic;
    double shieldRechargeCooldown, regenerationCooldown, magicReplenishCooldown;
    unsigned int shieldRechargeRate, regenerationRate, magicReplenishRate;
};

#endif /* GAMEPLAY_STATS_HEALTHDATA */
