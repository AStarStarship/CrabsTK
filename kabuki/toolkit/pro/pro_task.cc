/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/pro/pro_task.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_PRO_1
#include "ctask.h"

namespace _ {

Task::Task(const CH1* summary, const CH1* details, FLT weight_,
           SI4 time_estimate_min)
    : summary_(TextClone(summary)),
      details_(TextClone(details)),
      result_(nullptr),
      review_(nullptr),
      weight_(weight_),
      assessment_(0.0f),
      grade_(0.0f),
      time_estimate_(time_estimate_min * 60),
      time_begins_(0),
      time_ends_(0),
      time_started_(0),
      time_stopped_(0),
      collisions_(nullptr) {}

Task::Task(CH1* summary, CH1* details, FLT weight_, SI4 time_estimate_min)
    : summary_(summary),
      details_(details),
      result_(nullptr),
      review_(nullptr),
      weight_(weight_),
      assessment_(0.0f),
      grade_(0.0f),
      time_estimate_(time_estimate_min * 60),
      time_begins_(0),
      time_ends_(0),
      time_started_(0),
      time_stopped_(0),
      collisions_(nullptr) {}

Task::~Task() {}

const CH1* Task::GetHeader() { return summary_; }

void Task::SteadHeader(CH1* new_summary) {
  ASSERT(new_summary)
  summary_ = new_summary;
}

const CH1* Task::GetDetails() { return details_; }

void Task::SetDetails(CH1* new_details) {
  ASSERT(new_details)
  details_ = new_details;
}

const CH1* Task::GetResults() { return result_; }

void Task::SetResults(CH1* new_result) {
  if (new_result == nullptr) return;
  result_ = new_result;
}

const CH1* Task::GetReview() { return review_; }

void Task::StealReview(CH1* new_review) {
  if (new_review == nullptr) return;
  review_ = new_review;
}

FLT Task::GetWeight() { return weight_; }

bool Task::SetWeight(FLT new_weight) {
  if (new_weight < 0.0f) return false;
  weight_ = new_weight;
  return true;
}

FLT Task::GetAssessment() { return assessment_; }

bool Task::SetAssessment(FLT new_assessment) {
  if (new_assessment < 0.0f) return false;
  if (new_assessment > 1.0f) return false;
  assessment_ = new_assessment;
  return true;
}

FLT Task::GetGrade() { return grade_; }

bool Task::SetGrade(FLT new_grade) {
  if (new_grade < 0.0f) return false;
  if (new_grade > 1.0f) return false;
  grade_ = new_grade;
  return true;
}

Tms Task::GetTimeEstimate() { return time_estimate_; }

void Task::SetTimeEstimate(Tms time) { time_estimate_ = time; }

Tms Task::GetTimeBegins() { return time_begins_; }

const CH1* Task::SetTimeBegins(Tms time) {
  if (time >= time_ends_ && time_ends_ != 0)
    return "time_begins_ must be before time_ends_";
  time_begins_ = time;
  return 0;
}

Tms Task::GetTimeEnds() { return time_ends_; }

const CH1* Task::SetTimeEnds(Tms time) {
  if (time < time_ends_) return "time_ends_ must be after time_begans";
  time_ends_ = time;
  return 0;
}

Tms Task::GetTimeStarted() { return time_started_; }

const CH1* Task::SetTimeStarted(Tms time) {
  if (time < time_stopped_) return "Start time must be before stop time";
  time_stopped_ = time;
  return 0;
}

Tms Task::GetTimeStopped() { return time_stopped_; }

const CH1* Task::SetTimeStopped(Tms time) {
  if (time <= time_started_) return "time_stopped_ must be after time_started_";
  time_stopped_ = time;
  return 0;
}

void Task::Stop(CH1* new_result, FLT new_assessment) {
  result_ = new_result;
  assessment_ = new_assessment;
  time(&time_stopped_);
}

void Task::Start() { time(&time_started_); }

void Task::Stop(const CH1* new_result, FLT new_assessment) {
  Stop(TextClone(new_result), new_assessment);
}

void Task::AddCollision(Task* task) {
  if (task == nullptr) return;
  task->collisions_ = nullptr;
  collisions_ = task;
}

Task* Task::Collision() { return collisions_; }

bool Task::IsDone() { return summary_ != 0; }

bool Task::Contains(Tms t) {
  if (t < time_begins_) return false;
  if (time_stopped_ != 0) {
    if (t > time_stopped_) return false;
    if (t > time_started_) return true;
    if (t < time_ends_) return true;
    return false;
  }
  if (t < time_ends_) return true;
  return false;
}

void Task::Print(SI4 indentation, SI4 index) {
  PrintBreak();
  Printf("| Task (%3f): %string", weight_, summary_);
  PrintBreak("\n|", '-');
  std::cout << "| Begins: ";
  PrintDateTime(time_begins_);
  std::cout << "       Ends: ";
  PrintDateTime(time_ends_);
  PrintBreak("\n|", '-');
  std::cout << "| Details: " << details_;
  PrintBreak("\n|", '-');
  std::cout << "| Assessment: " << assessment_
            << " Result: " << (result_ == nullptr ? "N/A" : result_)
            << "\n| Started: ";
  PrintDateTime(time_started_);
  std::cout << "       Stopped: ";
  PrintDateTime(time_started_);
  PrintBreak("\n|", '-');
  std::cout << "|  Grade: " << grade_
            << " Review: " << (review_ == nullptr ? "N/A" : review_);
  PrintBreak("\n|", '_');
}

const CH1* Task::GetHelpString() {
  return "| Task: A general purpose schedule task.\n"
         "| Data Members\n:"
         "| Header    - A brief summary of the task.\n"
         "| Details    - Specific details about the task.\n"
         "| Weight     - A floating-point graph node weight.\n"
         "| Assessment - A floating-point between 0.0 and 1.0 that\n"
         "|              is a self assessment of the percent of the\n"
         "|              weight completed.\n"
         "| Result     - A brief self review of the completed task.\n"
         "| Grade      - A floating-point between 0.0 and 1.0 that\n"
         "|              is a reviewed assessment of the percent of the\n"
         "|              weight completed.\n"
         "| Review     - A post-task review of the work performed.\n"
         "|\n"
         "| Task Commands:\n"
         "|     -?    Print Help\n"
         "|     -B    Time Begins\n"
         "|     -E    Time Ends\n"
         "|     -S    Time Started\n"
         "|     -P    Time Stopped\n"
         "|     -a    Assessment\n"
         "|     -d    Details\n"
         "|     -g    Grade\n"
         "|     -string    Header\n"
         "|     -v    Review\n"
         "|     -w    Weight\n";
}

const CH1* Task::Command(CH1* input) {
  CH1 c,        //< The first CH1.
      d;         //< The second CH1.
  CH1 *end,     //< Used to detect the result of parsing.
      *buffer;   //< String buffer.
  SI4 value;     //< Value to (possibly) be parsed and temp value.
  FLT number;  //< A number to (possibly) be parsed.
  Tms time;      //< Time to (possibly) be parsed.
  if (input == nullptr) return "Task input was null";
  input = SkipSpaces(input);
  c = *input;
  if (c == 0) {
    std::cout << "\n| c == 0\n";
    return nullptr;
  }
  std::cout << "< " << summary_ << " < " << input << '\n';
  if (c == '-') {
    c = *(input + 1);
    d = *(input + 2);
    input += 3;
    if (d && !isspace(d)) return "Invalid Task flag";
    switch (c) {
      case '?':  // Print Help
        std::cout << GetHelpString();
        return nullptr;
      case 'B':  // Time Begins
                 // std::cout << "> Begins\n";
        if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
        SetTimeBegins(time);
        return Command(end + 1);
      case 'E':  // Time Ends
                 // std::cout << "> Ends\n";
        if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
        SetTimeEnds(time);
        return Command(end + 1);
      case 'S':  // Time Started
                 // std::cout << "> Started\n";
        if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
        SetTimeStarted(time);
        return Command(end);
      case 'P':  // Time Stopped
                 // std::cout << "> Stopped\n";
        if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
        SetTimeStopped(time);
        return Command(end);
      case 'a':  // Assessment
                 // std::cout << "> Assessment\n";
        end = ParseFloat(input, &number);
        if (input == end) return "Invalid self assessment entered";
        if (!SetAssessment(number))
          return "Assessment must be in the range of 0.0 to 1.0";
        return Command(end);
      case 'd':  // Details
                 // std::cout << "> Details\n";
        value = StringLength(input + 1, '\"') + 1;
        buffer = new CH1[value];
        end = Parse(input + 1, &buffer[0], value, '\"');
        if (end == nullptr) return "Invalid details CH1";
        SetDetails(buffer);
        if (*end == 0) {
          std::cout << "\n> Task done\n";
          return nullptr;
        }
        return Command(end + 1);
      case 'g':  // Grade
                 // std::cout << "> Grade\n";
        end = ParseFloat(input, &number);
        if (input == end) return "Invalid grade entered";
        SetGrade(number);
        return Command(end + 1);
      case 's':  // Header
                 // std::cout << "> Header\n";
        value = StringLength(input + 1, '\"') + 1;
        buffer = new CH1[value];
        end = Parse(input + 1, &buffer[0], value, '\"');
        if (end == nullptr) return "Invalid summary CH1";
        SteadHeader(buffer);
        if (*end == 0) {
          std::cout << "\n> Task done\n";
          return nullptr;
        }
        return Command(end + 1);
      case 'v':  // Review
                 // std::cout << "> Review\n";
        value = StringLength(input + 1, '\"') + 1;
        buffer = new CH1[value];
        end = Parse(input + 1, &buffer[0], value, '\"');
        if (end == nullptr) return "Invalid review CH1";
        StealReview(buffer);
        if (*end == 0)  //< This might be a '\"' or '\0'
        {
          std::cout << "\n> Task done\n";
          return nullptr;
        }
        return Command(end + 1);
      case 'w':  // Weight
                 // std::cout << "> Weight\n";
        end = ParseFloat(input, &number);
        if (input == end) return "Invalid weight entered";
        if (!SetWeight(number)) return "Invalid weight entered";
        return Command(end);
    }
    return "Invalid Task argument";
  }
  return "Invalid Task command";
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
