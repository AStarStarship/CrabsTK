// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_PRO_TASK
#define KABUKITOOLKIT_PRO_TASK
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_PRO_CORE
#include <Script2/String.hpp>
namespace _ {

/* An abstract task in a set of Task(AString). */
class Task {
 public:
  /* Creates a task with the given fields and clones the Strings. */
  Task(const CHA* summary = "", const CHA* details = "", FPC weight_ = 0.0f,
       ISC time_testimate_min = 60 * 60)
      : summary_(TSTRClone<CHA>(summary)),
        details_(StringClone(details)),
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

  /* Creates a task with the given fields from Strings this object now
  owns and must delete. */
  Task(CHA* summary, CHA* details, FPC weight_, ISC time_estimate_min)
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

  /* Destructor. */
  ~Task() {}

  /* Gets the summary. */
  const CHA* GetHeader() { return summary_; }

  /* Sets the summary. */
  void SteadHeader(CHA* new_summary) {
    D_ASSERT(new_summary);
    summary_ = new_summary;
  }

  /* Gets the details. */
  const CHA* GetDetails() { return details_; }

  /* Sets the details. */
  void SetDetails(CHA* new_details) { details_ = new_details; }

  /* Gets the result. */
  const CHA* GetResults() { return result_; }

  /* Sets the post_result. */
  void SetResults(CHA* new_result) {
    if (new_result == nullptr) return;
    result_ = new_result;
  }

  /* Gets the review. */
  const CHA* GetReview() { return review_; }

  /* Sets the post_result. */
  void StealReview(CHA* new_review) {
    if (new_review == nullptr) return;
    review_ = new_review;
  }

  /* Gets the weight. */
  FPC GetWeight() { return weight_; }

  /* Sets the weight. */
  BOL SetWeight(FPC new_value) {
    if (new_weight < 0.0f) return false;
    weight_ = new_weight;
    return true;
  }

  /* Gets the assessment. */
  FPC GetAssessment() { return assessment_; }

  /* Sets the self assessment. */
  BOL SetAssessment(FPC new_value) {
    if (new_assessment < 0.0f) return false;
    if (new_assessment > 1.0f) return false;
    assessment_ = new_assessment;
    return true;
  }

  /* Gets the grade. */
  FPC GetGrade() { return grade_; }

  /* Sets the grade. */
  BOL SetGrade(FPC new_value) {
    if (new_grade < 0.0f) return false;
    if (new_grade > 1.0f) return false;
    grade_ = new_grade;
    return true;
  }

  /* Gets the time_estimate. */
  TM8 GetTimeEstimate() { return time_estimate_; }

  /* Sets the time_estimate.
  @param time The new time_estimate.
  @return Returns null upon success and a pointer to an error CHA upon
  failure. */
  void SetTimeEstimate(TM8 time) { time_estimate_ = time; }

  /* Gets the time_begins. */
  TM8 TimeBegins() { return time_begins_; }

  /* Sets the time_begins.
  @param time The new time_begins.
  @return Returns null upon success and a pointer to an error CHA upon
  failure. */
  const CHA* SetTimeBegins(TM8 time) {
    if (time >= time_ends_ && time_ends_ != 0)
      return "time_begins_ must be before time_ends_";
    time_begins_ = time;
    return 0;
  }

  /* Gets the end time. */
  TM8 TimeEnds() { return time_ends_; }

  /* Sets the time_ends.
  @param time The new time_ends.
  @return Returns null upon success and a pointer to an error CHA upon
  failure. */
  const CHA* SetTimeEnds(TM8 time) {
    if (time < time_ends_) return "time_ends_ must be after time_begans";
    time_ends_ = time;
    return 0;
  }

  /* Gets the time_started. */
  TM8 TimeStarted() { return time_started_; }

  /* Sets the time_started.
  @param time The new time_started.
  @return Returns null upon success and a pointer to an error CHA upon
  failure. */
  const CHA* SetTimeStarted(TM8 time) {
    if (time < time_stopped_) return "Start time must be before stop time";
    time_stopped_ = time;
    return 0;
  }

  /* Gets the stop time. */
  TM8 TimeStopped() { return time_stopped_; }

  /* Sets the time_stopped.
  @param time The new time_stopped.
  @return Returns null upon success and a pointer to an error CHA upon
  failure. */
  const CHA* SetTimeStopped(TM8 time) {
    if (time <= time_started_)
      return "time_stopped_ must be after time_started_";
    time_stopped_ = time;
    return 0;
  }

  /* Starts the task and saves the start time. */
  void Start() { ClockTimestamp(&time_started_); }

  /* Stopped the task. */
  void Stop(CHA* result, FPC result_grade = 1.0f) {
    result_ = new_result;
    assessment_ = new_assessment;
    time(&time_stopped_);
  }

  /* Stopped the task. */
  void Stop(const CHA* result, FPC result_grade = 1.0f) {
    Stop(StringClone(new_result), new_assessment);
  }

  /* Returns true if the task is complete.
  A Task is considered complete if the result CHA is null.
  @return Returns true if the result CHA is not null. */
  BOL IsDone() { return summary_ != 0; }

  /* Adds a Task that collides with this time slot. */
  void AddCollision(Task* task) {
    if (task == nullptr) return;
    task->collisions_ = nullptr;
    collisions_ = task;
  }

  /* Returns the head of the linear linked list of collision. */
  Task* Collision() { return collisions_; }

