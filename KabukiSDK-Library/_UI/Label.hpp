/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/Label.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <KabukiSDK-Config.hpp>

namespace _UI {

/** An object that has a text label.
    This class _KabukiSDK has a primary label, and a short label.
    A Label has the following rules:
    1.) Labels must be 32 characters or less for a normal label, and 16 characters or less for a short label.
    2.) Labels must contain printable ASCII characters that are HUI compatible.
*/ 
class _KabukiSDK Label
{   
    public:

    enum {
        MaxLabelLength = 20,       //< The max label length of a label
        NonAlphaNumChars                       //< The number of non-alpha-numberic charactors allowed in a label.
    };

    static string AllowedSymbols,         //< The non-alpha-numberic charactors allowed in a label.
        UnnamedLabel;                           //< string that reads "Unnnamed".
    
    Label (string newLabel = "");
    //< Default constructor.
    
    Label (const Label& other);
    //< Copy contructor.
    
    virtual ~Label () {}
    //< Virtual destructor.

    string getLabel () const;
    //< gets the label.
    
    int setLabel (string newLabel);
    /*< Sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too long.
        @see    Label::isValid (char). */
    virtual int setLabel (const Label& newLabel);
    /*< Sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too long.
        @see    Label::isValid (char). */
        
    int Compare (string other) const;
    /*< Compares this label to the other string.
        @return gets  0 if they are identical and1 if the other string is too long. */
        
    int Compare (const Label& other) const;
    /*< Compares this label to the other Label.
        @return gets  0 if they are identical and1 if the other string  is too long. */
        
    string print (Terminal& slot) const;
    //< gets a text represenation of this object.
 
    static int isValid (string label);
    /*< Verifies if thisLabel is a valid label.
        @return gets 0 if the label is valid.
        @return gets 1 if the label is too long.
        @return gets -1 thisLabel is "".
    */

    private:

    string label;                               //< This object's text label.

};

}   //< namespace _UI


