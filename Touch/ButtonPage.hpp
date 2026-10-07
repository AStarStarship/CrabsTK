// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_BUTTONPAGE_DECL_DECL
#define CRABSTK_TOUCH_BUTTONPAGE_DECL_DECL
#include "Button.hpp"
#include "ControlLayer.hpp"
#include "WidgetPage.hpp"
namespace _ {

/* A type of Button that loads a WidgetPage into a Control Layer
The difference between a page and a macro button is that a page button
swtiches between pages in a template and a PageButton will be able to
perform other tasks in a virtual instrument such as loading one of the
sub-menus. Not sure if this class is neccissary yet. */
class LIB_MEMBER PageButton : public Button {
 public:
  /* Constructor. */
  PageButton(const WidgetPage& thePage = WidgetPage())
    : Button (initPage.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (initPage)) {
    // Nothing to do here :-)
  }

  /* Copy constructor. */
  PageButton(const PageButton& o)
    : Button (o.label (), Control::PAGE_BUTTON, MOMENTARY),
    thisPage (TemplatePage (o.thisPage)) {}

  /* Destructor. */
  virtual ~PageButton () {}

  /* The action  (s) performed when this button gets pressed. */
  void Press(const ControlLayer& parentLayer) { Button::Press (); }

  /* The action  (s) performed when this button gets FPD pressed. */
  void Depress(const ControlLayer& parentLayer) {
    Button::DoublePress ();
  }

  /* Gets thw WidgetPage. */
  WidgetPage* GetPage() { return page_; }

  /* Sets thisPage to the newPage.
  void SetPage(WidgetPage* newPage);

  /* Prints this object to a terminal. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    return o << "\nButtonPage:\n" << page_.Print (o);
  }

 private:
  WidgetPage* page_;  //< The page to load.
};
}  //< namespace _
#endif
#endif
