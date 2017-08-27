/** CS 162 Program 3: Shopping List
    @author Cale McCollough
    @brief  File contains the ItemList class.
*/

#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <fstream>

namespace _pro {

/** An item in a ShoppingList. */
struct Item {
    char* name;     //< The Item name.
    int quantity;   //< The quantity to purchase.
    float price;    //< The price of the item in generic units.

    /** Constructs an Item from the given arguments. */
    Item (char* name, int quantity, float price);

    /** Destructor. */
    ~Item ();
};

/** Calculates the total cost of the Item's quantity times price. */
float ItemTotal (Item* item);

class ItemList {
    public:

    enum {
        INIT_MAX_ITEMS = 32,   //< The init size of the list.
        MAX_STRING_LENGTH = 32    //< The name length of a name.
    };

    /** Default constructor creates a list of INIT_SIZE. */
    ItemList (char* category);

    /** Destructs the dynamic memory. */
    ~ItemList ();

    /** Adds a new item to the list and adds and updates quantity and price.
        If the item is new, the item is added with the given information. If the
        Item is already exists in the collection, it will add the new quantity to
        the Item. If the price is different, then the price will be updated with
        the new price.
        @return Returns nil upon success and a pointer to an error string upon failure. */
    const char* AddItem (char* name, int quantity, float price);

    /** Calculates the total price of the list. */
    float GetTotal ();

    /** Sums the quantities. */
    int SumQuantities ();

    /** Gets the category. */
    char* GetCategory ();

    /** Writes to an open file.
        @param ofstream The Out-File Stream.
        @pre   ofstream must have been opened. */
    void WriteToOpenFile (std::ofstream& file);

    /** Prints the list to the console. */
    void Print ();

    private:

    char* category;	//< The category string.
    int max_items,      //< The max number of Item(s) in the array.
        num_items;	//< The number of Item(s) in the list.
    Item** items;       //< Pointer to an array of Item pointers.
};
}       //< namespace _pro
#endif  //< ITEMLIST_H
