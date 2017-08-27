/** CS 162 Program 4-5
    @file   id_server.h
    @author Cale McCollough
    @brief  Contains the IdServer class.
*/

#ifndef KABUKI_PRO_IDSERVER
#define KABUKI_PRO_IDSERVER

#include <array.h>
#include <utils.h>

namespace _pro {

/** Manages user keys and assigns unique ids to events. */
class IdServer
{
    public:
    
    enum {
        MAX_KEY_LENGTH = 32, //< The max key length.
    };

    /** Constructs an empty server. */    
    IdServer ();

    /** Destructor. */
    ~IdServer ();

    /** Registers an unique key with the server without cloning the name. */
    int RegisterKey (char* key);

    /** Registers an unique key the server and clones the name string. */
    int RegisterKey (const char* key);

    /** Searches for the given key.
        @return Returns a pointer to a key string upon success and null
                if the key does not exist. */
    int Find (const char* key);

    /** Gets the number of events. */
    int NumEvents ();
 
    /** Registers a new Event UID by incrementing and returning the num_events. */
    int RegisterEvent ();
    
    /** Prints this object to the console. */
    void print ();
    
    private:

    int num_events_;            //< The global number of event created.
    _data::Array<char*> ids_;   //< A the id keys.
};        //< Array class
}         //< namespace _pro
#endif    //< KABUKI_PRO_IDSERVER

