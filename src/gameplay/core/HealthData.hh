#ifndef GAMEPLAY_CORE_HEALTHDATA
#define GAMEPLAY_CORE_HEALTHDATA

struct HealthData
{
    unsigned int health, maxHealth;
    unsigned int shield, maxShield;
    unsigned int magic, maxMagic;
    double shieldRechargeCooldown, regenerationCooldown, magicReplenishCooldown;
    unsigned int shieldRechargeRate, regenerationRate, magicReplenishRate;
};

#endif /* GAMEPLAY_CORE_HEALTHDATA */
