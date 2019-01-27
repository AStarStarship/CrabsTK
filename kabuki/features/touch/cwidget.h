/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_widget.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#ifndef HEADER_FOR_KT_HMI_WIDGET
#define HEADER_FOR_KT_HMI_WIDGET

#include "control_matrix.h"

namespace _ {

/* A Widget or instance of a widget.
    A Widget is defined by Google as "an application, or a component of an
   interface, that enables a user to perform a function or access a service." A
   Widget is an array of ControlMatrix pointers with a currently selected
   ControlMatrix.

    Each MIDI widget, mixer, ect has a MIDI implementation map and sets of pages
   of controls. For example, when the user switches from the mixer to an
   instrument, the controls need to change. An Widget the the saved settings.
   Each widget has a default Widget, and each Macro has an Widget instance that
   the user might edit.

    Each Widget will also store the resolution of the pots. Some widgets will
   accept 14-bit MIDI values.

    # Saving and Loading Widget(s):
*/
class SDK Widget : public ::_::Op {
 public:
  /* A list of all the different types of Human-Machine Input types. */
  enum Types { Touchscreen = 0, Keyboard, DrumPad, Controller, DMXWidget };

  /* Default contrctor. */
  Widget(const CH1* name = "");

  /* Copy constructor. */
  Widget(const Widget& other);

  /* Constructor loads a Widget from the filename. */
  void LoadFromFile(const File& f);

  /* Destructor. */
  ~Widget();

  /* Deep copies given widgen. */
  Widget& operator=(const Widget& w);

  /* Checks to see if this filename is equal to thatFilename and returns
   * non-zero if they are the same file. */
  BOL operator==(const File& f);

  /* Compairs this Widget to the given one.
      @return Gets true if the two objects are the identical. */
  SI4 Compare(const Widget& w);

  /* Deletes all of the ControlPages. */
  void Reset();

  /* Gets this Widget's name. */
  const CH1* GetName();

  /* Renames this Widget to the given string. */
  const CH1* SetName(const CH1* s);

  /* Gets what type Widget this is.
      This is the index of this Widget's mode. */
  SI4 GetType();

  /* Gets the number of pages in the Array. */
  SI4 GetNumPages();

  /* Gets true if this Widget contans a page with a name that matches the search
   * string. */
  BOL ContainsPage(const CH1* s);

  /* Inserts a new blank page into the Array.
      @param s The name for the new page. */
  SI4 InsertPage(const CH1* s);

  /* Inserts and deep copies an already existing page into the Array.
      @param newPage The page to deep copy. */
  SI4 InsertPage(const ControlMatrix& page);

  /* Deletes Page at thisIndex from this Widget. */
  SI4 DeletePage(SI4 index);

  /* Delete page with entitled by the given string. */
  SI4 DeletePage(const CH1* key);

  /* Gets a pointer to the first Page in the Widget. */
  ControlMatrix* FirstPage();

  /* Gets the page with this name. */
  ControlMatrix* GetPage(const CH1* name);

  /* Gets a const CH1* of the Page Labels. */
  const CH1* PrintPageLabels();

  /* Sets the filename of this Widget to the newFilename. */
  void SetFile(const File& file);

  /* Gets the filename of this Widget. */
  const CH1* GetFilename();

  /* Imports a Widget from thisFile. */
  void LoadFromFile(const File& f);

  /* Exports a Widget to thisFile. */
  void SaveToFile(const File& f);

  /* Script subroutines. */
  virtual const ::_::Op* Star(wchar_t index, ::_::Expression* io);

  /* Prints this object to a terminal. */
  inline ::_::Utf& Print(_::Utf& print) const;

 private:
  const CH1 *widget_name_,            //< This Widget's name.
      *filename_;                      //< Filename of this widget.
  SI4 num_pages_;                      //< Number of pages this Widget has.
  Parameter<SI4>*masterKnob_a_,        //< Master knob for Layer A.
      *master_knob_b_,                 //< Master knob for Layer B.
      *master_bttn_a_,                 //< Master button for Layer A.
      *master_bttn_b_;                 //< Master button for Layer B.
  ::_::TArray<ControlMatrix*> controls_;  //< Control grids.

  /* Deletes the given page from the Widget. */
  void DeletePage(WidgetPage& page);

  /* Inserts the given page into the list. */
  SI4 InsertPage(ControlMatrix& page);

  /* Inserts a page that has been tested for duplicate names. */
  SI4 InsertDuplicate(const CH1* page_key);
};

}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_WIDGET
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
