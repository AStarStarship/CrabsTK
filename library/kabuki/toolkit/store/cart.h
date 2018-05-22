/** Kabuki Toolkit
    @file    ~/source/kabuki/store/cart.h
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

#ifndef HEADER_FOR_KABUKI_STORE_CART
#define HEADER_FOR_KABUKI_STORE_CART

#include "item.h"

namespace kabuki { namespace store {

/** A cart of Items.
    
*/
class Cart: public _::Op {
    public:

    enum {
        kDefaultSize       = 256, //< Default size of the list.
        kMaxStringLength   = 32,  //< Max string length.
        kLexicalDeilimiter = '.'  //< Delimiter for identifying nested objects.
    };

    /** Default constructor creates a cart with max_items and given name. */
    Cart (const char* name, int max_items = kDefaultSize);

    /** Destructs the dynamic memory. */
    ~Cart ();

    /** Clears the cart. */
    void Clear ();

    /** Gets the count of Item(s). */
    int Length ();

    /** Adds a new item to the list and adds and updates quantity and cost.
        If the item is new, the item is added with the given information. If the
        Item is already exists in the collection, it will add the new quantity to
        the Item. If the cost is different, then the cost will be updated with
        the new cost.
        @return Returns nil upon success and a pointer to an error string upon
                failure. */
    virtual const char* Add (uid_t uid, const char* name, int64_t quantity,
                             double cost);

    /** Calculates the total cost of the list. */
    double GetTotalCost ();

    /** Sums the quantities. */
    int64_t GetTotalQuantity ();

    /** Gets the name_. */
    const char* GetName ();

    /** Gets the name_. */
    const char* SetName (const char* new_name);

    /** Prints this object to the printer. */
    _::Printer&& Print (_::Printer&& printer = _::Printer&());

    /** Parses Script Text Commands.
        @param text     Beginning of the Text buffer.
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an
        error_t ticket upon Read-Write failure. */
    virtual const _::Op* Star (wchar_t index, _::Expr* expr);

    private:

    char             * name_;  //< Cart name string.
    std::vector<Item*> items_; //< Item(s) in the cart.
    
};      //< class Cart
}       //< namespace store
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_STORE_CART
