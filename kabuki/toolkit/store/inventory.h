/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/store/inventory.h
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

#ifndef HEADER_FOR_KABUKI_STORE_INVENTORY
#define HEADER_FOR_KABUKI_STORE_INVENTORY

#include "cart.h"

namespace _ {

/** A set of Items.
    
*/
class Inventory : public _::Op {
    public:

    enum {
        kDefaultSize       = 32,  //< The init size of the list.
        kMaxStringLength   = 32,  //< The name length of a name.
        kLexicalDeilimiter = '.', //< Delimiter for identifying nested objects.
    };

    /** Default constructor creates a list of kDefaultSize. */
    Inventory (const char* category);

    /** Destructs the dynamic memory. */
    ~Inventory ();

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

    /** Gets the category. */
    const char* GetCategory ();

    /** Gets the category. */
    const char* SetCategory (const char* new_name);

    /** Calculates the total cost of the list. */
    double GetTotalCost ();

    /** Sums the quantities. */
    int64_t GetTotalQuantity ();

    /** Prints the list to the console. */
    _::Text& Out (_::Text& txt);

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
    virtual const _::Op* Star (uint index, _::Expr* expr);

    private:

    char                  * category_;   //< Category string.
    std::vector<Inventory*> inventories_;//< Vector of nested Inventory(s).
    std::vector<Item*>      items_;      //< Vector of Item(s).
    
};      //< class Inventory
}       //< namespace _
#endif  //< HEADER_FOR_KABUKI_STORE_INVENTORY
