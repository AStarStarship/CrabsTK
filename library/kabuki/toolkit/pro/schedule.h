/** kabuki::pro
    @version 0.x
    @file    ~/libraries/pro/schedule.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef KABUKI_PRO_SCHEDULE_H
#define KABUKI_PRO_SCHEDULE_H

#include "../../data/array.h"
#include "task.h"

using namespace kt;

namespace kt { namespace pro {

/** A group of Task(string) in temporal order that may repeat.
    A Schedule contains a collection of Task objects. It has a starting date, 
    and it can repeat x number of times. Multiple Schedule(string) can be combined 
    to create a Calendar. Schedules are designed to minimize the human time  
    to insert and manipulate the calendar.
    
    The average person will have no more than a handful of Tasks on any given
    day. Given 100 years in a life and 365 days in a year, 5*100*365 < 200K
    Tasks in a normal person's lifetime. Maybe 1M Task(string) tops per lifetime.
    
    Where does the Calendar live?
    The calendar lives in the cloud in an SQL database.
    
    How is it loaded?
    It is loaded chunks at a time.
    
    What happens when you make edits?
    The local object is edited, changes staged, and edit's to only single 
    tasks are written to the server.

    What about colliding events?
    Colliding events are stored in a linear lined list of collisions.

    When does speed matter?
    When the users has to sit there. When would this happen? Using a very 
    large array and inserting into it. That could be a buzz kill. Other than
    that, optimize for developer time. Memory footprint doesn't matter.
    
    
*/
class Schedule
{
    public:
    
    /** Default constructor creates a schedule with the default number of Tasks 
        pointers. */
    Schedule (const char* key = "", int num_repeats = 0, int color = ~0);
    
    /** Destructs the dynamic memory. */
    ~Schedule ();
    
    /** Gets the name. */
    const char* GetKey ();

    /** Set the name to the new_name. */
    void StealKey (char* new_name);

    /** Sets the name to the new name. */
    void SetKey (const char* new_name);

    /** Gets the readme. */
    const char* GetReadme ();

    /** Set the name to the new_readme. */
    void StealReadMe (char* new_readme);

    /** Sets the name to the new name.
        @param new_readme A char this object now owns the memory.  */
    void SetReadMe (const char* new_readme);
    
    /** Gets the default schedule color of the child-tasks. */
    int GetColor ();
    
    /** Sets the default schedule color. */
    void SetColor (int new_color);
    
    /** Add's a new Task to the schedule. */
    void Add (Task* task);
    
    /** Removes and deletes the Task at the given index.
        @param  index The index to remove.
        @return Returns false if the index is out of bounds. */
    bool Remove (int index);
    
    /** Searches for an event with the given time and creates one if it doesn't
        exist.
        @return Returns a pointer to an Event that starts at the given time, and
                 a new  Task if no containing Event(string) match the event_time. */
    Task* Find (time_t event_time);
    
    /** Gets the help menu. */
    static const char* GetHelpString ();
     
    /** Prints the Schedule to the console. */
    void Out (int indentation = 0);
    
    /** Parse char input, possibly from the keyboard.
        @param input The (possibly keyboard) input from the user.
        @param stack The stack of Project objects.
        @return Returns null upon success and an error char upon failure. */
    const char* Command (char* input);
     
    private:
    
    char* key_,             //< Name of the schedule.
        * readme_;          //< Readme for the Schedule.
    int color_,             //< Initial color of all of the Tasks.
        num_repeats_;       //< Number of times the Schedule repeats.
    _::Array<Task*> tasks_; //< Array of Task pointers.
};
}       //< namespace pro
}       //< namespace toolkit}       //< namespace kabuki
#endif  //< KABUKI_PRO_SCHEDULE_H

