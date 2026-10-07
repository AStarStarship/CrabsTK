// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_PRO_PROJECT
#define CRABSTK_PRO_PROJECT
#include <_Config.h>
#if SEAM >= CRABSTK_PRO
#include "Schedule.hpp"
#include "Task.hpp"
namespace _ {

/* A tree-like Project in the system that can be scheduled.
An Project can be a person or virtual entity. An Project is identified by
their unique key. */
class Project : public Operand {
 public:
  enum {
    INIT_NUM_SCHEDULES = 4,           //< Initial number of Schedule(AString).
    MAX_SCHEDULES = 32,               //< Max number of Schedule(AString).
    MAX_SUMMARY_LENGTH = 256,         //< The max readme CHA length.
    MAX_DETAIL_LENGTH = 1024 * 1024,  //< The max detail CHA length.
  };

  /* Pops a Project pointer off the stack. */
  static Project* Pop (Stack<Project*>* stack, const CHA* result) {
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
    o << "Popping " << stack->project->Key () << kLF
      << "new_top: " << new_top->project->Key () << kLF;
    delete stack;
    return new_top;
  }

  /* Pushes a Project pointer onto the stack. */
  static const CHA* Push (ProjectNode*& stack, Project* project) {
    if (stack == nullptr) return "Null push stack";
    if (stack->project == project)  // Then pop the project off the stack.
      return "";
    o << "> Pushing " << project->Key () << kLF;
    ProjectNode* temp = new ProjectNode (project, stack);
    stack = temp;
    return nullptr;
  }

  /* Default constructor initializes list with the given or left key. */
  Project(const CHA* key = "Unnamed", const CHA* readme = "")
    : key_ (key == nullptr ? StringClone ("") : key),
    readme_ (readme == nullptr ? StringClone ("") : readme),
    task_ (nullptr) {}

  /* Constructor initializes with stolen key and readme. */
  Project(CHA* key, CHA* readme)
    : key_ (key), readme_ (readme), task_ (nullptr) {}

  /* Destructor. */
  ~Project() {}

  /* Gets the key. */
  const TString<>& Key() { return key_; }

  /* Sets the key. */
  void StealKey(CHA* new_key);

  /* Clones the given key. */
  void SetKey (const CHA* new_key) { key_.Set (new_key); }

  /* Gets the readme. */
  const TString<>& GetReadMe() { return readme_; }

  /* Sets the readme to a clone of the new_readme and deletes the old one.. */
  void SetReadme (const CHA* new_readme) {
    readme_.Set (new_readme);
  }

