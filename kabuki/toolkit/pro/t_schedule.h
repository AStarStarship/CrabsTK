/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/pro/t_schedule.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_PRO_1
#ifndef KABUKI_TOOLKIT_PRO_SCHEDULE
#define KABUKI_TOOLKIT_PRO_SCHEDULE

#include "t_task.h"

namespace _ {

/* A group of Task(string) in temporal order that may repeat.
A Schedule contains a collection of Task objects. It has a starting date,
and it can repeat x number of times. Multiple Schedule(string) can be
combined to create a Calendar. Schedules are designed to minimize the human
time to insert and manipulate the calendar.

The average person will have no more than a handful of Tasks on any given
day. Given 100 years in a life and 365 days in a year, 5*100*365 < 200K
Tasks in a normal person's lifetime. Maybe 1M Task(string) tops per
lifetime.

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
class Schedule {
 public:
  /* Default constructor creates a schedule with the default number of Tasks
      pointers. */
  Schedule(const CH1* key = "", SI4 num_repeats = 0, SI4 color = ~0)
    : key_ (StrandClone (key)),
    tasks_ (),
    color_ (color),
    num_repeats_ (num_repeats) {}

  /* Destructs the dynamic memory. */
  ~Schedule () {}

  /* Gets the name. */
  const TStrand<>& Key() { return key_; }

  /* Sets the name to the new name. */
  void SetKey (const CH1* new_name) {
    return StealKey (StrandClone (new_name));
  }

  /* Gets the readme. */
  const CH1* GetReadme() { return readme_; }

  /* Sets the name to the new name.
  @param new_readme A CH1 this object now owns the memory.  */
  void SetReadMe(const CH1* new_readme) {
    return StealReadMe (StrandClone (new_readme));
  }

  /* Gets the default schedule color of the child-tasks. */
  SI4 Color() { return color_; }

  /* Sets the default schedule color. */
  void SetColor(SI4 new_color) { color_ = new_color; }

  /* Add's a new Task to the schedule. */
  void Add(Task* task) { tasks_.Add (task); }

  /* Removes and deletes the Task at the given index.
  @return Returns false if the index is out of bounds.
  @param  index The index to remove. */
  BOL Remove(SI4 index) { return tasks_.Remove (index); }

  /* Searches for an event with the given time and creates one if it doesn't
  exist.
  @return Returns a pointer to an Event that starts at the given time, and
  a new Task if no containing Event(string) match the event_time.
   */
  Task* Find(Tss event_time) {
    // @todo Replace me with std::map!
    for (SI4 i = 0; i < tasks_.GetCount (); ++i) {
      Task* task = tasks_[i];
      if (task->Contains (event_time)) return task;
    }
    return nullptr;
  }

  /* Gets the help menu. */
  static const CH1* GetHelpString() {
    return "\n| Schedule: A (optimally)recurring collection of "
      "chronological Tasks."
      "\n| Schedule Tasks can be addressed by entering the index."
      "\n| The rest of the CH1 is then passed on to the task "
      "\n| at the given index."
      "\n|"
      "\n| Schedule Commands:"
      "\n|   -a    Add a new Task."
      "\n|         Example: -a -string \"Header\" -d \"Details\" -b 3:30"
      "\n|                  -e 4:30 -w 10.0"
      "\n|"
      "\n|   -p    Prints the Schedule."
      "\n|         Example: -p"
      "\n|"
      "\n|   -r    Removes a task by index."
      "\n|         Example: -r 2"
      "\n";
  }

  /* Prints the Schedule to the console. */
  void Out(SI4 indentation = 0);

  template<typename Printer>
  Printer& Print (Printer& o) {
    o << StringLine ('-')
      << "\nSchedule: " << key_ << " element_count: " << tasks_.GetCount ()
      << "\n";
    PrintBreak ("\n<", '-');
    for (SI4 i = 0; i < tasks_.GetCount (); ++i) {
      o << "<  " << i << ": \"" << tasks_[i]->GetHeader () << "\"\n";
    }
    PrintBreak ("<", '-');
  }

