/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/pro/t_project.h
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
#ifndef INCLUDED_KABUKI_PRO_PROJECT
#define INCLUDED_KABUKI_PRO_PROJECT
#include "t_schedule.h"
#include "t_task.h"

namespace _ {

/* A tree-like Project in the system that can be scheduled.
An Project can be a person or virtual entity. An Project is identified by
their unique key. */
class Project : public Operand {
 public:
  enum {
    INIT_NUM_SCHEDULES = 4,           //< Initial number of Schedule(string).
    MAX_SCHEDULES = 32,               //< Max number of Schedule(string).
    MAX_SUMMARY_LENGTH = 256,         //< The max readme CH1 length.
    MAX_DETAIL_LENGTH = 1024 * 1024,  //< The max detail CH1 length.
  };

  /* Pops a Project pointer off the stack. */
  static Project* Pop (Stack<Project*>* stack, const CH1* result) {
    ASSERT (stack)
      ASSERT (result == nullptr)
      if (strcmp ("", result)) {
        o << "> " << result << ".\n";
        return stack;
      }
    o << "> Pop!\n";
    ProjectNode* new_top = stack->prev;
    if (new_top == nullptr) {
      o << "> Exiting application...\n";
      return nullptr;
    }
    o << "Popping " << stack->project->Key () << '\n'
      << "new_top: " << new_top->project->Key () << '\n';
    delete stack;
    return new_top;
  }

  /* Pushes a Project pointer onto the stack. */
  static const CH1* Push (ProjectNode*& stack, Project* project) {
    if (stack == nullptr) return "Null push stack";
    if (stack->project == project)  // Then pop the project off the stack.
      return "";
    o << "> Pushing " << project->Key () << '\n';
    ProjectNode* temp = new ProjectNode (project, stack);
    stack = temp;
    return nullptr;
  }

  /* Default constructor initializes list with the given or left key. */
  Project(const CH1* key = "Unnamed", const CH1* readme = "")
    : key_ (key == nullptr ? StrandClone ("") : key),
    readme_ (readme == nullptr ? StrandClone ("") : readme),
    task_ (nullptr) {}

  /* Constructor initializes with stolen key and readme. */
  Project(CH1* key, CH1* readme)
    : key_ (key), readme_ (readme), task_ (nullptr) {}

  /* Destructor. */
  ~Project() {}

  /* Gets the key. */
  const TStrand<>& Key() { return key_; }

  /* Sets the key. */
  void StealKey(CH1* new_key);

  /* Clones the given key. */
  void SetKey (const CH1* new_key) { key_.Set (new_key); }

  /* Gets the readme. */
  const TStrand<>& GetReadMe() { return readme_; }

  /* Sets the readme to a clone of the new_readme and deletes the old one.. */
  void SetReadme (const CH1* new_readme) {
    readme_.Set (new_readme);
  }

  /* Adds a Project.
  @param project The project to add.
  @return Returns false if the key is not a token. */
  BOL AddProject(const CH1* key) {
    if (!IsToken (key)) return false;
    StackPush<Project*> (projects_, new Project (key));
    return true;
  }

  /* Adds a Project.
  @param project The project to add.
  @return Returns false if the key is not a token. */
  BOL AddProject(Project* project) {
    if (!project) return false;
    StackPush<Project*> (projects_, project);
    return true;
  }

  /* Adds a Schedule.
  @param key The key of the new schedule to add.
  @return Returns false if the key is not a token. */
  BOL AddSchedule(const CH1* key) {
    if (!IsToken (key)) return false;
    StackPush<Schedule*> (schedules_, new Schedule (key));
    return true;
  }

  /* Adds a currently existing Schedule.
  @param schedule A new schedule this object now owns the memory of.
  @return Returns false if the key is not a token. */
  BOL AddSchedule(Schedule* schedule) {
    if (!schedule) return false;
    StackPush<Schedule*> (schedules_, schedule);
    return true;
  }

  /* Lists the keys in the given scope. */
  template<typename Printer>
  Printer& ListObjs(Printer& o) {
    o << LineStrand ();
      << "|\n"
      "| Objs in Scope:\n"
      "|\n"
      "| Projects: "
      << projects_->count << '\n';
    for (SI4 i = 0; i < projects_->count; ++i)
      o << "| " << i << ".) "
      << StackGet<Project*> (projects_, i)->Key () << '\n';
    o << "|\n"
      "| Schedules: "
      << schedules_->count << '\n';
    for (SI4 i = 0; i < schedules_->count; ++i)
      o << "| " << i << ".) "
      << schedules_[i]->Key () << '\n';
    return o << LineStrand ('_');
  }

