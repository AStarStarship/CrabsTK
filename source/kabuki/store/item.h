/** Kabuki Toolkit
    @file    ~/source/kabuki/store/item.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_KABUKI_STORE_ITEM
#define HEADER_FOR_KABUKI_STORE_ITEM

#include "config.h"

namespace kabuki { namespace store {

/** An item in a Cart.
    To identify items two unique ids are needed, a local uid and a manufacturer
    uid.
*/
class Item : public _::Operand {
    public:

    /** Constructs an Item from the given arguments. */
    Item (uid_t uid, const char* name, double price = 0.0,
          int64_t quantity = 0);

    /** Destructor. */
    virtual ~Item ();

    /** Gets the name_. */
    const char* GetName () const;

    /** Sets the name_.
        @param  new_name The new name string to clone.
        @return Nil upon success and an error string upon failure. */
    virtual const char* SetName (const char* new_name);

    /** Gets the description_. */
    const char* GetDescription () const;

    /** Sets the description_.
        @param  new_description A description string to clone.
        @return Nil upon success and an error string upon failure. */
    virtual const char* SetDescription (const char* new_description);

    /** Gets the supplier uid. */
    const char* GetSupplyUid () const;

    /** Sets the supplier uid.
        @param  new_suid A supplier uid string to clone.
        @return Nil upon success and an error string upon failure. */
    virtual const char* SetSupplyUid (const char* new_suid);

    /** Gets the unique id. */
    uid_t GetUid () const;

    /** Sets the unique uid.
        @param  new_suid A supplier uid string to clone.
        @return Nil upon success and an error string upon failure. */
    virtual const char* SetUid (uid_t value);

    /** Gets the quantity_. */
    int64_t GetQuantity () const;

    /** Attempts to add the given value to the quantity_.
        @param value The value to add.
        @return Nil upon success or an error string upon failure. */
    virtual const char* AddQuantity (int64_t value);

    /** Sets the cost_.
        @param  value New quantity.
        @return Nil upon success or an error string upon failure. */
    virtual const char* SetQuantity (int64_t value);

    /** Gets the cost_. */
    double GetCost () const;

    /** Sets the cost_.
        @param  value Cost value to add.
        @return Nil upon success or an error string upon failure. */
    virtual const char* AddCost (double value);

    /** Sets the cost_.
        @param  value New cost value.
        @return Nil upon success or an error string upon failure. */
    virtual const char* SetCost (double value);

    /** Calculates the total cost of the Item's quantity times price. */
    double GetCostTotal () const;

    /** Prints this object to the text. */
    virtual _::Text& Print (_::Text& txt);

    /** Prints this object to the text. */
    virtual void Print ();
    
    /** Handles Script Commands.
        @param text     Beginning of the Text buffer.
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Do (const char* text, const char* text_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an
        error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    private:

    char  * name_,       //< Item name.
          * description_,//< Item description.
          * supply_uid_; //< Supplier unique id.
    uid_t   uid_;        //< Store unique id.
    int64_t quantity_;   //< Quantity to purchase.
    double  cost_;       //< Item cost in generic units.
};

}       //< namespace store
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_STORE_ITEM
