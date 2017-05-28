/** Kabuki Theater
    @file    /.../Source/_HMI/WiidgetManager.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include "KabukiTheater-Config.hpp"

#include "Widget.hpp";
class _KabukiTheater_ ControlsPage;
class _KabukiTheater_ Controller;

namespace _HMI {

class _KabukiTheater_ WidgetManager
/*  The WidgetManager is an editor for templates
    The Template editor has one Template at a time loaded along with one page that it is editing.

    There is one main global instance of the WidgetManager. Through the GUI, a user can edit only one
    parameter on either ControlLayer through a drop down/up menu on either the top or bottom of the screen.
    This seperation of API from GUI allows the GUI to be programmed much easier by simply having them wrap
    the functions to the API. Because of this speration, there is not going to be any checking to see if a
    template has been saved in before applying any changes that this classes functions might make.

    # Templates File Format:
    The data for the Template file will be in the same order as the data declorations in this object followed
    by a packed array of the ControlsPage.
*/
{
    public:

    WidgetManager (Controller* controller = nullptr);
    /*< Default constructor. */

    ~WidgetManager ();
    /*< Default deconstructor. */

    void resetEditor ();
    /*< Resets the Editor to nullptr currentTemplate and currentPage. */

    int checkIfTemplateIsSaved ();
    /*< Checks to see if the current template is saved. */
    
    int checkIfPageIsSaved ();
    /*< Checks to see if the current page is saved. */

    void saveCurrentPage ();
    /*< Saves the current TemplatePage being edited.
        Warning: Does not compair to previous state! */
        
    void saveCurrentTemplate ();
    /*< Saves the current Template being edited
        Warning: Does not compair to previous state! */

    int loadTemplate (Template *thisTemplate);
    /*< Loads an Template into the editors currentTempalte. */
    
    int loadPage (ControlsPage *thisPage);
    /*< Loads an TemplatePage into the editors currentPage. */

    void newTemplate ();
    /*< Creates a new template. */
    
    void newTemplate (const char* initName);
    /*< Creates a new Template with the initName. */
    
    void saveTemplate ();
    /*< Saves the current template. */

    void renamePage (const char* newName);
    /*< Renames the current page being edited. */
    
    void renameTemplate (const char* newName);
    /*< Renames the current template being edited. */

    void removePage ();
    /*< Removes the current page being edited. */

    void removeTemplate ();
    /*< Removes the current template being edited. */

    void importTemplate (const File& f);
    /*< Imports a template from the given file. */
    
    void exportTemplate (const File& f);
    /*< Exports the current Template to the given file. */

    void undo ();
    /*< Undos the previous action. */
    
    void redo ();
    /*< Redos the previous action. */
    
    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:
    
    IsymmetricController* controls;     //< The iSymmeric control surface.

    Widget* currentTemplate;            //< The current template being edited.
    WidgetPage* currentPage;            //< The current page being edited.
};
}   //< _HMI }   //<Theater

