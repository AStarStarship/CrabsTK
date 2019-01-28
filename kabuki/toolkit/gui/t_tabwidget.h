/*
    /kabuki/toolkit/gui/tabwidget.h -- A wrapper around the widgets TabHeader and StackedWidget
    which hooks the two classes together.

    The tab widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_widget.h"
#include "t_tabheader.h"
#include "t_stackedwidget.h"
#include "t_theme.h"
#include "t_opengl.h"
#include "t_window.h"
#include "t_screen.h"
#include <algorithm>
#include <functional>

namespace _ {

/* A wrapper around the widgets TabHeader and StackedWidget which hooks
the two classes together.
@warning Unlike other widgets, children may **not** be added *directly* to a
TabWidget.  For example, the following code will raise an exception:

@code

// `this` might be say a _::Screen instance
Window *window = new Window(this, "Window Title");
TabWidget *tabWidget = window->add<TabWidget>();
// this label would be a direct child of tabWidget,
// which is forbidden, so an exception will be raised
new Label(tabWidget, "Some Label");

Instead, you are expected to be creating tabs and adding widgets to those.

@code

// `this` might be say a _::Screen instance
Window *window = new Window(this, "Window Title");
TabWidget *tabWidget = window->add<TabWidget>();
// Create a tab first
auto *layer = tabWidget->createTab("Tab Name");
// Add children to the created tabs
layer->setLayout(new GroupLayout());
new Label(layer, "Some Label");

A slightly more involved example of creating a TabWidget can also be found
in :ref:`nanogui_example_1` (search for `tabWidget` in the file). */
class SDK TabWidget : public Widget {
public:
  TabWidget (Widget *parent)
    : Widget (parent)
    , mHeader (new TabHeader (nullptr)) // create using nullptr, add children below
    , mContent (new StackedWidget (nullptr)) {

    // since TabWidget::addChild is going to throw an exception to prevent
    // mis-use of this class, add the child directly
    Widget::addChild (childCount (), mHeader);
    Widget::addChild (childCount (), mContent);

    mHeader->setCallback ([this](int i) {
      mContent->setSelectedIndex (i);
      if (mCallback)
        mCallback (i);
    });
  }

  /* Forcibly prevent mis-use of the class by throwing an exception.
  Children are not to be added directly to the TabWidget, see the class level
  documentation (@ref TabWidget) for an example.
  @throws std::runtime_error An exception is always thrown, as children
  are not allowed to be added directly to this Widget. */
  virtual void addChild (int index, Widget *widget) override {
    // there may only be two children: mHeader and mContent, created in the constructor
    throw std::runtime_error (
      "TabWidget: do not add children directly to the TabWidget, create tabs "
      "and add children to the tabs.  See TabWidget class documentation for "
      "example usage."
    );
  }

  void setActiveTab (int tabIndex) {
    mHeader->setActiveTab (tabIndex);
    mContent->setSelectedIndex (tabIndex);
  }

  int activeTab () const {
    DASSERT (mHeader->activeTab () == mContent->selectedIndex ());
    return mContent->selectedIndex ();
  }

  int tabCount () const {
    DASSERT (mContent->childCount () == mHeader->tabCount ());
    return mHeader->tabCount ();
  }

  /*
   * Sets the callable objects which is invoked when a tab is changed.
   * The argument provided to the callback is the index of the new active tab.
   */
  void setCallback (const std::function<void (int)> &callback) { mCallback = callback; };
  const std::function<void (int)> &callback () const { return mCallback; }

  // Creates a new tab with the specified name and returns a pointer to the layer.
  Widget *createTab (const std::string &label) {
    return createTab (tabCount (), label);
  }

  Widget *createTab (int index, const std::string &label) {
    Widget* tab = new Widget (nullptr);
    addTab (index, label, tab);
    return tab;
  }

  // Inserts a tab at the end of the tabs collection and associates it with the provided widget.
  void addTab (const std::string &label, Widget *tab) {
    addTab (tabCount (), name, tab);
  }

  /* Inserts a tab into the tabs collection at the specified index and
  associates it with the provided widget. */
  void addTab (int index, const std::string &label, Widget *tab) {
    DASSERT (index <= tabCount ());
    // It is important to add the content first since the callback
    // of the header will automatically fire when a new tab is added.
    mContent->addChild (index, tab);
    mHeader->addTab (index, label);
    DASSERT (mHeader->tabCount () == mContent->childCount ());
  }

  /*
   * Removes the tab with the specified label and returns the index of the label.
   * Returns whether the removal was successful.
   */
  bool removeTab (const std::string &label) {
    int index = mHeader->removeTab (tabName);
    if (index == -1)
      return false;
    mContent->removeChild (index);
    return true;
  }

  // Removes the tab with the specified index.
  void removeTab (int index) {
    DASSERT (mContent->childCount () < index);
    mHeader->removeTab (index);
    mContent->removeChild (index);
    if (activeTab () == index)
      setActiveTab (index == (index - 1) ? index - 1 : 0);
  }

  // Retrieves the label of the tab at a specific index.
  const std::string &tabLabelAt (int index) const {
    return mHeader->tabLabelAt (index);
  }

