/* Kabuki Toolkit
@version 0.x
@file    ~/libraries/pro/project.h
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

#pragma once
#include <stdafx.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_PRO_PROJECT_H
#define HEADER_FOR_KT_PRO_PROJECT_H
#include "schedule.h"
#include "task.h"

namespace _ {

class Project;
using ProjectArray = Stack<Schedule*>;
using ScheduleArray = Stack<Project*>;

/* Pops a Project pointer off the stack. */
Project* Pop(Stack<Project*>* stack, const char* result);

/* Pushes a Project pointer onto the stack. */
const char* Push(Stack<Project*>* stack, Project* project);

/* A tree-like Project in the system that can be scheduled.
    An Project can be a person or virtual entity. An Project is identified by
   their unique key. */
class Project : public Operand {
 public:
  enum {
    INIT_NUM_SCHEDULES = 4,    //< The initial number of Schedule(string).
    MAX_SCHEDULES = 32,        //< Max number of Schedule(string).
    MAX_SUMMARY_LENGTH = 256,  //< The max readme char length.
    MAX_DETAIL_LENGTH = 1024 * 1024,  //< The max detail char length.
  };

  /* Default constructor initializes list with the given or left key. */
  Project(const char* key = "Unnamed", const char* readme = "");

  /* Constructor initializes with stolen key and readme. */
  Project(char* key, char* readme);

  /* Destructor. */
  ~Project();

  /* Gets the key. */
  const char* GetKey();

  /* Sets the key. */
  void StealKey(char* new_key);

  /* Clones the given key. */
  void SetKey(const char* new_key);

  /* Gets the readme. */
  const char* GetReadMe();

  /* Sets the readme and deletes the current one. */
  void StealReadMe(char* new_readme);

  /* Sets the readme to a clone of the new_readme and deletes the old one.. */
  void CloneReadMe(const char* new_readme);

  /* Adds a Project.
      @param project The project to add.
      @return Returns false if the key is not a token. */
  bool AddProject(const char* key);

  /* Adds a Project.
      @param project The project to add.
      @return Returns false if the key is not a token. */
  bool AddProject(Project* project);

  /* Adds a Schedule.
      @param key The key of the new schedule to add.
      @return Returns false if the key is not a token. */
  bool AddSchedule(const char* key);

  /* Adds a currently existing Schedule.
      @param schedule A new schedule this object now owns the memory of.
      @return Returns false if the key is not a token. */
  bool AddSchedule(Schedule* schedule);

  /* Lists the keys in the given scope. */
  void ListObjs();

  /* Searches for a Schedule with the given key. */
  int ScheduleIndex(const char* key);

  /* Gets the Schedule with the given key.
      @param key The key of the Schedule to search for.
      @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(int index);

  /* Gets the Schedule with the given key.
      @param key The key of the Schedule to search for.
      @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(const char* key);

  /* Attempts to remove the Schedule at the given index. */
  bool RemoveSchedule(int index);

  /* Attempts to remove the Schedule at the given index. */
  bool RemoveSchedule(const char* key);

  /* Searches for a Project with the given key. */
  int ProjectIndex(const char* key);

  /* Gets the Project with the given key.
      @param key The key of the Project to search for.
      @return Returns null if calendar does not contain the given key. */
  Project* GetProject(int index);

  /* Searches for the given project key.
      @param  key The key of the Project to search for.
      @return Returns null if calendar does not contain the given key. */
  Project* GetProject(const char* key);

  /* Attempts to remove the Project at the given index. */
  bool RemoveProject(int index);

  /* Attempts to remove the Project at the given index. */
  bool RemoveProject(const char* key);

  /* Selects the task at the given index. */
  bool Select(int index);

  /* Deserializes the file with the stored key.
      @return Returns 0 upon success and a pointer to an error char upon
              failure. */
  void Load();

  /* Serializes the list to the given file.
      @return Returns 0 upon success and a pointer to an error char upon
              failure. */
  void Save();

  /* Gets the help char. */
  static const char* GetAppHelpString();

  /* Gets the help char. */
  static const char* GetHelpString();

  /* Prints the Shopping list to the console.
      @param indent_level The number of tabs of the indentation level.
      @param bullet_type The type of bullet to print: numbers, upper case
                         letters, lower case letters, stars, dashes etc.
      @param spaces_per_tab The number of spaces in a tab.
  */
  void Out(int indent_level = 0, char bullet_type = '1',
           int spaces_per_tab = 4);

  /* Script operations. */
  virtual const Op* Star(wchar_t index, Expr* expr);

 private:
  char *key_,                 //< Entity key.
      *readme_;               //< Project readme.
  Schedule* task_;            //< Current Schedule being edited.
  ScheduleArray* schedules_;  //< Composition of Schedule(string).
  ProjectArray* projects_;    //< Child projects.
};

}   //< namespace _
#endif  //< HEADER_FOR_KT_PRO_PROJECT_H
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
