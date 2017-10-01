/** Kabuki Theater
    @file    ~/Source/_hmi/Widget.h
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

#include "ControlMatrix.h"

namespace _hmi {

class _HMI_API Widget : public _::IDevice
/*< A Widget or instance of a widget.
    A Widget is defined by Google as "an application, or a component of an interface, that 
    enables a user to perform a function or access a service."
    A Widget is an array of ControlMatrix pointers with a currently selected ControlMatrix.

    Each MIDI widget, mixer, ect has a MIDI implementation map and sets of pages of controls.
    For example, when the user switches from the mixer to an instrument, the controls need to
    change. An Widget the the saved settings. Each widget has a default Widget, and
    each Macro has an Widget instance that the user might edit.

    Each Widget will also store the resolution of the pots. Some widgets will accept 14-bit
    MIDI values.

    # Saving and Loading Widget(s):
*/
{
    public:
    
    enum Types
    /*< A list of all the different types of Human-Machine Input types. */
    { 
        Touchscreen = 0, 
        Keyboard, 
        DrumPad, 
        Controller, 
        DMXWidget 
    };

    Widget (const char* newName = "");
    /*< Default contrctor. */
    
    Widget (const Widget& w);
    /*< Copy constructor. */
    
    void loadFromFile (const File& f);
    /*< Constructor loads a Widget from the filename. */
    
    ~Widget ();
    /*< Destructor. */

    Widget& operator= (const Widget& w);
    /*< Deep copies given widgen. */

    bool operator== (const File& f);
    /*< Checks to see if this filename is equal to thatFilename and returns non-zero if they are the same file. */

    int compare (const Widget& w);
    /*< Compairs this Widget to the given one.
        @return Gets true if the two objects are the identical. */
        
    void reset ();
    /*< Deletes all of the ControlPages. */

    const char* getName ();
    /*< Gets this Widget's name. */
    
    const char* setName (const char* s);
    /*< Renames this Widget to the given string. */

    int getType ();
    /*< Gets what type Widget this is.
        This is the index of this Widget's mode. */
        
    int getNumPages ();
    /*< Gets the number of pages in the Array. */

    bool containsPage (const char* s);
    /*< Gets true if this Widget contans a page with a name that matches the search string. */

    int insertPage (const char* s);
    /*< Inserts a new blank page into the Array.
        @param s The name for the new page. */

    int insertPage (const ControlMatrix& newPage);
    /*< Inserts and deep copies an already existing page into the Array.
        @param newPage The page to deep copy. */
        
    int deletePage (int index);
    /*< Deletes Page at thisIndex from this Widget. */
    
    int deletePage (const char* s);
    /*< Delete page with entitled by the given string. */

    ControlMatrix* firstPage ();
    /*< Gets a pointer to the first Page in the Widget. */
    
    ControlMatrix* getPage (const char* pageName);
    /*< Gets the page with this pageName. */

    const char* printPageLabels ();
    /*< Gets a const char* of the Page Labels. */

    void setFile (const File& newFilename);
    /*< Sets the filename of this Widget to the newFilename. */
    
    const char* getFilename ();
    /*< Gets the filename of this Widget. */

    void loadFromFile (const File& f);
    /*< Imports a Widget from thisFile. */
    
    void saveToFile (const File& f);
    /*< Exports a Widget to thisFile. */
    
    const _::Member* op (_::Terminal* io, byte index) override;
    /*< I2P subroutines. */
    
    inline void print() const;
    /*< Prints this object to a terminal. */

    private:

    const char* widgetName,                 //< This Widget's name.
        * filename;                         //< The filename of this widget.
    int numPages;                           //< The number of pages this Widget has.
    Parameter<int>* masterKnobA,            //< The master knob for Layer A.
        * masterKnobB,                      //< The master knob for Layer B.
        * masterBttnA,                      //< The master button for Layer A.
        * masterBttnB;                      //< The master button for Layer B.
    std::vector<ControlMatrix*> controls;   //< The control grids.

    void deletePage (WidgetPage& thisPage);
    /*< Deletes thisPage from the Widget. */
    
    int insertPage (ControlMatrix& newPage);
    /*< Inserts newPage into the list. */

    int insertDuplicate (const char*newPageName);
    /*< Inserts a page that has been tested for duplicate names. */
};

}   //< _hmi
}   //< _Theater
