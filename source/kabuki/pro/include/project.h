/** CS 162 Program 4-5
    @file   project_manager.h
    @author Cale McCollough
    @brief  File contains the header for the _pro::Manager
*/

#ifndef KABUKI_PRO_PROJECT_H
#define KABUKI_PRO_PROJECT_H

#include "array.h"
#include "task.h"
#include "schedule.h"

namespace _pro {

class Project;

/** A linear linked list node for creating a stack of Project. */
struct ProjectNode
{
    Project* project;
    ProjectNode* prev;
    
    /** Constructs a node that points to the previous node. */
    ProjectNode (Project* project, ProjectNode* prev = nullptr);

    /** Destructor. */
    ~ProjectNode ();

    /** Prints the stack key's in a horizontal link. */
    void Print ();
};

/** Pops a ProjectNode* off the stack. */
ProjectNode* Pop (ProjectNode* stack, const char* result);

/** Pushes a ProjectNode* off the stack. */
const char* Push (ProjectNode*& stack, Project* project);

/** A tree-like Project in the system that can be scheduled.
    An Project can be a person or virtual entity. An Project is identified by their 
    unique key. */
class Project
{
    public:
     
    enum {
        INIT_NUM_SCHEDULES = 4,           //< The initial number of Schedule(s).
        MAX_SCHEDULES      = 32,          //< Max number of Schedule(s).
        MAX_SUMMARY_LENGTH = 256,         //< The max readme string length.
        MAX_DETAIL_LENGTH  = 1024 * 1024, //< The max detail string length.
    };
    
    /** Default constructor initializes list with the given or left key. */
    Project (const char* key = "Unnamed", const char* readme = "");
    
    /** Constructor initializes with stolen key and readme. */
    Project (char* key, char* readme);
    
    /** Destructor. */
    ~Project ();

    /** Gets the key. */
    const char* GetKey ();
    
    /** Sets the key. */
    void StealKey (char* new_key);
    
    /** Clones the given key. */
    void SetKey (const char* new_key);

    /** Gets the readme. */
    const char* GetReadMe ();
    
    /** Sets the readme and deletes the current one. */
    void StealReadMe (char* new_readme);
    
    /** Sets the readme to a clone of the new_readme and deletes the old one.. */
    void CloneReadMe (const char* new_readme);

    /** Adds a Project.
        @param project The project to add. 
        @return Returns false if the key is not a token. */
    bool AddProject (const char* key);

    /** Adds a Project.
        @param project The project to add. 
        @return Returns false if the key is not a token. */
    bool AddProject (Project* project);

    /** Adds a Schedule.
        @param key The key of the new schedule to add.
        @return Returns false if the key is not a token. */
    bool AddSchedule (const char* key);

    /** Adds a currently existing Schedule.
        @param schedule A new schedule this object now owns the memory of.
        @return Returns false if the key is not a token. */
    bool AddSchedule (Schedule* schedule);

    /** Lists the keys in the given scope. */
    void ListObjects ();

    /** Searches for a Schedule with the given key. */
    int ScheduleIndex (const char* key);

    /** Gets the Schedule with the given key.
        @param key The key of the Schedule to search for.
        @return Returns null if calendar does not contain the given key. */
    Schedule* GetSchedule (int index);

    /** Gets the Schedule with the given key.
        @param key The key of the Schedule to search for.
        @return Returns null if calendar does not contain the given key. */
    Schedule* GetSchedule (const char* key);

    /** Attempts to remove the Schedule at the given index. */
    bool RemoveSchedule (int index);

    /** Attempts to remove the Schedule at the given index. */
    bool RemoveSchedule (const char* key);
     
    /** Searches for a Project with the given key. */
    int ProjectIndex (const char* key);

    /** Gets the Project with the given key.
        @param key The key of the Project to search for.
        @return Returns null if calendar does not contain the given key. */
    Project* GetProject (int index);

    /** Searches for the given project key.
        @param  key The key of the Project to search for.
        @return Returns null if calendar does not contain the given key. */
    Project* GetProject (const char* key);

    /** Attempts to remove the Project at the given index. */
    bool RemoveProject (int index);

    /** Attempts to remove the Project at the given index. */
    bool RemoveProject (const char* key);
     
    /** Selects the task at the given index. */
    bool Select (int index);
 
    /** Deserializes the file with the stored key.
        @return Returns 0 upon success and a pointer to an error string upon 
                failure. */
    void Load ();
     
    /** Serializes the list to the given file.
        @return Returns 0 upon success and a pointer to an error string upon 
                failure. */
    void Save ();

    /** Gets the help string. */
    static const char* GetAppHelpString ();

    /** Gets the help string. */
    static const char* GetHelpString ();
        
    /** Prints the Shopping list to the console.
        @param indent_level The number of tabs of the indentation level.
        @param bullet_type The type of bullet to print: numbers, upper case 
                           letters, lower case letters, stars, dashes etc.
        @param spaces_per_tab The number of spaces in a tab.
    */
    void Print (int indent_level = 0, char bullet_type = '1', 
                int spaces_per_tab = 4);
    
    /** Parses keyboard input from the user.
        @param input Usually is the keyboard input from the user. 
        @param stack Reference to the stack pointer to allow the project to
                     push and pop off of the stack. */
    const char* Command (char* input, ProjectNode*& stack);

    private:
     
    char* key_,                   //< The entity key.
        * readme_;                //< The project readme.
    Schedule* task_;              //< The current Schedule being edited.
    Array<Schedule*> schedules_;  //< A composition of Schedule(s).
    Array<Project*> projects_;    //< Child projects
};

}       //  namespace _pro
#endif  //< KABUKI_PRO_MANAGER_H

