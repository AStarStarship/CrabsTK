/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/store/store_inventory.cc
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

#include <stdafx.h>

#include "cart.h"

namespace _ {

Cart::Cart (const char* name, int max_items) :
    name_  (StrandClone (name)),
    items_ () {
    max_items = (max_items < 1)?1:max_items;
    items_.reserve (max_items);
}

Cart::~Cart () {
    // We used some dynamic memory for the name strings.
    int count = Length ();
    if (count == 0) {
        return;
    }
    for (int i = Length () - 1; i > 0; --i) {
        delete items_[i];
    }
    delete[] name_;
}

void Cart::Clear () {
    items_.clear ();
}

int Cart::Length () {
    return items_.size ();
}

const char* Cart::GetName () {
    return name_;
}

const char* Cart::SetName (const char* new_name) {
    if (!new_name) {
        return "Cart name can't be nil";
    }
    delete name_;
    name_ = StrandClone (new_name);
    return nullptr;
}

const char* Cart::Add (uid_t uid, const char* name, int64_t quantity,
                       double cost) {
    if (!name) {
        return "name can't be nil";
    }
    // Search for the item.
    for (int i = Length () - 1; i > 0; --i) {
        Item* item = items_[i];
        if (StrandEquals (name, item->GetName ())) {
            // It's a duplicate so update the quantity and price.
            item->AddQuantity (quantity);
            if (item->GetCost () != cost) {
                item->SetCost (cost);
            }
            return nullptr;
        }
    }
    // Add an Item.
    items_.push_back (new Item (uid, name, cost, quantity));
    return nullptr;
}

double Cart::GetTotalCost () {
    double total = 0.0;
    int count = Length ();
    if (!count) {
        return total;
    }
    for (; count > 0; --count) {
        Item* item = items_[count];
        total += ((double)item->GetQuantity ()) * item->GetCost ();
    }
    return total;
}

int64_t Cart::GetTotalQuantity () {
    int64_t quanity = 0;
    for (int i = Length () - 1; i > 0; --i) {
        quanity += items_[i]->GetQuantity ();
    }
    return quanity;
}

/* void Cart::WriteToOpenFile (std::ofstream& file) {
    if (name_[0] != '$')
        file << '$';
    file << name_ << "," << GetCount () << ",\n";
    for (int i = 0; i < GetCount (); ++i) {
        Item* item = items_[i];
        file << item->GetName () << ", " << item->GetQuantity () << ", "
             << item->GetPrice () << " \n";
    }
}*/

Text& Cart::Print (Text& txt) {
    enum { kSize = 32 };
    double quantity,      //< Temp var.
           price;         //< Temp var.
    char   buffer[kSize]; //< Temp buffer.

    txt << txt.Centered (name_, 20) << GetTotalQuantity ()
        << txt.Centered (buffer, 20)
        << txt.Centered ("", 20)
        << txt << GetTotalCost ()
        << txt.Centered (buffer, 20, true)
        << txt.Lines (4);
    for (int i = 0; i < Length (); ++i) {
        Item* item = items_[i];
        quantity = (double)item->GetQuantity ();
        price = item->GetCost ();
        txt << txt.Centered (item->GetName (), 20)
            << item->GetQuantity ()
            << txt.Centered (buffer, 20)
            << txt << price
            << txt.Centered (buffer, 20)
            << txt << price
            << txt.Centered (buffer, 20, true);
    }
    return txt.Break ("|", '-', 0);
}

const char* Cart::Sudo (const char* strand, const char* strand_end) {
    const char* token;
    if (!(token = TextSkipSpaces (strand, strand_end))) {
        return nullptr;
    }
    int index = -1;
    if (token = TextRead (strand, strand_end, index)) {
        if (index < 0) {
            return nullptr;
        }
        if (index >= Length ()) {
            return nullptr;
        }
        return items_[index]->Sudo (token + 1, strand_end);
    }
    char c = *strand;
    if (c == kLexicalDeilimiter) {
        for (int i = Length () - 1; i > 0; --i) {
            Item* item = items_[i];
            token = TokenEquals (strand, strand_end, item->GetName ());
            if (token) {
                return item->Sudo (token + 1, strand_end);
            }
        }
    }
    return "couldn't find item or inventory";
}

const Operation* Cart::Star (uint index, Expression* expr) {
    static const Operation This = { "Cart",
        OperationCount (0), OperationFirst ('A'),
        "", 0 };
    switch (index) {
        case '?': return ExpressionOperand (expr, &This);
        case 'A': {
        
        }
    }
    return nullptr;
}

}       //< namespace _
