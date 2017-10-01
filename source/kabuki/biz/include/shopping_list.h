/** CS 162 Program 3: Shopping List
    @file   shopping_list.h
    @author Cale McCollough
    @brief  Contains a generic shopping list that can read and write in comma
            separate values file.
*/

#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include "item_list.h"

namespace _pro {

/** A shopping list that stores items in categories and reads and writes to disk.
*/
class ShoppingList {
    public:

    enum {
        INIT_MAX_LISTS = 32,
        //< Init max number of lists.
        MAX_STRING_LENGTH = ItemList::MAX_STRING_LENGTH
        //< Max string length.
    };

    /** Constructor initializes list with the given file name. */
    ShoppingList (char* filename);

    /** Constructor initializes list with the given file name. */
    ShoppingList (const char* filename);

    /** Destructs the lists. */
    ~ShoppingList ();

    /** Adds the given item to the list.
        @param category The category of the Item.
        @param name The name of the Item.
        @param quantity The quantity of the Item to buy.
        @param price The price of the Item. */
    bool Add (char* category, char* name, int quantity, float price);

    /** Adds the given item to the list.
        @param category The category of the Item.
        @param name The name of the Item.
        @param quantity The quantity of the Item to buy.
        @param price The price of the Item. */
    bool Add (const char* category, const char* name, int quantity, float price);

    /** Gets the total of this list. */
    float GetTotal ();

    /** Deserializes the file with the stored file_name.
        @return Returns 0 upon success and a pointer to an error string upon failure. */
    void Load ();

    /** Serializes the list to the given file.
        @return Returns 0 upon success and a pointer to an error string upon failure. */
    void Save ();

    /** Gets the filename. */
    char* GetFilename ();

    /** Prints the Shopping list to the console. */
    void Print ();

    private:

    char* filename;     //< The filename.
    int num_lists,      //< The number of ItemList(s).
        max_lists;      //< The size of the lists array.
    ItemList** lists;   //< An array of lists of ItemList pointers.
};
}       //< namespace _pro
#endif  //< SHOPPINGLIST_H

