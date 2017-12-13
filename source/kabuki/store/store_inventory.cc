/** Kabuki Toolkit
    @file    ~/source/kabuki/store/store_inventory.cc
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

#include "inventory.h"

using namespace _;
using namespace std;

namespace kabuki { namespace store {

Inventory::Inventory (const char* category) :
    category_  (StrandClone (category)),
    items_     () {
}

Inventory::~Inventory () {
    // We used some dynamic memory for the name strings.
    int count = GetCount ();
    if (count == 0) {
        return;
    }
    for (int i = GetCount () - 1; i > 0; --i) {
        delete items_[i];
    }
    delete[] category_;
}

int Inventory::GetCount () {
    return items_.size ();
}

const char* Inventory::Add (uid_t uid, const char* name, int64_t quantity,
                           double cost) {
    if (!name) {
        return "name can't be nil";
    }
    // Search for the item.
    for (int i = GetCount () - 1; i > 0; --i) {
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

double Inventory::GetCostTotal () {
    double total = 0.0;
    int count = GetCount ();
    if (!count) {
        return total;
    }
    for (; count > 0; --count) {
        Item* item = items_[count];
        total += ((double)item->GetQuantity ()) * item->GetCost ();
    }
    return total;
}

char* Inventory::GetCategory () {
    return category_;
}

int64_t Inventory::GetQuantityTotal () {
    int64_t quanity = 0;
    for (int i = GetCount () - 1; i > 0; --i) {
        quanity += items_[i]->GetQuantity ();
    }
    return quanity;
}

/*
void Inventory::WriteToOpenFile (std::ofstream& file) {
    if (category_[0] != '$')
        file << '$';
    file << category_ << "," << GetCount () << ",\n";
    for (int i = 0; i < GetCount (); ++i) {
        Item* item = items_[i];
        file << item->GetName () << ", " << item->GetQuantity () << ", "
             << item->GetPrice () << " \n";
    }
}*/

void Inventory::Print () {
    enum { kSize = 32 };
    double quantity,   //< Temp var.
           price;      //< Temp var.
    char   buffer[kSize]; //< Temp buffer.

    PrintCentered (category_, 20);

    TextWrite (buffer, buffer + kSize, GetQuantityTotal ());
    PrintCentered (buffer, 20);
    PrintCentered ("", 20);
    TextWrite (buffer, buffer + kSize, GetCostTotal ());
    PrintCentered (buffer, 20, true);
    PrintLines (4);
    for (int i = 0; i < GetCount (); ++i) {
        Item* item = items_[i];
        PrintCentered (item->GetName (), 20);
        quantity = (double)item->GetQuantity ();
        price = item->GetCost ();
        TextWrite (buffer, buffer + kSize, item->GetQuantity ());
        PrintCentered (buffer, 20);
        TextWrite (buffer, buffer + kSize, price);
        PrintCentered (buffer, 20);
        TextWrite (buffer, buffer + kSize, quantity * price);
        PrintCentered (buffer, 20, true);
    }
    PrintBreak ("|", '-', 0);
}

const char* Inventory::Do (const char* text, const char* text_end) {
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    const char* next_token;
    if (*text == kLexicalDeilimiter) {
        for (int i = GetCount () - 1; i > 0; --i) {
            Inventory* inventory = inventories_[i];
            next_token = TokenEquals (text, text_end,
                                      inventory->GetCategory ());
            if (next_token) {
                return inventory->Do (next_token + 1, text_end);
            }
        }
    }
    for (int i = GetCount () - 1; i > 0; --i) {
        Item* item = items_[i];
        next_token = TokenEquals (text, text_end,
                                  item->GetName ());
        if (next_token) {
            return item->Do (next_token + 1, text_end);
        }
    }
    return "couldn't find item or inventory";
}

const Operation* Inventory::Star (uint index, Expression* expr) {
    static const Operation This = { "Inventory",
        OperationCount (0), OperationFirst ('A'),
        "", 0 };
    switch (index) {
        case '?': return ExpressionOperand (expr, &This);
        case 'A': {
        
        }
    }
    return nullptr;
}

}   //< namespace store
}   //< namespace kabuki
