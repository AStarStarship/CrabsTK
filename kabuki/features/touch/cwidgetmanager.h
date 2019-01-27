/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_/wiidget_nanager.h
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
#ifndef HEADER_FOR_KT_HMI_WIDGETPAGE
#define HEADER_FOR_KT_HMI_WIDGETPAGE

#include "config.h"

#include "widget.h";
class SDK ControlsPage;
class SDK Controller;

namespace _ {

/*  The WidgetManager is an editor for templates
    The Template editor has one Template at a time loaded along with one page
   that it is editing.

    There is one main global instance of the WidgetManager. Through the GUI, a
   user can edit only one parameter on either ControlLayer through a drop
   down/up menu on either the top or bottom of the screen. The separation of SDK
   from GUI allows the GUI to be programmed much easier by simply having them
   wrap the functions to the SDK. Because of this separation, there is not going
   to be any checking to see if a template has been saved in before applying any
   changes that this classes functions might make.

    # Templates File Format:
    The data for the Template file will be in the same order as the data
   declarations in this object followed by a packed array of the ControlsPage.
*/
class SDK WidgetManager {
 public:
  /* Default constructor. */
  WidgetManager(Controller* controller = nullptr);

  /* Default destructor. */
  ~WidgetManager();

  /* Resets the Editor to nullptr currentTemplate and currentPage. */
  void ResetEditor();

  /* Checks to see if the current template is saved. */
  SI4 CheckIfTemplateIsSaved();

  /* Checks to see if the current page is saved. */
  SI4 CheckIfPageIsSaved();

  /* Saves the current TemplatePage being edited.
      @warning Does not compare to previous state! */
  void SaveCurrentPage();

  /* Saves the current Template being edited
      @warning Does not compare to previous state! */
  void saveCurrentTemplate();

  /* Loads an Template into the editors currentTempalte. */
  SI4 LoadTemplate(Template* new_template);

  /* Loads an TemplatePage into the editors currentPage. */
  SI4 LoadPage(ControlsPage* thisPage);

  /* Creates a new template. */
  void NewTemplate();

  /* Creates a new Template with the name. */
  void NewTemplate(const CH1* name);

  /* Saves the current template. */
  void SaveTemplate();

  /* Renames the current page being edited. */
  void RenamePage(const CH1* name);

  /* Renames the current template being edited. */
  void RenameTemplate(const CH1* name);

  /* Removes the current page being edited. */
  void RemovePage();

  /* Removes the current template being edited. */
  void RemoveTemplate();

  /* Imports a template from the given file. */
  void ImportTemplate(const File& file);

  /* Exports the current Template to the given file. */
  void ExportTemplate(const File& file);

  /* Undoes the previous action. */
  void Undo();

  /* Undoes the previous action. */
  void Redo();

  /* Prints this object to a terminal. */
  inline void print(Script::Expression& slot);

 private:
  IsymmetricController* controls_;  //< Isymmeric control surface.
  Widget* template_;                //< Current template being edited.
  WidgetPage* page_;                //< Current page being edited.
};
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_WIDGETPAGE
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
