#ifndef GAMEPLAY_HUD_EFFECTMASK
#define GAMEPLAY_HUD_EFFECTMASK

#include <string>
#include <utility>

class DrawList;

enum class EffectMaskVariant {
  PULSE,
  GROW,
  FADE,
  STATIC
};

class EffectMask {
public:
  EffectMask(std::string sd, std::string tx, EffectMaskVariant v)
      : shader(std::move(sd)), texture(std::move(tx)), variant(v) {};

  void refresh(double absTime);

  void draw(double absTime, DrawList &d) const;

protected:
  std::string shader, texture;
  double beginTime = -10;
  EffectMaskVariant variant;
};

#endif /* GAMEPLAY_HUD_EFFECTMASK */
