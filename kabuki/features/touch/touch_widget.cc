/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_widget.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "component.h"
#include "control_matrix.h"
#include "control_midi.h"
#include "widget.h"

namespace _ {

Widget::Widget(const CH1* initLabel) : HmiComponent(initLabel), numPages(1) {
  // We have to have at least one blank page.
  pages.add(ControlsPage());
}

Widget::Widget(const File& initLabel) {}

Widget::Widget(const Widget& o) : WidgetName(o.WidgetName), numPages(1) {
  SI4 i;

  for (i = 0; i < size(); ++i) {
    insertPage(D[i]);
    current = current->next();
  }
}

Widget::~Widget() { reset(); }

void Widget::reset() {
  if (numPages <= 1) {
    if (numPages == 0) return;

    delete pagesHead;
    pagesHead = nullptr;
    return;
  }

  WidgetPage *getPrevious() = pagesHead->getNext(),
             *current = GetPrevious->getNext();

  while (current != pagesHead) {
    delete GetPrevious;
    GetPrevious = current;
    current = current->getNext();
  }

  delete GetPrevious;
  delete pagesHead;
  pagesHead = nullptr;
  numPages = 0;
}

Widget& Widget::operator=(const Widget& d) {
  reset();

  if (d.pagesHead == nullptr) {
    insertPage("Untitled");
    return;
  }
  insertPage(new WidgetPage(*d.pagesHead));

  WidgetPage *PsHead = d.pagesHead, *current = PsHead->getNext();

  while (current != PsHead) {
    insertPage(new WidgetPage(*current));
    current = current->next();
  }
}

const CH1* Widget::GetName() { return WidgetName; }

BOL Widget::GetName(const CH1* name) {  // Renames this Widget to name

  // To do: Perform input checking on name for rules (what rules?)

  widgetName = String(name);

  return true;
}

SI4 Widget::GetNumPages() { return numPages; }

BOL Widget::contains(const CH1* thisPageLabel) {
  if (pagesHead == nullptr) return false;

  WidgetPage* current = pagesHead;
  do {
    if (thisPageLabel.compare(current->getLabel())) return true;

    current = current->GetNext();
  } while (current != pagesHead);

  return false;
}

SI4 Widget::insertPage(WidgetPage* P) {
  if (contains(P->getLabel()))  // See if page already exists
  {
    insertDuplicate(P->getLabel());
    page(P->getLabel())->copy(*P);
    return 1;
  }
  return insertPage(new WidgetPage(*P));
}

SI4 Widget::insertPage(WidgetPage* P) {
  if (P == nullptr)  // Can't add a nullptr page
    return 0;

  if (pagesHead == nullptr)  // This shouldn't happen
  {
    pagesHead = P;

    P->setPrevPage(P);
    P->setNextPage(P);

    ++numPages;

    return 1;
  }
  // Inserts P at end of list.

  P->setNextPage(pagesHead);
  P->setPrevPage(pagesHead->getPrev());

  pagesHead->getPrev()->setNextPage(P);
  pagesHead->setPrevPage(P);

  ++numPages;

  return 1;
}

SI4 Widget::insertDuplicate(const CH1* name) {
  // If there is a duplicate name already, we want to do 2 things...
  // 1.) check to see if there is a trailing value at the end. If there is a
  // value,
  //     we strip it of the value form the end of the strin and then try to
  //     insert it with without it, then we increment the value and try to
  //     insert it. If The value is less than zero, then it will start the
  //     process over from 1.

  SI4 trailingValue;
  String name;

  trailingValue = name.getTrailingIntValue();

  if (trailingValue < 0) trailingValue = 1;

  name = name.dropLastCharacters(String(trailingValue).length()) +
         String(++trailingValue);
  return insertDuplicate(name);
}

SI4 Widget::insertPage(
    const CH1* name) {  // This function inserts a new Widget page in d

  String trimmedNewPageName;

  if (name.compare(""))
    trimmedNewPageName = T("Untitled");
  else
    trimmedNewPageName = trimmedNewPageName.trim();

  // We need search through the list to see if the list contains a page with
  // this name. If it does exit, we check to see if there is a number at the
  // end. If there is a number, then we increment it. If there isn't, then we
  // put one there.

  WidgetPage* current = pagesHead;

  do {
    if (!current->getLabel().compare(
            trimmedNewPageName))  // The page name already exists
    {
      return insertDuplicate(trimmedNewPageName);
    }

    current = current->next();
  } while (current != pagesHead);

  // Didn't find the trimmed. This shouldn't happen.

  return insertPage(new WidgetPage(trimmedNewPageName));
}

WidgetPage* Widget::firstPage() {
  if (size() > 0) return this[0];
  return nullptr;
}

WidgetPage* Widget::page(const CH1* name) {
  if (pagesHead == nullptr) return nullptr;

  WidgetPage* current = pagesHead;

  do {
    if (name.compare(current->getLabel())) return current;
    current = current->next();
  } while (current != pagesHead);

  return nullptr;
}

SI4 Widget::deletePage(uint32_t pageIndex) {
  uint32_t i;

  if (pageIndex > numPages || pageIndex < 1) return 0;

  WidgetPage* currentPage = pagesHead;

  for (i = 0; i <= pageIndex; ++i) currentPage = currentPage->next();

  deletePage(currentPage);
  return true;
}

SI4 Widget::deletePage(const CH1* thisLabel) {
  // Because our client application stores a copy of the Widget to edit,
  // and because there can't only be one page with any give name, we only
  // want the client to be able to delete by a given name.

  // Basically, we want to scan through the linked list of pages and find the
  // page with thisName and pass it to the private deletePage (WidgetPage *)
  // function

  if (pagesHead == nullptr)  // Redundant error checking
  {
    // We have to have at least one page in a Widget
    insertPage("Untitled");
    return false;
  }

  WidgetPage* current = pagesHead;

  do {
    if (thisLabel.compare(current->getLabel())) {
      deletePage(current);
      return true;
    }

    current = current->next();
  } while (current != pagesHead);

  // else, the Widget does not exist in the list
  return false;
}

void Widget::deletePage(WidgetPage* page) {
  // First check to see if there is 0 or 1 pages
  if (numPages <= 1) {
    if (numPages == 0)  // this shouldn't happen
    {
      insertPage("Untitled");
      return;
    }

    // We have to have at least one page
    pagesHead->copy(untitledPage);
    return;
  }

  if (page == pagesHead) pagesHead = pagesHead->next();

  page->getPrev()->setNextPage(page->next());
  page->next()->setPrevPage(page->getPrev());
  delete page;

  --numPages;
}

SI4 Widget::compare(const Widget& thatWidget) {
  if (WidgetName.compare(thatWidget.WidgetName) ||
      numPages != thatWidget.numPages)
    return false;

  if (numPages <
      1)  // then both have 0 pages because we already copaired numPages
    return true;

  // pagesHead should not be equal to nullptr here becasue we have 1 page

  if (!pagesHead->compare(*thatWidget.pagesHead)) return false;

  WidgetPage *a = pagesHead->next(), *b = thatWidget.pagesHead->next();

  while (a != pagesHead)  // WARNING!!!: The way this is code COULD cause a
                          // bug... probably wont.
  {
    if (!a->compare(*b)) return false;

    a = a->GetNext();
    b = b->GetNext();
  }

  return true;
}

const CH1* Widget::GetInfo() {
#ifdef __TESTING_UNIT__controls__
  return debugString;
#else
  return T("Some sort of info about this class.");
#endif
}
SI4 Widget::infoValue() { return debugValue; }

String Widget::printPageLables() {
  String pageLables;
  WidgetPage* current = pagesHead;

  pageLables << "Pages: ";

  if (current) {
    pageLables << current->getLabel();

    for (uint32_t i = 0; i < 5; ++i) {
      pageLables << " -> ";

      current = current->next();

      if (current == pagesHead) break;

      pageLables << current->getLabel();
    }
  } else {
    pageLables << "No Pages\n";
  }

  return pageLables;
}

const CH1* Widget::toString() {
  const string namesFormater = " -> ";

  string stringRep = String("Widget: ");

  stringRep << WidgetName << "\n";

  stringRep << "numPages (): " << numPages << "\n" << dashedLnBreak;

  if (numPages == 0) {
    stringRep << "No Pages\n";
    return stringRep;
  }

  stringRep << pagesHead->getLabel();

  current = pagesHead->next();

  while (current != pagesHead) {
    stringRep << " -> " << current->getLabel();
    current = current->next();
  }

  stringRep << "\n";

  return stringRep;
}

void Widget::importWidget(const File& thisFile) {}

void Widget::exportWidget(const File& thisFile) {}

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
