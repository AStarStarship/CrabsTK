/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_widget.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_WIDGET
#define KABUKI_TOOLKIT_AV_WIDGET

#include "t_controlmatrix.h"
#include "t_component.h"
#include "t_controlmidi.h"

namespace _ {

/* A TWidget or instance of a widget.
@todo Convert from stupid linked list to ASCII.

A TWidget is defined by Google as "an application, or a component of an
interface, that enables a user to perform a function or access a service." A
TWidget is an array of TControlMatrix pointers with a currently selected
TControlMatrix.

Each MIDI widget, mixer, ect has a MIDI implementation map and sets of pages
of controls. For example, when the user switches from the mixer to an
instrument, the controls need to change. An TWidget the the saved settings.
Each widget has a default TWidget, and each Macro has an TWidget instance that
the user might edit.

Each TWidget will also store the resolution of the pots. Some widgets will
accept 14-bit MIDI values.

# Saving and Loading TWidget(s):
*/
class SDK TWidget : public Operation {
 public:
  /* A list of all the different types of Human-Machine Input types. */
  enum Types { Touchscreen = 0, Keyboard, DrumPad, Controller, DMXWidget };

  /* Default contrctor. */
  TWidget(const CH1* name = "")
    : HmiComponent (initLabel), page_count_ (1) {
    // We have to have at least one blank page.
    pages.add (ControlsPage ());
  }

  /* Copy constructor. */
  TWidget(const TWidget& o)
    : WidgetName (o.WidgetName), page_count_ (1) {
    SI4 i;

    for (i = 0; i < size (); ++i) {
      InsertPage (D[i]);
      current = current->next ();
    }
  }

  /* Destructor. */
  ~TWidget() { Reset (); }

  /* Deep copies given widgen. */
  TWidget& operator=(const TWidget& o) {
    Reset ();

    if (d.pagesHead == nullptr) {
      InsertPage ("Untitled");
      return;
    }
    InsertPage (new WidgetPage (*d.pagesHead));

    WidgetPage *PsHead = d.pagesHead, *current = PsHead->getNext ();

    while (current != PsHead) {
      InsertPage (new WidgetPage (*current));
      current = current->next ();
    }
  }

  /* Compairs this TWidget to the given one.
  @return Gets true if the two objects are the identical. */
  SI4 Compare(const TWidget& o) {
    if (WidgetName.Compare (o.name_) ||
      page_count_ != o.page_count_)
      return false;

    if (page_count_ <
      1)  // then both have 0 pages because we already copaired page_count_
      return true;

    // pagesHead should not be equal to nullptr here becasue we have 1 page

    if (!pagesHead->compare (*o.pagesHead)) return false;

    WidgetPage *a = pagesHead->next (), *b = o.pagesHead->next ();

    while (a != pagesHead)  // WARNING!!!: The way this is code COULD cause a
                            // bug... probably wont.
    {
      if (!a->compare (*b)) return false;

      a = a->Next ();
      b = b->Next ();
    }

    return true;
  }

  /* Deletes all of the ControlPages. */
  void Reset() {
    if (page_count_ <= 1) {
      if (page_count_ == 0) return;

      delete pagesHead;
      pagesHead = nullptr;
      return;
    }

    WidgetPage *getPrevious () = pagesHead->getNext (),
      *current = GetPrevious->getNext ();

    while (current != pagesHead) {
      delete GetPrevious;
      GetPrevious = current;
      current = current->getNext ();
    }

    delete GetPrevious;
    delete pagesHead;
    pagesHead = nullptr;
    page_count_ = 0;
  }

  /* Gets this TWidget's name. */
  TStrand<>& Name() { return name_; }

  /* Renames this TWidget to the given string. */
  const CH1* SetName(const TStrand<>& new_name) {
    return name_.Set (new_name);
  }

  /* Gets what type TWidget this is.
  This is the index of this TWidget's mode. */
  SI4 GetType();

  /* Gets the number of pages in the Array. */
  SI4 GetNumPages() { return page_count_; }

  /* Gets true if this TWidget contans a page with a name that matches the search
  string. */
  BOL ContainsPage(const CH1* s) {
    if (pagesHead == nullptr) return false;

    WidgetPage* current = pagesHead;
    do {
      if (thisPageLabel.Compare (current->Label ())) return true;

      current = current->Next ();
    } while (current != pagesHead);

    return false;
  }

  /* Inserts a new blank page into the Array.
  @param s The name for the new page. */
  SI4 InsertPage(const CH1* s) {  // This function inserts a new TWidget page in d
    TStrand<> trimmed_name;

    if (name.Compare (""))
      trimmed_name = T ("Untitled");
    else
      trimmed_name = trimmed_name.trim ();

    // We need search through the list to see if the list contains a page with
    // this name. If it does exit, we check to see if there is a number at the
    // end. If there is a number, then we increment it. If there isn't, then we
    // put one there.

    WidgetPage* current = pagesHead;

    do {
      if (!current->Label ().Compare (
        trimmed_name)) { // The page name already exists
        return InsertDuplicate (trimmed_name);
      }

      current = current->next ();
    } while (current != pagesHead);

    // Didn't find the trimmed. This shouldn't happen.

    return InsertPage (new WidgetPage (trimmed_name));
  }
  
