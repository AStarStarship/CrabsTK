/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/WidgetPage.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <KabukiTheater-Config.hpp>
#include "Label.hpp"

namespace _Theater/HMI {

class MIDIControl;
class Button;
class ControlMatrix;

class _KabukiTheater_ WidgetPage
/*< Class that represents a page of controls in a Widget.
    A WidgetPage is composed of multiple groups of controls.
*/
{
    public:

    enum {
        MinControlPairs = 6,               //< The minimum number of control pairs allowed per Page.
        MaxControlPairs = 16,                           //< The maximum number of control pairs allowed per Page.
    };

    static const char* TypeText;                       //< The const char* "Page".

    /** Default constructor. */
    WidgetPage  (const char* initName = "", int initNumControlPairs = 0);
    WidgetPage  (const WidgetPage& thisPage);            //< Copy constuctor.
    ~WidgetPage ();                                     //< Destructor.

    WidgetPage& operator=  (const WidgetPage& other);    //< C++ operator= overlaoder.

    int numControlPairs ();                             //< Gets the num_control_pairs.

    /** Gets a pointer to the specified groupNumber.
        @return Gets nullptr if the groupNumber is invalid. */
    ControlMatrix* getControlGroup  (int groupNumber);

    /** Gets the knob at the specified index.
        @return Gets nullptr if thisIndex is greater than the num_control_pairs. */
    MIDIControl* getKnob  (int index);

    /** Gets the button at the specified index.
        @return Gets 0 thisIndex is greater than the num_control_pairs. */
    Button* getButton  (int index);

    const char* getLabel ();
    //< Gets the pageLabel.
    
    void setLabel  (const char* newLabel);
    //< Sets the pageLabel to the newLabel.

    int compare  (const WidgetPage& thatPage);
    /*< Compares this control to thatControl.
        @return Gets true if this control and thatControl are identical. */

    bool masterControlsEnabled ();
    //< Gets whether the master knob and button are enabled/disabled.
    
    void enableMasterControls ();
    //< Enables the master controls.
    
    void disableMasterControls ();
    //< Disables Master Controls.

    bool hasButtons ();
    //< Gets true if this page of controls has Button  (s)

    const char* getType ();
    //< Gets typeText.
    
    void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    int numControlPairs;                    //< The number of BoundedControl/Button pairs.

    bool mstrControlsEnabled;               //< Stores if knob 9 is page specific or is the master controls.

    std::vector<MIDIControl*> knobs;        //< The knob controls.
    std::vector<Button*> bttns;             //< The button controls.

    std::vector<ControlMatrix*> controlGroup; //< An array of pointers to ControlGroup objects.
};

#ifdef DEBUG
/** Unit test for the WidgetPage class _KabukiTheater_. */
class _KabukiTheater_ WidgetPageTests: public UnitTest
{
    public:

    WidgetPageTests ()
        : UnitTest ("Testing Controls::WidgetPage")
    {
        // Nothing to do here!
    }

    void runTest ()
    {
        logMessage ("Testing Page class\n" + ConsoleLine ('~'));

        WidgetPage testPage ("Test Page");

        beginTest ("Testing const char* ToString ()");
        logMessage  (testPage.ToString ());

        logMessage ("Testing copy constructor...");
        WidgetPage testPageCopy  (testPage);
        logMessage  (testPageCopy.ToString ());

        logMessage ("Done testing Page class");
    }
};
static WidgetPageTests widgetPageTests ();
#endif // DEBUG
}   //< namespace _Theater/HMI

