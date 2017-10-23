/** kabuki::pro
    @file    ~/source/kabuki/id/include/server.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef KABUKI_PRO_IDSERVER
#define KABUKI_PRO_IDSERVER

#include "../include/id_config.h"

namespace _pro {

/** Manages user keys and assigns unique ids to events. */
class IdServer
{
    public:
    
    enum {
        MAX_KEY_LENGTH = 32, //< The max key length.
    };

    /** Creates an empty server. */    
    IdServer ();

    /** Destructor. */
    ~IdServer ();

    /** Registers an unique key with the server without cloning the name. */
    int RegisterKey (char* key);

    /** Registers an unique key the server and clones the name char. */
    int RegisterKey (const char* key);

    /** Searches for the given key.
        @return Returns a pointer to a key char upon success and null
                if the key does not exist. */
    int Find (const char* key);

    /** Gets the number of events. */
    int NumEvents ();
 
    /** Registers a new Event UID by incrementing and returning the num_events. */
    int RegisterEvent ();
    
    /** Prints this object to the console. */
    void Print (_::Log& log);
    
    private:

    int num_events_;            //< The global number of event created.
    _::Array<char*, int> ids_;  //< A the id keys.
};        //< Array class
}         //< namespace _pro
#endif    //< KABUKI_PRO_IDSERVER