  /* Adds a Project.
  @param project The project to add.
  @return Returns false if the key is not a token. */
  BOL AddProject(const CHA* key) {
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
  BOL AddSchedule(const CHA* key) {
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
    o << LineString ();
      << "|\n"
      "| Objs in Scope:\n"
      "|\n"
      "| Projects: "
      << projects_->count << kLF;
    for (ISC i = 0; i < projects_->count; ++i)
      o << "| " << i << ".) "
      << StackGet<Project*> (projects_, i)->Key () << kLF;
    o << "|\n"
      "| Schedules: "
      << schedules_->count << kLF;
    for (ISC i = 0; i < schedules_->count; ++i)
      o << "| " << i << ".) "
      << schedules_[i]->Key () << kLF;
    return o << LineString ('_');
  }

  /* Searches for a Schedule with the given key. */
  ISC ScheduleIndex(const CHA* key) {
    if (key == nullptr) return false;
    //  Search for an already existing key to add it to:
    for (ISC i = 0; i < schedules_->count; ++i)
      if (CompareTokenString (key, StackGet<Schedule*> (schedules_, i)->Key ()))
        return i;
    return -1;
  }

  /* Gets the Schedule with the given key.
  @param key The key of the Schedule to search for.
  @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(ISC index) {
    if (index < 0) return nullptr;
    if (index >= schedules_->count) return nullptr;
    return schedules_[index];
  }

  /* Gets the Schedule with the given key.
  @param key The key of the Schedule to search for.
  @return Returns null if calendar does not contain the given key. */
  Schedule* GetSchedule(const CHA* key) {
    return schedules_[ScheduleIndex (key)];
  }

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(ISC index) {
    return StackRemove<Schedule*> (schedules_, index);
  }

  /* Attempts to remove the Schedule at the given index. */
  BOL RemoveSchedule(const CHA* key) {
    return RemoveSchedule (ScheduleIndex (key));
  }

  /* Searches for a Project with the given key. */
  ISC ProjectIndex(const CHA* key) {
    if (key == nullptr) return -1;
    //  Search for an already existing key to add it to:
    for (ISC i = 0; i < projects_->count; ++i)
      if (CompareTokenString (key, StackGet<Project*> (projects_, i)->Key ()))
        return i;
    return -1;
  }

  /* Gets the Project with the given key.
  @param key The key of the Project to search for.
  @return Returns null if calendar does not contain the given key. */
  Project* GetProject(ISC index) {
    if (index < 0) return nullptr;
    if (index >= projects_->count) return nullptr;
    return StackGet<Project*> (projects_, index);
  }

  /* Searches for the given project key.
  @param  key The key of the Project to search for.
  @return Returns null if calendar does not contain the given key. */
  Project* GetProject(const CHA* key) {
    return StackGet<Project*> (projects_, ProjectIndex (key));
  }

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(ISC index) {
    return StackRemove<Project*> (projects_, index);
  }

  /* Attempts to remove the Project at the given index. */
  BOL RemoveProject(const CHA* key) {
    return RemoveProject (ProjectIndex (key));
  }

  /* Selects the task at the given index. */
  BOL Select(ISC index) {
    if (index < 0) return false;
    if (index >= schedules_->count) return false;
    task_ = StackGet<Schedule*> (schedules_, index);
    return true;
  }

  /* De-serializes the file with the stored key.
  @return Returns 0 upon success and a pointer to an error CHA upon
  failure. */
  void Load() {
    /*
    const CHA* error;  //< Error flag.
    o << "\n\n| Reading from " << key << kLF;
    std::ifstream file (key);
    if (!file.is_open ())
    {
    o << "Unable to open file for reading";
    return;
    }
    CHA buffer[MAX_String_LENGTH];
    file.getline (buffer, MAX_String_LENGTH, ',');
    //< Throw away till the comma..
    ISC num_categories;
    file >> num_categories;
    //o << num_categories << " categories.\n";
    file.getline (buffer, MAX_String_LENGTH, kLF);
    //< Throw away the rest of the line.
    // Read each of the ItemList categories.
    for (ISC i = 0; i < num_categories; ++i)
    {
    CHA* key = new CHA[MAX_String_LENGTH];
    file.getline (key, MAX_String_LENGTH, ',');
    //o << "| key: " << key;
    ISC num_items;
    file >> num_items;
    //o << " num_items: " << num_items << kLF;
    while (file.get () != kLF);  //< Throw away the rest of the line.
    for (ISC i = 0; i < num_items; ++i)
    {
    CHA* name = new CHA[MAX_String_LENGTH];
    file.getline (name, MAX_String_LENGTH, ',');
    ISC quantity;
    FPC price;
    CHA c;
    //file >> quantity >> c >> price >> c;
    file >> quantity;
    while (file.get () != ',');
    file >> price;
    while (file.get () != kLF);
    add (key, name, quantity, price);
    }
    :}
    file.close ();
    */
  }

  /* Serializes the list to the given file.
  @return Returns 0 upon success and a pointer to an error CHA upon
  failure. */
  void Save() {
    /*
    o << "\n\n| Writing to " << key << "...\n";
    const CHA* error;
    std::ofstream file;
    file.open (key);
    if (!file.is_open ())
    {
    o << "Unable Top open file for writing";
    return;
    }
    file << "Project," << element_count << ',' << get_total () << kLF;
    ItemList** list = lists;
    for (ISC i = 0; i < element_count; ++i)
    list[i]->write_to_open_file (file);
    file.close ();
    */
  }

  /* Gets the help CHA. */
  static const CHA* GetAppHelpString() {
    return "\n| CT.pro.Project Help:\n"
      "| There are three primary classes for use:\n"
      "|\n"
      "| Task     - A general purpose todo task.\n"
      "| Schedule - A collection of tasks that can repeat a specified\n"
      "|            number of times.\n"
      "| Project  - A tree structure composed of a collection of\n"
      "|            Schedule(AString) and a collection of sub-projects.\n"
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
      "| Examples: subproject example-schedule -AString \"A task\" -b "
      "3:30PM'\n"
      "|           class-schedule -a 1.0 -AString \"Homework\" -d \"Program "
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

  /* Gets the help CHA. */
  static const CHA* GetHelpString() {
    return "\n| Project Commands: \n"
      "|    -S  Adds a new Schedule and selects it.\n"
      "|        Examples: > -S new-schedule\n"
      "|\n"
      "|    -P  Adds a new Project and pushes it onto the stack.\n"
      "|        Args: key:CHA\n"
      "|        Examples: > -P new-project\n"
      "|\n"
      "|    -l  Lists the current item's in scope to the console.\n"
      "|        Args: none\n"
      "|\n"
      "|    -p  Prints out object with the given handle.\n"
      "|        Example: > task_name -p\n"
      "|\n"
      "|    -r  Removes the object at the given index\n"
      "|        Args: index:ISC or key:CHA\n"
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
  Printer& Print(Printer& o, ISC indent_level = 0, CHA bullet_type = '1',
           ISC spaces_per_tab = 4) {
    o << LineString ()
      << "\nProject: " << key_
      << "\n| Projects: " << projects_->count << "|\n";
    for (ISC i = 0; i < projects_->count; ++i) {
      Ar4Get<Project*> (projects_, i)->Print (o);
    }
    o << "| Schedules: " << schedules_->count << "|\n";
    for (ISC i = 0; i < schedules_->count; ++i) {
      Ar4Get<Schedule*> (schedules_, i)->Print (o);
    }
    return o << LineString ('_');
  }

  /* ASCIICrabs operations. */
  virtual const Op* Star (CHN index, Expr* expr) {
    return nullptr;
  }

 private:
  TString<> key_,             //< Entity key.
      readme_;                //< Project readme.
  Schedule* task_;            //< Current Schedule being edited.
  TStack<Project> schedules_; //< Composition of Schedule(AString).
  TStack<Schedule> projects_; //< Child projects.
};

}  //< namespace _
#endif
#endif