  /* Searches for a Schedule with the given key. */
  SI4 ScheduleIndex(const CH1* key) {
    if (key == nullptr) return false;
    //  Search for an already existing key to add it to:
    for (SI4 i = 0; i < schedules_->count; ++i)
      if (CompareTokenString (key, StackGet<Schedule*> (schedules_, i)->Key ()))
        return i;
    return -1;
  }

  /* Gets the Schedule with the given key.
  @param key The key of the Schedule to search for.
  @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(SI4 index) {
    if (index < 0) return nullptr;
    if (index >= schedules_->count) return nullptr;
    return schedules_[index];
  }

  /* Gets the Schedule with the given key.
  @param key The key of the Schedule to search for.
  @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(const CH1* key) {
    return schedules_[ScheduleIndex (key)];
  }

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(SI4 index) {
    return StackRemove<Schedule*> (schedules_, index);
  }

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(const CH1* key) {
    return RemoveSchedule (ScheduleIndex (key));
  }

  /* Searches for a Project with the given key. */
  SI4 ProjectIndex(const CH1* key) {
    if (key == nullptr) return -1;
    //  Search for an already existing key to add it to:
    for (SI4 i = 0; i < projects_->count; ++i)
      if (CompareTokenString (key, StackGet<Project*> (projects_, i)->Key ()))
        return i;
    return -1;
  }

  /* Gets the Project with the given key.
  @param key The key of the Project to search for.
  @return Returns null if calendar does not contain the given key. */
  Project* GetProject(SI4 index) {
    if (index < 0) return nullptr;
    if (index >= projects_->count) return nullptr;
    return StackGet<Project*> (projects_, index);
  }

  /* Searches for the given project key.
  @param  key The key of the Project to search for.
  @return Returns null if calendar does not contain the given key. */
  Project* GetProject(const CH1* key) {
    return StackGet<Project*> (projects_, ProjectIndex (key));
  }

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(SI4 index) {
    return StackRemove<Project*> (projects_, index);
  }

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(const CH1* key) {
    return RemoveProject (ProjectIndex (key));
  }

  /* Selects the task at the given index. */
  BOL Select(SI4 index) {
    if (index < 0) return false;
    if (index >= schedules_->count) return false;
    task_ = StackGet<Schedule*> (schedules_, index);
    return true;
  }

  /* De-serializes the file with the stored key.
  @return Returns 0 upon success and a pointer to an error CH1 upon
  failure. */
  void Load() {
    /*
    const CH1* error;  //< Error flag.
    o << "\n\n| Reading from " << key << '\n';
    std::ifstream file (key);
    if (!file.is_open ())
    {
    o << "Unable to open file for reading";
    return;
    }
    CH1 buffer[MAX_STRING_LENGTH];
    file.getline (buffer, MAX_STRING_LENGTH, ',');
    //< Throw away till the comma..
    SI4 num_categories;
    file >> num_categories;
    //o << num_categories << " categories.\n";
    file.getline (buffer, MAX_STRING_LENGTH, '\n');
    //< Throw away the rest of the line.
    // Read each of the ItemList categories.
    for (SI4 i = 0; i < num_categories; ++i)
    {
    CH1* key = new CH1[MAX_STRING_LENGTH];
    file.getline (key, MAX_STRING_LENGTH, ',');
    //o << "| key: " << key;
    SI4 num_items;
    file >> num_items;
    //o << " num_items: " << num_items << '\n';
    while (file.get () != '\n');  //< Throw away the rest of the line.
    for (SI4 i = 0; i < num_items; ++i)
    {
    CH1* name = new CH1[MAX_STRING_LENGTH];
    file.getline (name, MAX_STRING_LENGTH, ',');
    SI4 quantity;
    FP4 price;
    CH1 c;
    //file >> quantity >> c >> price >> c;
    file >> quantity;
    while (file.get () != ',');
    file >> price;
    while (file.get () != '\n');
    add (key, name, quantity, price);
    }
    :}
    file.close ();
    */
  }

  /* Serializes the list to the given file.
  @return Returns 0 upon success and a pointer to an error CH1 upon
  failure. */
  void Save() {
    /*
    o << "\n\n| Writing to " << key << "...\n";
    const CH1* error;
    std::ofstream file;
    file.open (key);
    if (!file.is_open ())
    {
    o << "Unable top open file for writing";
    return;
    }
    file << "Project," << element_count << ',' << get_total () << '\n';
    ItemList** list = lists;
    for (SI4 i = 0; i < element_count; ++i)
    list[i]->write_to_open_file (file);
    file.close ();
    */
  }