  /* Parse CH1 input, possibly from the keyboard.
  @param input The (possibly keyboard) input from the user.
  @param stack The stack of Project objects.
  @return Returns null upon success and an error CH1 upon failure. */
  const CH1* Command(CH1* input) {
    CH1 c,     //< The first CH1.
      d;      //< The second CH1.
    SI4 value;  //< An index to possibly be scanned.
    const CH1* result;
    Task* t;

    input = SkipSpaces (input);
    o << "> " << key_ << " > " << input << '\n';
    c = *input;
    if (!c) return "No Schedule arguments read";
    // o << "> Schedule ";
    if (c == '-') {
      c = *(input + 1);
      d = *(input + 2);
      // o << '-' << c << d << '\n';
      if (d && !isspace (d)) return "Invalid Schedule flag";
      input = SkipSpaces (input);
      switch (c) {
      case 'a':  // Add new Task
      {
        o << "< Adding Task: \"" << input + 3 << "\"\n";
        input = SkipSpaces (input + 2);
        c = *input;
        if (c != '-') return "You must add at least one flag";
        t = new Task ();
        if (result = t->Command (input)) return result;
        tasks_.Add (t);
        return nullptr;
      }
      case 'p':  // Print
      {
        // First try to find an integer to print.

        if (!sscanf (input, "%i", &value)) {
          Print ();
          return nullptr;
        }
        if (value < 0 || value >= tasks_.GetCount ())
          return "Invalid task index";
        tasks_[value]->Print ();
        return nullptr;
      }
      case 'r':  //< Remove
      {
        input += 3;
        // input = skip_spaces (input);
        c = *input;
        if (!isdigit (c)) return "Error scanning index to remove";
        if (!sscanf (input, "%i", &value))  //< This shouldn't happen.
          return "Error scanning index to remove";
        o << "| Removing task " << value << '\n';
        if (value > tasks_.GetCount ()) return "Invalid schedule index";
        Remove (value);
        return nullptr;
      }
      }
      return "Invalid Schedule flag argument";
    } else if (isdigit (c)) {
      // Because we checked for the -flag before this, we'll never have a
      // negative number!
      if (!sscanf (input, "%i", &value))  // This shouldn't happen.
        return "Error reading Project member index";
      if (value < tasks_.GetCount ())
        return tasks_[value]->Command (TextTokenEnd (input) + 1);
      return "Invalid Project member index";
    } else if (c == '\"') {
      return "Search feature not implemented";
    } else if (c == '?')  // Help
    {
      o << GetHelpString ();
      return input;
    }

    return "Invalid Schedule command";
  }

