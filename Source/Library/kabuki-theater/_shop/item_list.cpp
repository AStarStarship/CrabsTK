/** CS 162 Program 3: ItemList class
    @file   item_list.cpp
    @author Cale McCollough
    @brief  File contains the ItemList class.
*/

#include "item_list.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string.h>

namespace _pro {

Item::Item (char* name, int quantity, float price):
    name (name),
    quantity (quantity),
    price (price) { }

Item::~Item () {
    delete name;
}

float ItemTotal (Item* item) {
    if (item == nullptr)
        return 0.0f;
    return ((float)item->quantity) * item->price;
}

ItemList::ItemList (char* category):
    category (category),
    max_items (INIT_MAX_ITEMS),
    num_items (0),
    items (new Item*[INIT_MAX_ITEMS]) {
    if (category == nullptr) {
        category = new char[3];
        category[0] = '$';
        category[1] = '\n';
        category[2] = 0;
    }
    for (int i = 0; i < INIT_MAX_ITEMS; ++i)
        items[i] = nullptr;
}

ItemList::~ItemList () {
    // We used some dynamic memory for the name strings.
    for (int i = 0; i < num_items; ++i)
        delete items[i];
    delete[] items;
    delete[] category;
}

const char* ItemList::AddItem (char* name, int quantity, float price) {
    if (name == nullptr)
        return "Null name";
    // Search for the item.
    for (int i = 0; i < num_items; ++i) {
        Item* current = items[i];
        if (strcmp (name, current->name) == 0) {
      // It's a duplicate so update the quantity and price.
            current->quantity += quantity;
            if (price != 0.0f)
                current->price = price;
            i = num_items;    //< Set loop counter to exit condition.
        }
    }
    // Add an Item.
    if (num_items + 1 >= max_items)
        return "list full";
    items[num_items++] = new Item (name, quantity, price);
}

float ItemList::GetTotal () {
    float total = 0.0f;
    for (int i = 0; i < num_items; ++i) {
        Item* item = items[i];
        total += ((float)item->quantity) * item->price;
    }
    return total;
}

char* ItemList::GetCategory () {
    return category;
}

int ItemList::SumQuantities () {
    int sum = 0;
    for (int i = 0; i < num_items; ++i)
        sum += items[i]->quantity;
    return sum;
}

void ItemList::WriteToOpenFile (std::ofstream& file) {
    if (category[0] != '$')
        file << '$';
    file << category << "," << num_items << ",\n";
    for (int i = 0; i < num_items; ++i) {
        Item* item = items[i];
        file << item->name << ", " << item->quantity << ", " << item->price << " \n";
    }
}

void ItemList::Print () {
    float quantity,  //< Temp var.
        price;     //< Temp var.
    char buffer[32]; //< Temp buffer.

    PrintCentered (category, 20);

    sprintf (buffer, "%i", SumQuantities ());
    PrintCentered (buffer, 20);
    PrintCentered ("", 20);
    sprintf (buffer, "%f", GetTotal ());
    PrintCentered (buffer, 20, true);
    PrintLines (4);
    for (int i = 0; i < num_items; ++i) {
        Item* item = items[i];
        PrintCentered (item->name, 20);
        quantity = (float)item->quantity;
        price = item->price;
        sprintf (buffer, "%i", item->quantity);
        PrintCentered (buffer, 20);
        sprintf (buffer, "%f", price);
        PrintCentered (buffer, 20);
        sprintf (buffer, "%f", quantity * price);
        PrintCentered (buffer, 20, true);
    }
    PrintBreak ("|", '-', 0);
}

}   //< namespace _pro
