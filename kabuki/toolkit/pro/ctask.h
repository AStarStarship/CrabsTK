/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/pro/ctask.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#ifndef HEADER_FOR_KT_PRO_TASK
#define HEADER_FOR_KT_PRO_TASK

namespace _ {

/* An abstract task in a set of Task(string). */
class Task {
 public:
  /* Creates a task with the given fields and clones the strings. */
  Task(const CH1* summary = "", const CH1* details = "", FLT weight_ = 0.0f,
       SI4 time_testimate_min = 60 * 60);

  /* Creates a task with the given fields from strings this object now
      owns and must delete. */
  Task(CH1* summary, CH1* details, FLT weight_, SI4 time_estimate_min);

  /* Destructor. */
  ~Task();

  /* Gets the summary. */
  const CH1* GetHeader();

  /* Sets the summary. */
  void SteadHeader(CH1* new_summary);

  /* Gets the details. */
  const CH1* GetDetails();

  /* Sets the details. */
  void SetDetails(CH1* new_details);

  /* Gets the result. */
  const CH1* GetResults();

  /* Sets the post_result. */
  void SetResults(CH1* new_result);

  /* Gets the review. */
  const CH1* GetReview();

  /* Sets the post_result. */
  void StealReview(CH1* new_review);

  /* Gets the weight. */
  FLT GetWeight();

  /* Sets the weight. */
  bool SetWeight(FLT new_value);

  /* Gets the assessment. */
  FLT GetAssessment();

  /* Sets the self assessment. */
  bool SetAssessment(FLT new_value);

  /* Gets the grade. */
  FLT GetGrade();

  /* Sets the grade. */
  bool SetGrade(FLT new_value);

  /* Gets the time_estimate. */
  Tms GetTimeEstimate();

  /* Sets the time_estimate.
  @param time The new time_estimate.
  @return Returns null upon success and a pointer to an error CH1 upon
  failure. */
  void SetTimeEstimate(Tms time);

  /* Gets the time_begins. */
  Tms GetTimeBegins();

  /* Sets the time_begins.
  @param time The new time_begins.
  @return Returns null upon success and a pointer to an error CH1 upon
  failure. */
  const CH1* SetTimeBegins(Tms time);

  /* Gets the end time. */
  Tms GetTimeEnds();

  /* Sets the time_ends.
  @param time The new time_ends.
  @return Returns null upon success and a pointer to an error CH1 upon
  failure. */
  const CH1* SetTimeEnds(Tms time);

  /* Gets the time_started. */
  Tms GetTimeStarted();

  /* Sets the time_started.
  @param time The new time_started.
  @return Returns null upon success and a pointer to an error CH1 upon
  failure. */
  const CH1* SetTimeStarted(Tms time);

  /* Gets the stop time. */
  Tms GetTimeStopped();

  /* Sets the time_stopped.
  @param time The new time_stopped.
  @return Returns null upon success and a pointer to an error CH1 upon
  failure. */
  const CH1* SetTimeStopped(Tms time);

  /* Starts the task and saves the start time. */
  void Start();

  /* Stopped the task. */
  void Stop(CH1* result, FLT result_grade = 1.0f);

  /* Stopped the task. */
  void Stop(const CH1* result, FLT result_grade = 1.0f);

  /* Returns true if the task is complete.
  A Task is considered complete if the result CH1 is null.
  @return Returns true if the result CH1 is not null. */
  bool IsDone();

  /* Adds a Task that collides with this time slot. */
  void AddCollision(Task* task);

  /* Returns the head of the linear linked list of collision. */
  Task* Collision();

  /* Returns true if the event contains the given time. */
  bool Contains(Tms t);

  /* Prints the help menu. */
  static const CH1* GetHelpString();

  /* Prints the Task to the console. */
  void Out(SI4 indentation = 0, SI4 index = 0);

  /* Parse a text command stream (possibly from the user). */
  const CH1* Command(CH1* input);

 private:
  CH1 *summary_,          //< Task summary.
      *details_,          //< Task details.
      *result_,           //< Task results log entry.
      *review_;           //< An after-incident review of the Event.
  FLT weight_,            //< Weight of the task.
      assessment_,        //< User's self-assessed grade.
      grade_;             //< Reviewed grade.
  TM8 time_estimate_,     //< Amount of time it is estimated this will take.
      time_begins_,       //< Time the task begins.
      time_ends_,         //< Time the task was finished ends.
      time_started_,      //< Time the user started the task.
      time_stopped_;      //< Time the user stopped the task.
  Task* collisions_;      //< Pointer to the collision node (if any).
};
} //< namespace _
#endif
#endif
