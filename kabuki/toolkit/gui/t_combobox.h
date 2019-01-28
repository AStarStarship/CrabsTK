/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/toolkit/gui/combobox.h
 *
 * @brief Simple combo box widget based on a popup button.
 */

#pragma once
#include <pch.h>


#include "t_popupbutton.h"

namespace _ {

/* Simple combo box widget based on a popup button. */
class SDK ComboBox : public PopupButton {
public:
    // Create an empty combo box
    ComboBox(Widget *parent)
      : PopupButton (parent), mSelectedIndex (0) {
    }

    // Create a new combo box with the given items
    ComboBox(Widget *parent, const std::vector<std::string> &items)
      : PopupButton (parent), mSelectedIndex (0) {
      setItems (items);
    }

    /* Create a new combo box with the given items, providing both short and
    long descriptive labels for each item. */
    ComboBox(Widget *parent, const std::vector<std::string> &items,
             const std::vector<std::string> &itemsShort)
      : PopupButton (parent), mSelectedIndex (0) {
      setItems (items, itemsShort);
    }

    // The callback to execute for this ComboBox.
    std::function<void(int)> callback() const { return mCallback; }

    // Sets the callback to execute for this ComboBox.
    void setCallback(const std::function<void(int)> &callback) { mCallback = callback; }

    // The current index this ComboBox has selected.
    int selectedIndex() const { return mSelectedIndex; }

    // Sets the current index this ComboBox has selected.
    void setSelectedIndex(int idx) {
      if (mItemsShort.empty ())
        return;
      const std::vector<Widget *> &children = popup ()->children ();
      ((Button *)children[mSelectedIndex])->setPushed (false);
      ((Button *)children[idx])->setPushed (true);
      mSelectedIndex = idx;
      setCaption (mItemsShort[idx]);
    }

    // Sets the items for this ComboBox, providing both short and long descriptive lables for each item.
    void setItems(const std::vector<std::string> &items,
                  const std::vector<std::string> &itemsShort) {
      DASSERT (items.size () == itemsShort.size ());
      mItems = items;
      mItemsShort = itemsShort;
      if (mSelectedIndex < 0 || mSelectedIndex >= (int)items.size ())
        mSelectedIndex = 0;
      while (mPopup->childCount () != 0)
        mPopup->removeChild (mPopup->childCount () - 1);
      mPopup->setLayout (new GroupLayout (10));
      int index = 0;
      for (const auto &str : items) {
        Button *button = new Button (mPopup, str);
        button->setFlags (Button::RadioButton);
        button->setCallback ([&, index] {
          mSelectedIndex = index;
          setCaption (mItemsShort[index]);
          setPushed (false);
          popup ()->setVisible (false);
          if (mCallback)
            mCallback (index);
        });
        index++;
      }
      setSelectedIndex (mSelectedIndex);
    }

    // Sets the items for this ComboBox.
    void setItems(const std::vector<std::string> &items) { setItems(items, items); }

    // The items associated with this ComboBox.
    const std::vector<std::string> &items() const { return mItems; }

    // The short descriptions associated with this ComboBox.
    const std::vector<std::string> &itemsShort() const { return mItemsShort; }

    // Handles mouse scrolling events for this ComboBox.
    virtual bool scrollEvent(const Vector2i &p, const Vector2f &rel) override {
      if (rel.y () < 0) {
        setSelectedIndex (std::min (mSelectedIndex + 1, (int)(items ().size () - 1)));
        if (mCallback)
          mCallback (mSelectedIndex);
        return true;
      } else if (rel.y () > 0) {
        setSelectedIndex (std::max (mSelectedIndex - 1, 0));
        if (mCallback)
          mCallback (mSelectedIndex);
        return true;
      }
      return Widget::scrollEvent (p, rel);
    }

    // Saves the state of this ComboBox to the specified Serializer.
    virtual void save(Serializer &s) const override {
      Widget::save (s);
      s.set ("items", mItems);
      s.set ("itemsShort", mItemsShort);
      s.set ("selectedIndex", mSelectedIndex);
    }

    // Sets the state of this ComboBox from the specified Serializer.
    virtual bool load(Serializer &s) override {
      if (!Widget::load (s)) return false;
      if (!s.get ("items", mItems)) return false;
      if (!s.get ("itemsShort", mItemsShort)) return false;
      if (!s.get ("selectedIndex", mSelectedIndex)) return false;
      return true;
    }

protected:
    // The items associated with this ComboBox.
    std::vector<std::string> mItems;

    // The short descriptions of items associated with this ComboBox.
    std::vector<std::string> mItemsShort;

    // The callback for this ComboBox.
    std::function<void(int)> mCallback;

    // The current index this ComboBox has selected.
    int mSelectedIndex;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