  /* Retrieves the index of a specific tab using its tab label.
   @return -1 if there is no such tab. */
  int tabLabelIndex (const std::string &label) {
    return mHeader->tabIndex (label);
  }

  /* Retrieves the index of a specific tab using a widget pointer.
  @return -1 if there is no such tab. */
  int tabIndex (Widget* tab) {
    return mContent->childIndex (tab);
  }

  /* This function can be invoked to ensure that the tab with the provided
  index the is visible, i.e to track the given tab. Forwards to the tab
  header widget. This function should be used whenever the client wishes
  to make the tab header follow a newly added tab, as the content of the
  new tab is made visible but the tab header does not track it by default. */
  void ensureTabVisible (int index) {
    if (!mHeader->isTabVisible (index))
      mHeader->ensureTabVisible (index);
  }

  /* Returns a `const` pointer to the Widget associated with the specified
  label.
  @param label The label used to create the tab.
  @return The Widget associated with this label, or `nullptr` if not found.
   */
  const Widget *tab (const std::string &label) const {
    int index = mHeader->tabIndex (tabName);
    if (index == -1 || index == mContent->childCount ())
      return nullptr;
    return mContent->children ()[index];
  }

  /*
   * @brief Returns a pointer to the Widget associated with the specified label.
   *
   * @param label
   *     The label used to create the tab.
   *
   * @return
   *     The Widget associated with this label, or `nullptr` if not found.
   */
  Widget *tab (const std::string &label) {
    int index = mHeader->tabIndex (tabName);
    if (index == -1 || index == mContent->childCount ())
      return nullptr;
    return mContent->children ()[index];
  }

  /*
   * @brief Returns a `const` pointer to the Widget associated with the
   *        specified index.
   *
   * @param index
   *     The current index of the desired Widget.
   *
   * @return
   *     The Widget at the specified index, or `nullptr` if `index` is not
   *     a valid index.
   */
  const Widget *tab (int index) const {
    if (index < 0 || index >= mContent->childCount ())
      return nullptr;
    return mContent->children ()[index];
  }

  /*
   * @brief Returns a pointer to the Widget associated with the specified index.
   *
   * @param index
   *     The current index of the desired Widget.
   *
   * @return
   *     The Widget at the specified index, or `nullptr` if `index` is not
   *     a valid index.
   */
  Widget *tab (int index) {
    if (index < 0 || index >= mContent->childCount ())
      return nullptr;
    return mContent->children ()[index];
  }

  virtual void performLayout (NVGcontext* ctx) override {
    int headerHeight = mHeader->preferredSize (ctx).y ();
    int margin = mTheme->mTabInnerMargin;
    mHeader->setPosition ({ 0, 0 });
    mHeader->setSize ({ mSize.x (), headerHeight });
    mHeader->performLayout (ctx);
    mContent->setPosition ({ margin, headerHeight + margin });
    mContent->setSize ({ mSize.x () - 2 * margin, mSize.y () - 2 * margin - headerHeight });
    mContent->performLayout (ctx);
  }

  virtual Vector2i preferredSize (NVGcontext* ctx) const override {
    auto contentSize = mContent->preferredSize (ctx);
    auto headerSize = mHeader->preferredSize (ctx);
    int margin = mTheme->mTabInnerMargin;
    auto borderSize = Vector2i (2 * margin, 2 * margin);
    Vector2i tabPreferredSize = contentSize + borderSize + Vector2i (0, headerSize.y ());
    return tabPreferredSize;
  }

  virtual void draw (NVGcontext* ctx) override {
    int tabHeight = mHeader->preferredSize (ctx).y ();
    auto activeArea = mHeader->activeButtonArea ();


    for (int i = 0; i < 3; ++i) {
      nvgSave (ctx);
      if (i == 0)
        nvgIntersectScissor (ctx, mPos.x (), mPos.y (), activeArea.first.x () + 1, mSize.y ());
      else if (i == 1)
        nvgIntersectScissor (ctx, mPos.x () + activeArea.second.x (), mPos.y (), mSize.x () - activeArea.second.x (), mSize.y ());
      else
        nvgIntersectScissor (ctx, mPos.x (), mPos.y () + tabHeight + 2, mSize.x (), mSize.y ());

      nvgBeginPath (ctx);
      nvgStrokeWidth (ctx, 1.0f);
      nvgRoundedRect (ctx, mPos.x () + 0.5f, mPos.y () + tabHeight + 1.5f, mSize.x () - 1,
        mSize.y () - tabHeight - 2, mTheme->mButtonCornerRadius);
      nvgStrokeColor (ctx, mTheme->mBorderLight);
      nvgStroke (ctx);

      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, mPos.x () + 0.5f, mPos.y () + tabHeight + 0.5f, mSize.x () - 1,
        mSize.y () - tabHeight - 2, mTheme->mButtonCornerRadius);
      nvgStrokeColor (ctx, mTheme->mBorderDark);
      nvgStroke (ctx);
      nvgRestore (ctx);
    }

    Widget::draw (ctx);
  }

private:
  TabHeader* mHeader;
  StackedWidget* mContent;
  std::function<void (int)> mCallback;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
