// InterprocessExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>

using namespace boost::interprocess;

enum {
    kRoomSize = 1024,       //< The test room size.
    kSlotSize = 128,        //< The test slot size.
};

int main (int argc, char *argv[]) {
    if (argc == 1) 
    {
        // Parent process
        // Create a native windows shared memory object.
        windows_shared_memory shm (create_only, "MySharedMemory", read_write, 
            1024);

        // Map the whole shared memory in this process.
        mapped_region region (shm, read_write);

        // Write all the memory to 1.
        std::memset (region.get_address (), 1, region.get_size ());

        // Launch child process.
        std::string s (argv[0]); s += " child ";
        if (0 != std::system (s.c_str ()))
            return 1;
        // windows_shared_memory is destroyed when the last attached process 
        // dies...
    }
    else
    {
        // Open already created shared memory object.
        windows_shared_memory shm (open_only, "MySharedMemory", read_only);

        // Map the whole shared memory in this process.
        mapped_region region (shm, read_only);

        // Check that memory was initialized to 1.
        char *mem = static_cast<char*> (region.get_address ());
        for (std::size_t i = 0; i < region.get_size (); ++i)
            if (*mem++ != 1)
                return 1;       // Error checking memory
        return 0;
    }
    return 0;
}
