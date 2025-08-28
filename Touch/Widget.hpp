// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_WIDGET
#define KABUKITOOLKIT_TOUCH_WIDGET
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#include "ControlMatrix.hpp"
#include "Component.hpp"
#include "ControlMidi.hpp"
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
class LIB_MEMBER TWidget : public Operation {
 public:
  /* A list of all the different types of Human-Machine Input types. */
  enum Types { Touchscreen = 0, Keyboard, DrumPad, Controller, DMXWidget };

  /* Default contrctor. */
  TWidget(const CHA* name = "")
    : HMIComponent (initLabel), page_count_ (1) {
    // We have to have at least one blank page.
    pages.add (ControlsPage ());
  }

  /* Copy constructor. */
  TWidget(const TWidget& o)
    : WidgetName (o.WidgetName), page_count_ (1) {
    ISC i;

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
  ISC Compare(const TWidget& o) {
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
  TString<>& Name() { return name_; }

  /* Renames this TWidget to the given AString. */
  const CHA* SetName(const TString<>& new_name) {
    return name_.Set (new_name);
  }

  /* Gets what type TWidget this is.
  This is the index of this TWidget's mode. */
  ISC GetType();

  /* Gets the number of pages in the Array. */
  ISC GetNumPages() { return page_count_; }

  /* Gets true if this TWidget contans a page with a name that matches the search
  AString. */
  BOL ContainsPage(const CHA* s) {
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
  ISC InsertPage(const CHA* s) {  // This function inserts a new TWidget page in d
    TString<> trimmed_name;

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
  ISC InsertPage(const TControlMatrix& page) {
    if (contains (P->Label ()))  { // See if page already exists
      InsertDuplicate (P->Label ());
      page (P->Label ())->copy (page.Label ());
      return 1;
    }
    return InsertPage (new WidgetPage (*P));
  }

  /* Deletes Page at thisIndex from this TWidget. */
  ISC DeletePage(ISC index) {
    if (index > page_count_ || index < 1) return 0;
    WidgetPage* currentPage = pagesHead;
    for (ISC i = 0; i <= pageIndex; ++i) currentPage = currentPage->next ();
    DeletePage (currentPage);
    return true;
  }

  /* Delete page with entitled by the given AString. */
  ISC DeletePage(const CHA* key) {
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
  TControlMatrix* GetPage(const CHA* name) {
    if (pagesHead == nullptr) return nullptr;

    WidgetPage* current = pagesHead;

    do {
      if (name.Compare (current->Label ())) return current;
      current = current->next ();
    } while (current != pagesHead);

    return nullptr;
  }

  /* Sets the filename of this TWidget to the newFilename. */
  const CHA* SetFile (const CHA* new_name) {
    return filename_.Set (new_name);
  }

  /* Gets the filename of this TWidget. */
  const TString<>& GetFilename () { return filename_; }

  /* Prints the page_labels_ to the given Printer. */
  template<typename Printer>
  Printer& PrintPageLables (Printer& o) {
    TString<> pageLables;
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
    const CHA* namesFormater = " -> ";
    o << "\nWidget: " << WidgetName <<  << " PageCount: " << page_count_ 
      << "\n" << LineString ('-');
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
  TString<> widget_name_,       //< This TWidget's name.
      filename_;                //< Filename of this widget.
  ISC num_pages_;               //< Number of pages this TWidget has.
  Parameter<ISC>*masterKnob_a_, //< Master knob for Layer A.
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
  ISC InsertPage (TControlMatrix page) {
    return matrixs_.Insert (page)
  }

  /* Inserts a page that has been tested for duplicate names. */
  ISC InsertDuplicate(const CHA* page_key) {
    // If there is a duplicate name already, we want to do 2 things...
    // 1.) check to see if there is a trailing value at the end. If there is a
    // value,
    //     we strip it of the value form the end of the strin and then try to
    //     insert it with without it, then we increment the value and try to
    //     insert it. If The value is less than zero, then it will start the
    //     process over from 1.

    ISC trailingValue;
    TString<> name;

    trailingValue = name.GetTrailingIntValue ();

    if (trailingValue < 0) trailingValue = 1;

    name = name.dropLastCharacters (TString<> (trailingValue).length ()) +
      TString<> (++trailingValue);
    return InsertDuplicate (name);
  }
};

}  //< namespace _
#endif
#endif
