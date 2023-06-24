#include "ResultScreen.hh"
#include "engine/virtual/Graphics.hh"
#include "gameplay/hud/EffectMask.hh"
#include "support/Locale.hh"
#include "util/Util.hh"
#include <string>
#include <fmt/format.h>

ResultScreen::ResultScreen(const ResultSummary &s, double time)
    : variant(s.variant),
      playerName({
                     {"x", "100"},
                     {"y", "770"},
                     {"width", "600"},
                     {"height", "0"},// Auto
                     {"size", "0.25"},
                     {"text", s.playerName},
                     {"color", "255,255,255,255"},
                     {"font", "neat"}
                 }),
      hintText({
                   {"x", "100"},
                   {"y", "700"},
                   {"width", "600"},
                   {"height", "0"},
                   {"size", "0.375"},
                   {"text", wstring2unicode(getLocaleText(
                       std::string("Result/Hint/") + (s.variant == ResultScreenVariant::FAILED ? "Fail" : "Pass")))},
                   {"color", s.variant == ResultScreenVariant::FAILED ? "255,226,76,255" : "155,232,255,255"},
                   {"font", "mono"}
               }),
      albumImage({
                     {"x", "100"},
                     {"y", "450"},
                     {"width", "600"},
                     {"height", "200"},
                     {"visible", "1"},
                     {"background", s.banner},
                     {"external", "1"},
                 }),
      songTitle({
                    {"x", "100"},
                    {"y", "330"},
                    {"width", "600"},
                    {"height", "0"},
                    {"size", "0.4"},
                    {"text", s.songTitle},
                    {"color", "246,251,252,255"},
                    {"font", "styled"}
                }),
      artist({
                 {"x", "100"},
                 {"y", "270"},
                 {"width", "250"},
                 {"height", "0"},
                 {"size", "0.2"},
                 {"text", s.artist},
                 {"color", "206,212,212,255"},
                 {"font", "styled"}
             }),
      diffLevel({
                    {"x", "100"},
                    {"y", "210"},
                    {"width", "250"},
                    {"height", "0"},
                    {"size", "0.25"},
                    {"text", s.diffLevel},
                    {"color", s.diffColor},
                    {"font", "neat"}
                }),
      score({
                {"x", "362"},
                {"y", "687"},
                {"xalign", "center,center"},
                {"width", "0"},
                {"height", "0"},
                {"size", "0.52"},
                {"text", fmt::format("{:09d}", s.score)},
                {"color", "255,255,255,255"},
                {"font", "neat"}
            }),
      cPF({
              {"x", "1045"},
              {"y", "515"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.pf)},
              {"color", "255,229,67,255"},
              {"font", "neat"}
          }),
      cAT({
              {"x", "1045"},
              {"y", "445"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.at)},
              {"color", "161,229,222,255"},
              {"font", "neat"}
          }),
      cAC({
              {"x", "1045"},
              {"y", "376"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.ac)},
              {"color", "161,229,67,255"},
              {"font", "neat"}
          }),
      cMD({
              {"x", "1045"},
              {"y", "307"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.md)},
              {"color", "224,225,204,255"},
              {"font", "neat"}
          }),
      cTC({
              {"x", "1045"},
              {"y", "238"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.tc)},
              {"color", "118,118,118,255"},
              {"font", "neat"}
          }),
      cLT({
              {"x", "1045"},
              {"y", "168"},
              {"width", "150"},
              {"height", "0"},
              {"size", "0.33"},
              {"text", std::to_string(s.lt)},
              {"color", "224,63,43,255"},
              {"font", "neat"}
          }),
      rankIcon({
                   {"x", "1325"},
                   {"y", "350"},
                   {"width", "200"},
                   {"height", "200"},
                   {"visible", "1"},
                   {"background", "ui/level-" + s.rank},
               }),
      contBtn({
                  {"x", "1300"},
                  {"y", "120"},
                  {"width", "240"},
                  {"height", "80"},
                  {"color", "255,255,255,255"},
                  {"background", "ui/btn-ok"},
              }),
      background({
                     {"x", "0"},
                     {"y", "0"},
                     {"width", "1600"},
                     {"height", "900"},
                     {"visible", "1"},
                     {"background",
                      std::string("ui/result-") + (variant == ResultScreenVariant::FAILED ? "fail" : "pass")},
                 }),
      transition("hud/effect-mask", "hud/flash-effect", EffectMaskVariant::FADE) {
  transition.refresh(time);
}

void ResultScreen::draw(double t, DrawList &d) {
  background.computeLayout();
  background.draw(d);
  score.computeLayout();
  playerName.computeLayout();
  playerName.draw(d);
  hintText.computeLayout();
  hintText.draw(d);
  albumImage.computeLayout();
  albumImage.draw(d);
  songTitle.computeLayout();
  songTitle.draw(d);
  artist.computeLayout();
  artist.draw(d);
  diffLevel.computeLayout();
  diffLevel.draw(d);
  score.computeLayout();
  score.draw(d);
  cPF.computeLayout();
  cPF.draw(d);
  cAT.computeLayout();
  cAT.draw(d);
  cAC.computeLayout();
  cAC.draw(d);
  cMD.computeLayout();
  cMD.draw(d);
  cTC.computeLayout();
  cTC.draw(d);
  cLT.computeLayout();
  cLT.draw(d);
  if (variant != ResultScreenVariant::FAILED) {
    rankIcon.computeLayout();
    // rankIcon.draw(d);
  }
  contBtn.computeLayout();
  contBtn.draw(d);
  transition.draw(t, d);
}
