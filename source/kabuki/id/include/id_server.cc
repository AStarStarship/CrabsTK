/** CS 162 Program 4-5
    @file    id_server.cpp
    @authtor Cale McCollough
    @breif   Implementation of the IdServer class.
*/

#include "id_server.h"
#include <iostream>
#include <string.h>

namespace _pro {

IdServer::IdServer ():
    num_events_ (0)
{
}

IdServer::~IdServer ()
{
}

int IdServer::NumEvents ()
{
    return num_events_;
}

int IdServer::RegisterKey (char* key)
{
    if (strlen (key) > MAX_KEY_LENGTH)
        return -1;
    return ids_.Add (key);
}

int IdServer::RegisterKey (const char* key)
{
    return RegisterKey (CloneString (key));
}

int IdServer::Find (const char* key)
{
    // @todo Replace me with hash table!
    for (int i = 0; i < ids_.GetCount (); ++i)
        if (!strcmp (key, ids_.Element (i)))
            return i;
    return -1;
}

int IdServer::RegisterEvent ()
{
    return num_events_++;
}

void IdServer::print ()
{
    std::cout << "\n| Id Server:"
              << "\n| num_events: " << num_events_
              << "\n| Registered Handles: \n";
    for (int i = 0; i < ids_.GetCount (); ++i)
        std::cout << "| " << i << ": " << ids_[i] << '\n';
}

}       //< namespace _pro