  /* Inserts and deep copies an already existing page into the Array.
  @param newPage The page to deep copy. */
  SI4 InsertPage(const TControlMatrix& page) {
    if (contains (P->Label ()))  { // See if page already exists
      InsertDuplicate (P->Label ());
      page (P->Label ())->copy (page.Label ());
      return 1;
    }
    return InsertPage (new WidgetPage (*P));
  }

  /* Deletes Page at thisIndex from this TWidget. */
  SI4 DeletePage(SI4 index) {
    if (index > page_count_ || index < 1) return 0;
    WidgetPage* currentPage = pagesHead;
    for (SI4 i = 0; i <= pageIndex; ++i) currentPage = currentPage->next ();
    DeletePage (currentPage);
    return true;
  }

  /* Delete page with entitled by the given string. */
  SI4 DeletePage(const CH1* key) {
    // Because our client application stores a copy of the TWidget to edit,
    // and because there can't only be one page with any give name, we only
    // want the client to be able to delete by a given name.

    // Basically, we want to scan through the linked list of pages and find the
    // page with thisName and pass it to the private DeletePage (WidgetPage *)
    // function

    if (pagesHead == nullptr)  { // Redundant error checking
      // We have to have at least one page in a TWidget
      InsertPage ("Untitled");
      return false;
    }

    WidgetPage* current = pagesHead;

    do {
      if (label.Compare (current->Label ())) {
        DeletePage (current);
        return true;
      }

      current = current->next ();
    } while (current != pagesHead);

    // else, the TWidget does not exist in the list
    return false;
  }

  /* Gets a pointer to the first Page in the TWidget. */
  TControlMatrix* FirstPage() {
    if (Size () > 0) return this[0];
    return nullptr;
  }

  /* Gets the page with this name. */
  TControlMatrix* GetPage(const CH1* name) {
    if (pagesHead == nullptr) return nullptr;

    WidgetPage* current = pagesHead;

    do {
      if (name.Compare (current->Label ())) return current;
      current = current->next ();
    } while (current != pagesHead);

    return nullptr;
  }

  /* Sets the filename of this TWidget to the newFilename. */
  const CH1* SetFile (const CH1* new_name) {
    return filename_.Set (new_name);
  }

  /* Gets the filename of this TWidget. */
  const TStrand<>& GetFilename () { return filename_; }

  /* Prints the page_labels_ to the given Printer. */
  template<typename Printer>
  Printer& PrintPageLables (Printer& o) {
    TStrand<> pageLables;
    WidgetPage* current = pagesHead;
    o << "Pages: ";
    if (current) {
      o << current->Label ();
      for (uint32_t i = 0; i < 5; ++i) {
        o << " -> ";
        current = current->next ();
        if (current == pagesHead) break;
        o << current->Label ();
      }
    } else {
      o << "No Pages\n";
    }
    return o;
  }

  /* Prints this object to a Printer. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    const CH1* namesFormater = " -> ";
    o << "\nWidget: " << WidgetName <<  << " PageCount: " << page_count_ 
      << "\n" << StringLine ('-');
    if (page_count_ == 0) {
      o << "No Pages\n";
      return o;
    }
    o << pagesHead->Label ();
    current = pagesHead->next ();
    while (current != pagesHead) {
      o << " -> " << current->Label ();
      current = current->next ();
    }
    return o;
  }

  /* Script2 operations. */
  virtual const Op* Star (CHW index, Expr* io);

 private:
  TStrand<> widget_name_,       //< This TWidget's name.
      filename_;                //< Filename of this widget.
  SI4 num_pages_;               //< Number of pages this TWidget has.
  Parameter<SI4>*masterKnob_a_, //< Master knob for Layer A.
      *master_knob_b_,          //< Master knob for Layer B.
      *master_bttn_a_,          //< Master button for Layer A.
      *master_bttn_b_;          //< Master button for Layer B.
  TArray<TControlMatrix> matrixs_;  //< Control grids.

  /* Deletes the given page from the TWidget. */
  void DeletePage(WidgetPage& page) {
    // First check to see if there is 0 or 1 pages
    if (page_count_ <= 1) {
      if (page_count_ == 0)  { // this shouldn't happen
        InsertPage ("Untitled");
        return;
      }
      // We have to have at least one page
      pagesHead->copy (untitledPage);
      return;
    }

    if (page == pagesHead) pagesHead = pagesHead->next ();

    page->getPrev ()->setNextPage (page->next ());
    page->next ()->setPrevPage (page->getPrev ());
    delete page;

    --page_count_;
  }

  /* Inserts the given page into the list. */
  SI4 InsertPage (TControlMatrix page) {
    return matrixs_.Insert (page)
  }

  /* Inserts a page that has been tested for duplicate names. */
  SI4 InsertDuplicate(const CH1* page_key) {
    // If there is a duplicate name already, we want to do 2 things...
    // 1.) check to see if there is a trailing value at the end. If there is a
    // value,
    //     we strip it of the value form the end of the strin and then try to
    //     insert it with without it, then we increment the value and try to
    //     insert it. If The value is less than zero, then it will start the
    //     process over from 1.

    SI4 trailingValue;
    TStrand<> name;

    trailingValue = name.GetTrailingIntValue ();

    if (trailingValue < 0) trailingValue = 1;

    name = name.dropLastCharacters (TStrand<> (trailingValue).length ()) +
      TStrand<> (++trailingValue);
    return InsertDuplicate (name);
  }
};

}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_WIDGET
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