  const CH1* Project::Command (CH1* input, TArray<Project*>* stack) {
    CH1 c,  //< The first flag CH1.
      d,   //< The second flag CH1.
      e;   //< The third flag CH1.
    CH1* end;
    if (input == nullptr) return "Null Project command";
    input = SkipSpaces (input);

    SI4 value;  //< An index to possibly be scanned.
    c = *input;
    if (c == '-') {
      d = *(input + 1);
      e = *(input + 2);
      o << " >" << key_ << " > \"" << c << d << e << "\" < " << input
        << '\n';
      if (e && !isspace (e)) return "Invalid Project flag";
      input += 3;
      switch (d) {
      case 'S':  // Add to the schedule
      {
        if (!e) return "You must enter a Schedule name";
        o << "> Adding Schedule: " << input << '\n';
        end = TextTokenEnd (input);
        d = *input;
        // o << "> d: " << d << '\n';
        if (!d || input == end) return "You must enter a valid key";
        if (ScheduleIndex (input) >= 0 || ScheduleIndex (input) >= 0)
          return "Key is in use. Type -l to list the objects in the current "
          "scope.";
        Schedule* string = new Schedule (input);
        StackPush<Schedule*> (schedules_, string);
        return nullptr;
      }
      case 'P':  // Add new Project
      {
        if (!e) return "You must enter a Project name";
        o << "> Adding Project: " << input << '\n';
        end = TextTokenEnd (input);
        d = *input;
        if (!d || input == end) return Push (stack, this);
        if (ProjectIndex (input) >= 0 || ProjectIndex (input) >= 0)
          return "Key is in use. Type -l to list the objects in the current "
          "scope";
        Project* p = new Project (input);
        StackPush<Project*> (projects_, p);
        return nullptr;
      }
      case 'l':  //< List Objs
      {
        ListObjs ();
        return nullptr;
      }
      case 'p':
      {
        // o << "> Printing ";
        Print ();
        return nullptr;
      }
      case 'r':  //< Remove
      {
        o << "> Removing ";
        d = *(++input);
        if (!isspace (d)) return "Invalid flag";
        d = *(++input);
        input = SkipSpaces (input);
        d = *input;
        if (d == 0) return "Invalid flag argument";
        if (isdigit (d)) {
          if (!sscanf (input, "%i", &value))  //< Shouldn't happen.
            return "Error scanning index to remove";
          o << value << '\n';
          if (!RemoveSchedule (value)) return "Invalid schedule index";
        } else {
          // Search for a project with the given key.
          end = TextTokenEnd (input);
          if (end == input)
            return "You must input a valid Project or Schedule "
            "key";
          value = ProjectIndex (input);
          if (value < 0) {
            value = ScheduleIndex (input);
            if (value < 0) return "Invalid project index";
            RemoveSchedule (value);
          } else {
            RemoveProject (value);
          }
        }
        return nullptr;
      }
      }
      return "Invalid Project flag";
    } else if (isdigit (c)) {
      // Because we checked for the -flag before this, we'll never have a
      // negative number!
      if (!sscanf (input, "%i", &value))  // This shouldn't happen.
        return "Could not read Project member index";
      o << " >" << key_ << " >" << input << '\n';
      input = TextTokenEnd (input);  // Scroll past the number.
      SI4 num_subprojects = projects_->count;
      if (value < num_subprojects)
        return projects_[value]->Command (input, stack);
      else if (value < num_subprojects + schedules_->count - 1)
        return schedules_[value]->Command (input);
      return "Invalid Project member index";
    } else if (c == '?')  // Print detailed help
    {
      c = *(input + 1);
      if (!c) {
        o << GetAppHelpString ();
        return nullptr;
      }
      if (!StringCompare (input + 2, "project")) {
        o << Project::GetHelpString ();
        return nullptr;
      }
      if (!StringCompare (input + 2, "schedule")) {
        o << Schedule::GetHelpString ();
        return nullptr;
      }
      if (!StringCompare (input + 2, "task")) {
        o << Task::GetHelpString ();
        return nullptr;
      }
      return nullptr;
    }
    if (*input == 0) return Push (stack, this);
    // Search for key to pass the command CH1 onto.
    end = TextTokenEnd (input);
    o << " >" << key_ << " >" << input << '\n';
    if (*input == 0) return "Invalid key format.";

    value = ProjectIndex (input);
    if (value >= 0)
      return StackGet<Project*> (projects_, value)->Command (end, stack);
    value = ScheduleIndex (input);
    if (value >= 0) return StackGet<Schedule*> (schedules_, value)->Command (end);
    return "The Project does not contain the given key in this scope.";
  }

 private:
  TStrand<> key_,        //< Name of the schedule.
          readme_;       //< Readme for the Schedule.
  SI4 color_,            //< Initial color of all of the Tasks.
      num_repeats_;      //< Number of times the Schedule repeats.
  TArray<Task*> tasks_;  //< Array of Task pointers.
};
}       //< namespace _
#endif
#endif
