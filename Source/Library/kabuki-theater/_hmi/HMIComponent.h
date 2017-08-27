/** Kabuki Theater
    @file    /.../Source/_HMI/HMIComponent.hpp
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

#include <KabukiTheater-Config.hpp>

namespace _HMI {

typedef enum {
    HMITypeMIDI = 0,
    HMITypeDMX,

} HMIComponentType;

const char* getAllowedSymbols ();
//< The non-alpha-numberic charactors allowed in a label.
void setAllowedSymbols (const char* s);
/*< Sets the allowed symbols to the given string. */

inline bool containsChar (const char* chars, char c);
/*< Checks the given char c to see if any of the chars contain it. */

inline bool containsChar (const char* chars, const char* s);
/*< Checks the given string s to see if any of the chars are in it. */

int isValidLabel (const char* label);
/*< Verifies if thisLabel is a valid label.
    @return Returns 0 if the label is valid.
    @return Returns 1 if the label is too long.
    @return Returns -1 thisLabel is "".
*/

const char* getUnnamedLabel ();
/*< All unnameed labels share the same char* pointer. */

typedef enum ParameterTypes
/** A list of the most commmon Parameter types. */
{
    DMXControl = 0,
    MIDIControl,
    OSCControl,
    DMXButton,
    MIDIButton,
    OSCButton,
    MacroButton
} ParameterType;

class _HMI_Devices_ HMIComponent
/*< A component in a Human-Machine Interface that may be referenced by name or UID.
    A HMIComponent has the following rules:
    1.) Labels must be 255 characters or less for a normal label, and 16 characters or less for a short label.
    2.) Labels must contain printable ASCII characters that are HUI compatible.
*/ 
{
    public:

    enum {
        MaxLabelLength = 20,    //< The max label length of a label.
    };
    
    HMIComponent (const char* newLabel = "");
    //< Default constructor.
    
    HMIComponent (const HMIComponent& other);
    //< Copy contructor.
    
    virtual ~HMIComponent () {}
    //< Virtual destructor.

    uid_t getUID ();
    /*< Gets the unique ID for the object. */

    const char* getHMIComponentType ();
    /*< Gets the unique string of the component type. */

    const char* getLabel () const;
    //< Gets the label.
    
    virtual int setLabel (const char* s);
    /*< Sets the label to the given string.
        @return Returns 0 upon success and 1 if the newLabel is too long.
        @see    HMIComponent::isValid (char). */
        
    int compare (const char* s) const;
    /*< Compares this label to the given string.
        @return Returns 0 if they are identical and1 if the other char* is too long. */
         
    virtual void print () const;
    /*< Prints this object to a stdout. */

    private:

    uid_t uid,              //< The unique id.
        type;               //< The type of HMI component.
    const char*label;       //< This object's text label.
};

}   //< _HMI
