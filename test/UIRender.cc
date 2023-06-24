#include "TestTools.hh"

#include "engine/virtual/Framework.hh"
#include "engine/virtual/Graphics.hh"
#include "engine/virtual/Window.hh"
#include "engine/virtual/Input.hh"
#include "support/Locale.hh"
#include "ui/comp/impl/Button.hh"
#include "ui/pages/ResultScreen.hh"

int main() {
  vtInitWindow();
  vtInitGraphics();
  vtInitInput();
  vtSetFPSCap(60);
  setLocale("zh-CN");
  ResultScreen rs({
                      .variant = ResultScreenVariant::COMPLETE,
                      .score = 123456789,
                      .songTitle = "Meteor Crossroads",
                      .artist= "OpenLight Game OST // Team Almost There",
                      .diffLevel = "Revive Lv.500",
                      .playerName = "Player",
                      .diffColor = "245,37,12,255",
                      .pf = 320,
                      .at = 645,
                      .ac = 137,
                      .md = 16,
                      .tc = 2,
                      .lt = 160,
                  }, vtGetTime());
  DrawList dr;
  if (vtShouldDraw()) {
    dr.clear();
    rs.draw(vtGetTime(), dr);
    vtDrawList(dr);
    vtDisplayFlip();
    dr.clear();
  }
  vtWindowLoop();
  WANT(vtGetGraphicsError() == 0)
  TEND
}