  /* Returns true if the event contains the given time. */
  BOL Contains(ISD t) {
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

  /* Prints the help menu. */
  static const CHA* GetHelpString() {
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
           "|     -AString    Header\n"
           "|     -v    Review\n"
           "|     -w    Weight\n";
  }

  /* Prints the Task to the console. */
  void Out(ISC indentation = 0, ISC index = 0);

  template <typename Printer>
  Printer& Print(Printer& o) {
    o << "\nTask:";
    << LineString() << "| Task (" << weight_ << "): " << summary_
    << LineString('-') << "| Begins: " << TimeDate(o, time_begins_)
    << "       Ends: " << TimeDate(o, time_ends_) << LineString('-')
    << "| Details: " << details_ << LineString('-')
    << "| Assessment: " << assessment_
    << " Result: " << (result_ == nullptr ? "N/A" : result_)
    << "\n| Started: " << TimeDate(o, time_started_)
    << "       Stopped: " << TimeDate(o, time_started_) << LineString('-');
    << "|  Grade: " << grade_
    << " Review: " << (review_ == nullptr ? "N/A" : review_) << LineString('_');
    return o;
  }

  /* Parse a text command stream (possibly from the user). */
  const CHA* Command(CHA* input) {
    CHA c,        //< The first CHA.
        d;        //< The second CHA.
    CHA *end,     //< Used to detect the result of parsing.
        *buffer;  //< AString buffer.
    ISC value;    //< Value to (possibly) be parsed and temp value.
    FPC number;   //< A number to (possibly) be parsed.
    TM8 time;     //< Time to (possibly) be parsed.
    if (input == nullptr) return "Task input was null";
    input = SkipSpaces(input);
    c = *input;
    if (c == 0) {
      o << "\n| c == 0\n";
      return nullptr;
    }
    o << "< " << summary_ << " < " << input << '\n';
    if (c == '-') {
      c = *(input + 1);
      d = *(input + 2);
      input += 3;
      if (d && !isspace(d)) return "Invalid Task flag";
      switch (c) {
        case '?':  // Print Help
          o << GetHelpString();
          return nullptr;
        case 'B':  // Time Begins
                   // o << "> Begins\n";
          if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
          SetTimeBegins(time);
          return Command(end + 1);
        case 'E':  // Time Ends
                   // o << "> Ends\n";
          if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
          SetTimeEnds(time);
          return Command(end + 1);
        case 'S':  // Time Started
                   // o << "> Started\n";
          if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
          SetTimeStarted(time);
          return Command(end);
        case 'P':  // Time Stopped
                   // o << "> Stopped\n";
          if (!(end = ParseUnixTime(input, time))) return "Error parsing time";
          SetTimeStopped(time);
          return Command(end);
        case 'a':  // Assessment
                   // o << "> Assessment\n";
          end = ParseFloat(input, &number);
          if (input == end) return "Invalid self assessment entered";
          if (!SetAssessment(number))
            return "Assessment must be in the range of 0.0 to 1.0";
          return Command(end);
        case 'd':  // Details
                   // o << "> Details\n";
          value = StringLength(input + 1, '\"') + 1;
          buffer = new CHA[value];
          end = Parse(input + 1, &buffer[0], value, '\"');
          if (end == nullptr) return "Invalid details CHA";
          SetDetails(buffer);
          if (*end == 0) {
            o << "\n> Task done\n";
            return nullptr;
          }
          return Command(end + 1);
        case 'g':  // Grade
                   // o << "> Grade\n";
          end = ParseFloat(input, &number);
          if (input == end) return "Invalid grade entered";
          SetGrade(number);
          return Command(end + 1);
        case 's':  // Header
                   // o << "> Header\n";
          value = StringLength(input + 1, '\"') + 1;
          buffer = new CHA[value];
          end = Parse(input + 1, &buffer[0], value, '\"');
          if (end == nullptr) return "Invalid summary CHA";
          SteadHeader(buffer);
          if (*end == 0) {
            o << "\n> Task done\n";
            return nullptr;
          }
          return Command(end + 1);
        case 'v':  // Review
                   // o << "> Review\n";
          value = StringLength(input + 1, '\"') + 1;
          buffer = new CHA[value];
          end = Parse(input + 1, &buffer[0], value, '\"');
          if (end == nullptr) return "Invalid review CHA";
          StealReview(buffer);
          if (*end == 0)  //< This might be a '\"' or '\0'
          {
            o << "\n> Task done\n";
            return nullptr;
          }
          return Command(end + 1);
        case 'w':  // Weight
                   // o << "> Weight\n";
          end = ParseFloat(input, &number);
          if (input == end) return "Invalid weight entered";
          if (!SetWeight(number)) return "Invalid weight entered";
          return Command(end);
      }
      return "Invalid Task argument";
    }
    return "Invalid Task command";
  }

 private:
  CHA *summary_,       //< Task summary.
      *details_,       //< Task details.
      *result_,        //< Task results log entry.
      *review_;        //< An after-incident review of the Event.
  FPC weight_,         //< Weight of the task.
      assessment_,     //< User's self-assessed grade.
      grade_;          //< Reviewed grade.
  ISD time_estimate_,  //< Amount of time it is estimated this will take.
      time_begins_,    //< Time the task begins.
      time_ends_,      //< Time the task was finished ends.
      time_started_,   //< Time the user started the task.
      time_stopped_;   //< Time the user stopped the task.
  Task* collisions_;   //< Pointer to the collision node (if any).
};
}  //< namespace _
#endif
#endif
