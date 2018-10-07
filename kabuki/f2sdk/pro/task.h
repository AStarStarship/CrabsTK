/* Kabuki Toolkit
@version 0.x
@file    /libraries/kt/pro/task.h
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
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_PRO_TASK
#define HEADER_FOR_KT_PRO_TASK
#include "config.h"

namespace _ {

/* An abstract task in a set of Task(string). */
class Task {
 public:
  /* Creates a task with the given fields and clones the strings. */
  Task(const char* summary = "", const char* details = "", float weight_ = 0.0f,
       int time_testimate_min = 60 * 60);

  /* Creates a task with the given fields from strings this object now
      owns and must delete. */
  Task(char* summary, char* details, float weight_, int time_estimate_min);

  /* Destructor. */
  ~Task();

  /* Gets the summary. */
  const char* GetHeader();

  /* Sets the summary. */
  void SteadHeader(char* new_summary);

  /* Gets the details. */
  const char* GetDetails();

  /* Sets the details. */
  void SetDetails(char* new_details);

  /* Gets the result. */
  const char* GetResults();

  /* Sets the post_result. */
  void SetResults(char* new_result);

  /* Gets the review. */
  const char* GetReview();

  /* Sets the post_result. */
  void StealReview(char* new_review);

  /* Gets the weight. */
  float GetWeight();

  /* Sets the weight. */
  bool SetWeight(float new_value);

  /* Gets the assessment. */
  float GetAssessment();

  /* Sets the self assessment. */
  bool SetAssessment(float new_value);

  /* Gets the grade. */
  float GetGrade();

  /* Sets the grade. */
  bool SetGrade(float new_value);

  /* Gets the time_estimate. */
  Tms GetTimeEstimate();

  /* Sets the time_estimate.
      @param time The new time_estimate.
      @return Returns null upon success and a pointer to an error char upon
     failure. */
  void SetTimeEstimate(Tms time);

  /* Gets the time_begins. */
  Tms GetTimeBegins();

  /* Sets the time_begins.
      @param time The new time_begins.
      @return Returns null upon success and a pointer to an error char upon
     failure. */
  const char* SetTimeBegins(Tms time);

  /* Gets the end time. */
  Tms GetTimeEnds();

  /* Sets the time_ends.
      @param time The new time_ends.
      @return Returns null upon success and a pointer to an error char upon
     failure. */
  const char* SetTimeEnds(Tms time);

  /* Gets the time_started. */
  Tms GetTimeStarted();

  /* Sets the time_started.
      @param time The new time_started.
      @return Returns null upon success and a pointer to an error char upon
     failure. */
  const char* SetTimeStarted(Tms time);

  /* Gets the stop time. */
  Tms GetTimeStopped();

  /* Sets the time_stopped.
      @param time The new time_stopped.
      @return Returns null upon success and a pointer to an error char upon
     failure. */
  const char* SetTimeStopped(Tms time);

  /* Starts the task and saves the start time. */
  void Start();

  /* Stopped the task. */
  void Stop(char* result, float result_grade = 1.0f);

  /* Stopped the task. */
  void Stop(const char* result, float result_grade = 1.0f);

  /* Returns true if the task is complete.
      A Task is considered complete if the result char is null.
      @return Returns true if the result char is not null. */
  bool IsDone();

  /* Adds a Task that collides with this time slot. */
  void AddCollision(Task* task);

  /* Returns the head of the linear linked list of collision. */
  Task* Collision();

  /* Returns true if the event contains the given time. */
  bool Contains(Tms t);

  /* Prints the help menu. */
  static const char* GetHelpString();

  /* Prints the Task to the console. */
  void Out(int indentation = 0, int index = 0);

  /* Parse a text command stream (possibly from the user). */
  const char* Command(char* input);

 private:
  char *summary_,         //< Task summary.
      *details_,          //< Task details.
      *result_,           //< Task results log entry.
      *review_;           //< An after-incident review of the Event.
  float weight_,          //< Weight of the task.
      assessment_,        //< User's self-assessed grade.
      grade_;             //< Reviewed grade.
  Tms time_estimate_,  //< Amount of time it is estimated this will take.
      time_begins_,       //< Time the task begins.
      time_ends_,         //< Time the task was finished ends.
      time_started_,      //< Time the user started the task.
      time_stopped_;      //< Time the user stopped the task.
  Task* collisions_;      //< Pointer to the collision node (if any).
};
}       //< namespace _
#endif  //< HEADER_FOR_KT_PRO_TASK
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
