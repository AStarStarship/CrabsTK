/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/pro/cproject.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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
#include "schedule.h"
#include "ctask.h"

namespace _ {

class Project;
using ProjectArray = Stack<Schedule*>;
using ScheduleArray = Stack<Project*>;

/* Pops a Project pointer off the stack. */
Project* Pop(Stack<Project*>* stack, const CH1* result);

/* Pushes a Project pointer onto the stack. */
const CH1* Push(Stack<Project*>* stack, Project* project);

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

  /* Default constructor initializes list with the given or left key. */
  Project(const CH1* key = "Unnamed", const CH1* readme = "");

  /* Constructor initializes with stolen key and readme. */
  Project(CH1* key, CH1* readme);

  /* Destructor. */
  ~Project();

  /* Gets the key. */
  const CH1* GetKey();

  /* Sets the key. */
  void StealKey(CH1* new_key);

  /* Clones the given key. */
  void SetKey(const CH1* new_key);

  /* Gets the readme. */
  const CH1* GetReadMe();

  /* Sets the readme and deletes the current one. */
  void StealReadMe(CH1* new_readme);

  /* Sets the readme to a clone of the new_readme and deletes the old one.. */
  void CloneReadMe(const CH1* new_readme);

  /* Adds a Project.
  @param project The project to add.
  @return Returns false if the key is not a token. */
  BOL AddProject(const CH1* key);

  /* Adds a Project.
  @param project The project to add.
  @return Returns false if the key is not a token. */
  BOL AddProject(Project* project);

  /* Adds a Schedule.
  @param key The key of the new schedule to add.
  @return Returns false if the key is not a token. */
  BOL AddSchedule(const CH1* key);

  /* Adds a currently existing Schedule.
  @param schedule A new schedule this object now owns the memory of.
  @return Returns false if the key is not a token. */
  BOL AddSchedule(Schedule* schedule);

  /* Lists the keys in the given scope. */
  void ListObjs();

  /* Searches for a Schedule with the given key. */
  SI4 ScheduleIndex(const CH1* key);

  /* Gets the Schedule with the given key.
      @param key The key of the Schedule to search for.
      @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(SI4 index);

  /* Gets the Schedule with the given key.
      @param key The key of the Schedule to search for.
      @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(const CH1* key);

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(SI4 index);

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(const CH1* key);

  /* Searches for a Project with the given key. */
  SI4 ProjectIndex(const CH1* key);

  /* Gets the Project with the given key.
      @param key The key of the Project to search for.
      @return Returns null if calendar does not contain the given key. */
  Project* GetProject(SI4 index);

  /* Searches for the given project key.
      @param  key The key of the Project to search for.
      @return Returns null if calendar does not contain the given key. */
  Project* GetProject(const CH1* key);

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(SI4 index);

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(const CH1* key);

  /* Selects the task at the given index. */
  BOL Select(SI4 index);

  /* De-serializes the file with the stored key.
      @return Returns 0 upon success and a pointer to an error CH1 upon
              failure. */
  void Load();

  /* Serializes the list to the given file.
      @return Returns 0 upon success and a pointer to an error CH1 upon
              failure. */
  void Save();

  /* Gets the help CH1. */
  static const CH1* GetAppHelpString();

  /* Gets the help CH1. */
  static const CH1* GetHelpString();

  /* Prints the Shopping list to the console.
      @param indent_level The number of tabs of the indentation level.
      @param bullet_type The type of bullet to print: numbers, upper case
                         letters, lower case letters, stars, dashes etc.
      @param spaces_per_tab The number of spaces in a tab.
  */
  void Out(SI4 indent_level = 0, CH1 bullet_type = '1',
           SI4 spaces_per_tab = 4);

  /* Script operations.
  */
  virtual const Op* Star(wchar_t index, Expr* expr);

 private:
  CH1* key_,                 //< Entity key.
      * readme_;              //< Project readme.
  Schedule* task_;            //< Current Schedule being edited.
  ScheduleArray* schedules_;  //< Composition of Schedule(string).
  ProjectArray* projects_;    //< Child projects.
};

}  // namespace _
#endif  //< INCLUDED_KABUKI_PRO_PROJECT
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