  /* Gets the help CH1. */
  static const CH1* GetAppHelpString() {
    return "\n| kabuki.toolkit.pro.Project Help:\n"
      "| There are three primary classes for use:\n"
      "|\n"
      "| Task     - A general purpose todo task.\n"
      "| Schedule - A collection of tasks that can repeat a specified\n"
      "|            number of times.\n"
      "| Project  - A tree structure composed of a collection of\n"
      "|            Schedule(string) and a collection of sub-projects.\n"
      "|\n"
      "| To print out the commands for any of the classes, enter:\n"
      "| \"? project\"\n"
      "| \"? schedule\" or\n"
      "| \"? task\"\n"
      "|\n"
      "| Each Schedule and Project has a unique key assigned by the\n"
      "| IdServer. A key may not start with a number or dash ('-'),\n"
      "| and must not contain any whitespace. This key is they used to\n"
      "| push the given project onto the stack of projects, and to\n"
      "| route commands to the intended Schedules. To pop the current\n"
      "| project off the stack, press enter. To exit the application,\n"
      "| press enter till you've popped all the projects off the stack.\n"
      "|\n"
      "| Commands may or may not have arguments. When the system \n"
      "| starts, it is in the root Project scope. The user can enter\n"
      "| the -l command to list the handles in the current scope.\n"
      "|\n"
      "| Examples: subproject example-schedule -string \"A task\" -b "
      "3:30PM'\n"
      "|           class-schedule -a 1.0 -string \"Homework\" -d \"Program "
      "5\n"
      "|           -B @3PM -E @4PM -S @3:30PM -P @6:58PM \n"
      "|           -w 25.0 -A 1.0 -R \"I got it all right!\"\n"
      "|           -g 0.5 -r \"Wrong! All wrong!\"\n"
      "|\n"
      "| Some fields are marked as optional. These fields you don't\n"
      "| have enter to use the command. Fields may be redefined as\n"
      "| multiple times in one command.\n"
      "|\n"
      "| Examples: subproject schedule -a 1.0 -a 1.0 -a 1.0 -a 1.0\n"
      "|\n";
  }

  /* Gets the help CH1. */
  static const CH1* GetHelpString() {
    return "\n| Project Commands: \n"
      "|    -S  Adds a new Schedule and selects it.\n"
      "|        Examples: > -S new-schedule\n"
      "|\n"
      "|    -P  Adds a new Project and pushes it onto the stack.\n"
      "|        Args: key:CH1\n"
      "|        Examples: > -P new-project\n"
      "|\n"
      "|    -l  Lists the current item's in scope to the console.\n"
      "|        Args: none\n"
      "|\n"
      "|    -p  Prints out object with the given handle.\n"
      "|        Example: > task_name -p\n"
      "|\n"
      "|    -r  Removes the object at the given index\n"
      "|        Args: index:SI4 or key:CH1\n"
      "|        Examples: > -r 1\n"
      "|                  > -r project-name\n"
      "|\n"
      "|    CR  Pressing enter (ASCII CR) pops the currently selected\n"
      "|        project off of the stack of projects till it gets to\n"
      "|        the root project, then it exits the program.\n"
      "|        Args: none\n"
      "|\n";
  }

  /* Prints the Shopping list to the console.
  @param indent_level The number of tabs of the indentation level.
  @param bullet_type The type of bullet to print: numbers, upper case
  letters, lower case letters, stars, dashes etc.
  @param spaces_per_tab The number of spaces in a tab. */
  template<typename Printer>
  Printer& Print(Printer& o, SI4 indent_level = 0, CH1 bullet_type = '1',
           SI4 spaces_per_tab = 4) {
    o << LineStrand ()
      << "\nProject: " << key_
      << "\n| Projects: " << projects_->count << "|\n";
    for (SI4 i = 0; i < projects_->count; ++i) {
      Ar4Get<Project*> (projects_, i)->Print (o);
    }
    o << "| Schedules: " << schedules_->count << "|\n";
    for (SI4 i = 0; i < schedules_->count; ++i) {
      Ar4Get<Schedule*> (schedules_, i)->Print (o);
    }
    return o << LineStrand ('_');
  }

  /* Script2 operations. */
  virtual const Op* Star (wchar_t index, Expr* expr) {
    return nullptr;
  }

 private:
  TStrand<> key_,             //< Entity key.
      readme_;                //< Project readme.
  Schedule* task_;            //< Current Schedule being edited.
  TStack<Project> schedules_; //< Composition of Schedule(string).
  TStack<Schedule> projects_; //< Child projects.
};

}  // namespace _
#endif
#endif
