#ifndef ALMOSTTHERE_SRC_UI_PAGES_UIPAGE_HH
#define ALMOSTTHERE_SRC_UI_PAGES_UIPAGE_HH

class DrawList;

class UIPage {
public:
  /**
   * @brief Main page draw method.
   * @param d Target draw list.
   */
  virtual void draw(double t, DrawList &d) = 0;

};

#endif // ALMOSTTHERE_SRC_UI_PAGES_UIPAGE_HH
