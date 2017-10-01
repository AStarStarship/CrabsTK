/** CS 162 Program 4-5
    @file   task.h
    @author Cale McCollough
    @brief  File contains the Task class.
*/

#ifndef KABUKI_PRO_TASK_H
#define KABUKI_PRO_TASK_H

#include <time.h>

namespace _pro {

/** An abstract task in a set of Task(s). */
class Task
{
    public:
    
    /** Constructs a task with the given fields and clones the strings. */
    Task (const char* summary = "", const char* details = "",  
          float weight = 0.0f, int time_testimate_min = 60 * 60);

    /** Constructs a task with the given fields from strings this object now 
        owns and must delete. */
    Task (char* summary, char* details, float weight, int time_estimate_min);
    
    /** Destructor. */
    ~Task ();

    /** Gets the summary. */
    const char* GetHeader ();

    /** Sets the summary. */
    void SteadHeader (char* new_summary);

    /** Gets the details. */
    const char* GetDetails ();

    /** Sets the details. */
    void SetDetails (char* new_details);
    
    /** Gets the result. */
    const char* GetResults ();
    
    /** Sets the post_result. */
    void SetResults (char* new_result);
    
    /** Gets the review. */
    const char* GetReview ();
    
    /** Sets the post_result. */
    void StealReview (char* new_review);

    /** Gets the weight. */
    float GetWeight ();
    
    /** Sets the weight. */
    bool SetWeight (float new_value);

    /** Gets the assessment. */
    float GetAssessment ();
    
    /** Sets the self assessment. */
    bool SetAssessment (float new_value);

    /** Gets the grade. */
    float GetGrade ();
    
    /** Sets the grade. */
    bool SetGrade (float new_value);
    
    /** Gets the time_estimate. */
    time_t GetTimeEstimate ();
    
    /** Sets the time_estimate.
        @param time The new time_estimate.
        @return Returns null upon success and a pointer to an error string upon failure. */
    void SetTimeEstimate (time_t time);
    
    /** Gets the time_begins. */
    time_t GetTimeBegins ();
    
    /** Sets the time_begins.
        @param time The new time_begins.
        @return Returns null upon success and a pointer to an error string upon failure. */
    const char* SetTimeBegins (time_t time);
    
    /** Gets the end time. */
    time_t GetTimeEnds ();
    
    /** Sets the time_ends.
        @param time The new time_ends.
        @return Returns null upon success and a pointer to an error string upon failure. */
    const char* SetTimeEnds (time_t time);
    
    /** Gets the time_started. */
    time_t GetTimeStarted ();
    
    /** Sets the time_started.
        @param time The new time_started.
        @return Returns null upon success and a pointer to an error string upon failure. */
    const char* SetTimeStarted (time_t time);
    
    /** Gets the stop time. */
    time_t GetTimeStopped ();
    
    /** Sets the time_stopped.
        @param time The new time_stopped.
        @return Returns null upon success and a pointer to an error string upon failure. */
    const char* SetTimeStopped (time_t time);

    /** Starts the task and saves the start time. */
    void Start ();

    /** Stopped the task. */
    void Stop (char* result, float result_grade = 1.0f);

    /** Stopped the task. */
    void Stop (const char* result, float result_grade = 1.0f);
    
    /** Returns true if the task is complete.
        A Task is considered complete if the result string is null.
        @return Returns true if the result string is not null. */
    bool IsDone ();

    /** Adds a Task that collides with this time slot. */
    void AddCollision (Task* task);

    /** Returns the head of the linear linked list of collision. */
    Task* Collision ();

    /** Returns true if the event contains the given time. */
    bool Contains (time_t t);
    
    /** Prints the help menu. */
    static const char* GetHelpString ();
    
    /** Prints the Task to the console. */
    void Print (int indentation = 0, int index = 0);

    /** Parse a text command stream (possibly from the user). */
    const char* Command (char* input);

    private:
    
    char* summary_,              //< The task summary.
        * details_,              //< The task details.
        * result_,               //< The task results log entry.
        * review_;               //< An after-incident review of the Event.
    float weight_,               //< The weight of the task.
          assessment_,           //< The user's self-assessed grade.
          grade_;                //< The reviewed grade.
    time_t time_estimate_,       //< The amount of time it is estimated this will take.
           time_begins_,         //< The time the task begins.
           time_ends_,           //< The time the task was finished ends.
           time_started_,        //< The time the user started the task.
           time_stopped_;        //< The time the user stopped the task.
    Task* collisions_;            //< Pointer to the collision node (if any).
};
}       //< namespace _pro
#endif  //< KABUKI_PRO_TASK_H

