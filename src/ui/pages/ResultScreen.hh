#ifndef ALMOSTTHERE_SRC_UI_PAGES_RESULTSCREEN_HH
#define ALMOSTTHERE_SRC_UI_PAGES_RESULTSCREEN_HH

#include "UIPage.hh"
#include "gameplay/hud/EffectMask.hh"
#include "ui/comp/impl/Box.hh"
#include "ui/comp/impl/Button.hh"
#include "ui/comp/impl/Label.hh"
#include "ui/comp/impl/TextArea.hh"
#include <string>

enum class ResultScreenVariant {
  COMPLETE,
  FAILED,
};

struct ResultSummary {
  ResultScreenVariant variant;
  unsigned int score;
  std::string songTitle, artist, diffLevel, playerName, rank, banner, diffColor;
  unsigned int pf, at, ac, md, tc, lt;
};

class ResultScreen final : public UIPage {
public:
  ResultScreen(const ResultSummary &s, double time);
  void draw(double t, DrawList &d) override;

protected:
  ResultScreenVariant variant;
  Label playerName, hintText; // Player name and hint text
  Box albumImage; // Album image
  Label songTitle, artist, diffLevel; // Difficulty level
  Label score; // Total score
  Label cPF, cAT, cAC, cMD, cTC, cLT; // Counters
  Box rankIcon; // The rank icon
  Button contBtn; // Continue
  Box background; // Background cover
  EffectMask transition; // Fade-in method
};

#endif // ALMOSTTHERE_SRC_UI_PAGES_RESULTSCREEN_HH
