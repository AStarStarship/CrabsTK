/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /project/store/citem.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#ifndef HEADER_FOR_KABUKI_STORE_ITEM
#define HEADER_FOR_KABUKI_STORE_ITEM

#include "config.h"

namespace _ {

/** An item in a Cart.
    To identify items two unique ids are needed, a local uid and a manufacturer
    uid.
*/
class Item : public _::Op {
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
    double GetTotalCost () const;

    /** Prints this object to the text. */
    virtual _::Printer&& Out (_::Printer&& printer = _::Printer& ());
    
    /** Handles Script Commands.
        @param text     Beginning of the Text buffer.
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an
        error_t ticket upon Read-Write failure. */
    virtual const _::Op* Star (uint index, _::Expr* expr);

    private:

    char  * name_,       //< Item name.
          * description_,//< Item description.
          * supply_uid_; //< Supplier unique id.
    uid_t   uid_;        //< Store unique id.
    int64_t quantity_;   //< Quantity to purchase.
    double  cost_;       //< Item cost in generic units.
};

}       //< namespace _
#endif  //< HEADER_FOR_KABUKI_STORE_ITEM
