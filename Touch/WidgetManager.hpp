// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_WIDGETPAGE
#define KABUKITOOLKIT_TOUCH_WIDGETPAGE
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
#include "widget.hpp";
class LIB_MEMBER ControlsPage;
class LIB_MEMBER Controller;

namespace _ {

/*  The WidgetManager is an editor for templates
The Template editor has one Template at a time loaded along with one page
that it is editing.

There is one main global instance of the WidgetManager. Through the GUI, a
user can edit only one parameter on either ControlLayer through a drop
down/up menu on either the Top or bottom of the screen. The separation of LIB_MEMBER
from GUI allows the GUI to be programmed much easier by simply having them
wrap the functions to the LIB_MEMBER. Because of this separation, there is not going
to be any checking to see if a template has been saved in before applying any
changes that this classes functions might make.

# Templates File Format:
The data for the Template file will be in the same order as the data
declarations in this object followed by a packed array of the ControlsPage.
*/
class LIB_MEMBER WidgetManager {
 public:
  /* Default constructor. */
  WidgetManager(Controller* controller = nullptr);

  /* Default destructor. */
  ~WidgetManager();

  /* Resets the Editor to nullptr currentTemplate and currentPage. */
  void ResetEditor();

  /* Checks to see if the current template is saved. */
  ISC CheckIfTemplateIsSaved();

  /* Checks to see if the current page is saved. */
  ISC CheckIfPageIsSaved();

  /* Saves the current TemplatePage being edited.
      @warning Does not compare to previous state! */
  void SaveCurrentPage();

  /* Saves the current Template being edited
      @warning Does not compare to previous state! */
  void saveCurrentTemplate();

  /* Loads an Template into the editors currentTempalte. */
  ISC LoadTemplate(Template* new_template);

  /* Loads an TemplatePage into the editors currentPage. */
  ISC LoadPage(ControlsPage* thisPage);

  /* Creates a new template. */
  void NewTemplate();

  /* Creates a new Template with the name. */
  void NewTemplate(const CHA* name);

  /* Saves the current template. */
  void SaveTemplate();

  /* Renames the current page being edited. */
  void RenamePage(const CHA* name);

  /* Renames the current template being edited. */
  void RenameTemplate(const CHA* name);

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
  inline void print(Script::Expr& slot);

 private:
  IsymmetricController* controls_;  //< Isymmeric control surface.
  TWidget* template_;                //< Current template being edited.
  WidgetPage* page_;                //< Current page being edited.
};
}  //< namespace _
#endif
#endif
