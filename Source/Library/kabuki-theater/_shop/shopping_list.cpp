/** CS 162 Program 3
    @file   shopping_list.cpp
    @author Cale McCollough
    @brief  Contains a generic shopping list that can read and write in comma
            separate values file.
*/

#include "utils.h"
#include "shopping_list.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

namespace _pro {

ShoppingList::ShoppingList (char* filename):
    filename (filename),
    max_lists (INIT_MAX_LISTS),
    num_lists (0),
    lists (new ItemList*[INIT_MAX_LISTS]) {
    ItemList** list = lists;
    // Set the unused ListItem pointers to nullptr.
    for (int i = 0; i < INIT_MAX_LISTS; ++i)
        list[i] = nullptr;
}

ShoppingList::ShoppingList (const char* filename):
    filename (CloneString (filename)),
    num_lists (0),
    max_lists (INIT_MAX_LISTS) {
    ItemList** list = new ItemList*[INIT_MAX_LISTS];
    lists = list;
    // Set the unused ListItem pointers to nullptr.
    for (int i = 0; i < INIT_MAX_LISTS; ++i)
        list[i] = nullptr;
}

ShoppingList::~ShoppingList () {
    delete filename;
    for (int i = 0; i < num_lists; ++i)
        delete lists[i];
    delete[] lists;
}

char* ShoppingList::GetFilename () {
    return filename;
}

bool ShoppingList::Add (char* category, char* name, int quantity, float price) {
    //std::cout << "| Adding ";
    if (category == nullptr)
        return false;
    if (quantity < 1)
        return false;
    if (price < 0.0f)
        return false;
    if (num_lists >= max_lists)
        return false;
     //std::cout << quantity << " of " << name << " at $" << price
     //          << " each to category: " << category << '\n';
    ItemList*  current;
    // Search for an already existing category to add it to:
    for (int i = 0; i < num_lists; ++i) {
        current = lists[i];
        if (!strcmp (category, current->GetCategory ())) {
            if (!current->AddItem (name, quantity, price)) {
                std::cout << "Error adding: " << name << '\n';
                return false;
            }
            return true;
        }
    }
    // Create a new category.
    current = new ItemList (category);
    current->AddItem (name, quantity, price);
    lists[num_lists++] = current;
    return true;
}

bool ShoppingList::Add (const char* category, const char* name, int quantity, float price) {
    return Add (CloneString (category), CloneString (name), quantity, price);
}

float ShoppingList::GetTotal () {
    float total = 0.0f;
    ItemList** list = lists;
    for (int i = 0; i < num_lists; ++i)
        total += list[i]->GetTotal ();
    return total;
}

void ShoppingList::Load () {

    const char* error;  //< Error flag.
    std::cout << "\n\n| Reading from " << filename << '\n';
    std::ifstream file (filename);
    if (!file.is_open ()) {
        std::cout << "Unable to open file for reading";
        return;
    }
    char buffer[MAX_STRING_LENGTH];
    file.getline (buffer, MAX_STRING_LENGTH, ',');   //< Throw away till the comma..
    int num_categories;
    file >> num_categories;
    //std::cout << num_categories << " categories.\n";
    file.getline (buffer, MAX_STRING_LENGTH, '\n');  //< Throw away the rest of the line.
    // Read each of the ItemList categories.
    for (int i = 0; i < num_categories; ++i) {
        char* category = new char[MAX_STRING_LENGTH];
        file.getline (category, MAX_STRING_LENGTH, ',');
        //std::cout << "| category: " << category;
        int num_items;
        file >> num_items;
        //std::cout << " num_items: " << num_items << '\n';
        while (file.get () != '\n');  //< Throw away the rest of the line.
        for (int i = 0; i < num_items; ++i) {
            char* name = new char[MAX_STRING_LENGTH];
            file.getline (name, MAX_STRING_LENGTH, ',');
            int quantity;
            float price;
            char c;
            //file >> quantity >> c >> price >> c;
            file >> quantity;
            while (file.get () != ',');
            file >> price;
            while (file.get () != '\n');
            Add (category, name, quantity, price);
        }
    }
    file.close ();
}

void ShoppingList::Save () {
    std::cout << "\n\n| Writing to " << filename << "...\n";
    const char* error;
    std::ofstream file;
    file.open (filename);
    if (!file.is_open ()) {
        std::cout << "Unable top open file for writing";
        return;
    }
    file << "Shopping List," << num_lists << ',' << GetTotal () << '\n';
    ItemList** list = lists;
    for (int i = 0; i < num_lists; ++i)
        list[i]->WriteToOpenFile (file);
    file.close ();
}

void ShoppingList::Print () {
    PrintBreak ();
    std::cout << "| Shopping List: " << filename;
    PrintBreak ("|");
    PrintCentered ("Name", 20);
    PrintCentered ("Quantity", 20);
    PrintCentered ("Price", 20);
    PrintCentered ("Total", 20, true);
    PrintColumnBreak (4);
    ItemList** list = lists;
    for (int i = 0; i < num_lists; ++i) {
        list[i]->Print ();
    }
    std::cout << "| Total: " << GetTotal ();
    PrintBreak ("|");
}

}   //< namespace _pro