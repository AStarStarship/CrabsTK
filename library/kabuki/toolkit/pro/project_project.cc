/** kabuki::pro
    @version 0.x
    @file    ~/libraries/pro/impl/project.cc
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

#include "project.h"

using namespace _;
using namespace kabuki::data;

namespace kabuki { namespace pro {

Project* Pop (_::Array<Project*>* stack, const char* result) {
    if (stack == nullptr)
        return nullptr;
    if (result == nullptr)
        return stack;
    if (strcmp ("", result)) {
        std::cout << "> " << result << ".\n";
        return stack;
    }
    std::cout << "> Pop!\n";
    ProjectNode* new_top = stack->prev;
    if (new_top == nullptr) {
        std::cout << "> Exiting application...\n";
        return nullptr;
    }
    std::cout << "Popping " << stack->project->GetKey () << '\n'
        << "new_top: " << new_top->project->GetKey () << '\n';
    delete stack;
    return new_top;
}

const char* Push (ProjectNode*& stack, Project* project) {
    if (stack == nullptr)
        return "Null push stack";
    if (stack->project == project)  // Then pop the project off the stack.
        return "";
    std::cout << "> Pushing " << project->GetKey () << '\n';
    ProjectNode* temp = new ProjectNode (project, stack);
    stack = temp;
    return nullptr;
}

Project::Project (char* key, char* readme) :
    key_ (key == nullptr?StringClone (""):key),
    readme_ (readme == nullptr ? StringClone (""):readme),
    task_ (nullptr) {
}

Project::Project (const char* key, const char* readme) :
    key_ (StringClone (key)),
    readme_ (StringClone (readme)),
    task_ (nullptr) {
}

Project::~Project () {
}

void Project::ListObjects () {
    PrintBreak ();
    std::cout << "|\n"
        "| Objects in Scope:\n"
        "|\n"
        "| Projects: " << projects_->count << '\n';
    for (int i = 0; i < projects_->count; ++i)
        std::cout << "| " << i << ".) " << ArrayGet<Project*> (projects_, i)->GetKey () << '\n';
    std::cout << "|\n"
        "| Schedules: " << schedules_->count << '\n';
    for (int i = 0; i < schedules_->count; ++i)
        std::cout << "| " << i << ".) " << ArrayGet<Schedule*> (schedules_, i)->GetKey () << '\n';
    PrintBreak ("|", '_');
}

const char* Project::GetKey () {
    return key_;
}

void Project::StealKey (char* new_name) {
    if (new_name == nullptr)
        return;
    delete key_;
    key_ = new_name;
}

void Project::SetKey (const char* new_name) {
    StealKey (StringClone (new_name));
}

const char* Project::GetReadMe () {
    return readme_;
}

void Project::StealReadMe (char* new_readme) {
    if (new_readme == nullptr)
        return;
    readme_ = new_readme;
}

void Project::CloneReadMe (const char* new_readme) {
    StealReadMe (StringClone (new_readme));
}

bool Project::AddProject (const char* key) {
    if (!IsToken (key))
        return false;
    ArrayPush<Project*> (projects_, new Project (key));
    return true;
}

bool Project::AddProject (Project* project) {
    if (!project)
        return false;
    ArrayPush<Project*> (projects_, project);
    return true;
}

bool Project::AddSchedule (const char* key) {
    if (!IsToken (key))
        return false;
    ArrayPush<Schedule*> (schedules_, new Schedule (key));
    return true;
}

bool Project::AddSchedule (Schedule* schedule) {
    if (!schedule)
        return false;
    ArrayPush<Schedule*> (schedules_, schedule);
    return true;
}

Schedule* Project::GetSchedule (int index) {
    if (index < 0)
        return nullptr;
    if (index >= schedules_->count)
        return nullptr;
    return ArrayGet<Schedule*> (schedules_, index);
}

int Project::ScheduleIndex (const char* key) {
    if (key == nullptr)
        return false;
    //  Search for an already existing key to add it to:
    for (int i = 0; i < schedules_->count; ++i)
        if (CompareTokenString (key, ArrayGet<Schedule*> (schedules_, i)->GetKey ()))
            return i;
    return -1;
}

Schedule* Project::GetSchedule (const char* query) {
    return schedules_[ScheduleIndex (query)];
}

bool Project::RemoveSchedule (int index) {
    return ArrayRemove<Schedule*> (schedules_, index);
}

bool Project::RemoveSchedule (const char* key) {
    return RemoveSchedule (ScheduleIndex (key));
}

int Project::ProjectIndex (const char* key) {
    if (key == nullptr)
        return -1;
    //  Search for an already existing key to add it to:
    for (int i = 0; i < projects_->count; ++i)
        if (CompareTokenString (key, ArrayGet<Project*> (projects_, i)->GetKey ()))
            return i;
    return -1;
}

Project* Project::GetProject (int index) {
    if (index < 0)
        return nullptr;
    if (index >= projects_->count)
        return nullptr;
    return  ArrayGet<Project*> (projects_, index);
}

Project* Project::GetProject (const char* key) {
    return ArrayGet<Project*> (projects_, ProjectIndex (key));
}

bool Project::RemoveProject (int index) {
    return ArrayRemove<Project*> (projects_, index);
}

bool Project::RemoveProject (const char* key) {
    return RemoveProject (ProjectIndex (key));
}

bool Project::Select (int index) {
    if (index < 0) return false;
    if (index >= schedules_->count) return false;
    task_ = ArrayGet<Schedule*> (schedules_, index);
    return true;
}

void Project::Load () {
    /*
    const char* error;  //< Error flag.
    std::cout << "\n\n| Reading from " << key << '\n';
    std::ifstream file (key);
    if (!file.is_open ())
    {
    std::cout << "Unable to open file for reading";
    return;
    }
    char buffer[MAX_STRING_LENGTH];
    file.getline (buffer, MAX_STRING_LENGTH, ',');
    //< Throw away till the comma..
    int num_categories;
    file >> num_categories;
    //std::cout << num_categories << " categories.\n";
    file.getline (buffer, MAX_STRING_LENGTH, '\n');
    //< Throw away the rest of the line.
    // Read each of the ItemList categories.
    for (int i = 0; i < num_categories; ++i)
    {
    char* key = new char[MAX_STRING_LENGTH];
    file.getline (key, MAX_STRING_LENGTH, ',');
    //std::cout << "| key: " << key;
    int num_items;
    file >> num_items;
    //std::cout << " num_items: " << num_items << '\n';
    while (file.get () != '\n');  //< Throw away the rest of the line.
    for (int i = 0; i < num_items; ++i)
    {
    char* name = new char[MAX_STRING_LENGTH];
    file.getline (name, MAX_STRING_LENGTH, ',');
    int quantity;
    float price;
    char c;
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

void Project::Save () {
    /*
    std::cout << "\n\n| Writing to " << key << "...\n";
    const char* error;
    std::ofstream file;
    file.open (key);
    if (!file.is_open ())
    {
    std::cout << "Unable top open file for writing";
    return;
    }
    file << "Project," << num_elements << ',' << get_total () << '\n';
    ItemList** list = lists;
    for (int i = 0; i < num_elements; ++i)
    list[i]->write_to_open_file (file);
    file.close ();
    */
}

const char* Project::GetAppHelpString () {
    return "\n| Kabuki::_pro::Project Help:\n"
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
        "| Examples: subproject example-schedule -string \"A task\" -b 3:30PM'\n"
        "|           class-schedule -a 1.0 -string \"Homework\" -d \"Program 5\n"
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

const char* Project::GetHelpString () {
    return "\n| Project Commands: \n"
        "|    -S  Adds a new Schedule and selects it.\n"
        "|        Examples: > -S new-schedule\n"
        "|\n"
        "|    -P  Adds a new Project and pushes it onto the stack.\n"
        "|        Args: key:char\n"
        "|        Examples: > -P new-project\n"
        "|\n"
        "|    -l  Lists the current item's in scope to the console.\n"
        "|        Args: none\n"
        "|\n"
        "|    -p  Prints out object with the given handle.\n"
        "|        Example: > task_name -p\n";
    "|\n"
        "|    -r  Removes the object at the given index\n"
        "|        Args: index:int or key:char\n"
        "|        Examples: > -r 1\n"
        "|                  > -r project-name\n"
        "|\n"
        "|    CR  Pressing enter (ASCII CR) pops the currently selected\n"
        "|        project off of the stack of projects till it gets to\n"
        "|        the root project, then it exits the program.\n"
        "|        Args: none\n"
        "|\n";
}

void Project::Print (int indent_level, char bullet_type, int spaces_per_tab) {
    PrintBreak ();
    std::cout << "| Project: " << key_;
    PrintBreak ("\n|");
    std::cout << "| Projects: " << projects_->count
        << "|\n";
    for (int i = 0; i < projects_->count; ++i) {
        Ar4Get<Project*> (projects_, i)->Print ();
    }
    std::cout << "| Schedules: " << schedules_->count
        << "|\n";
    for (int i = 0; i < schedules_->count; ++i) {
        Ar4Get<Schedule*> (schedules_, i)->Print ();
    }
    PrintBreak ("|", '_');
}

const char* Project::Command (char* input, Array<Project*>* stack) {
    char c,  //< The first flag char.
         d,  //< The second flag char.
         e;  //< The third flag char.
    char* end;
    if (input == nullptr)
        return "Null Project command";
    input = SkipSpaces (input);

    int value;       //< An index to possibly be scanned.
    c = *input;
    if (c == '-') {
        d = *(input + 1);
        e = *(input + 2);
        std::cout << " >" << key_ << " > \"" << c << d << e << "\" < " << input << '\n';
        if (e && !isspace (e))
            return "Invalid Project flag";
        input += 3;
        switch (d) {
            case 'S': // Add to the schedule
            {
                if (!e)
                    return "You must enter a Schedule name";
                std::cout << "> Adding Schedule: " << input << '\n';
                end = TextTokenEnd (input);
                d = *input;
                //std::cout << "> d: " << d << '\n';
                if (!d || input == end)
                    return "You must enter a valid key";
                if (ScheduleIndex (input) >= 0 || ScheduleIndex (input) >= 0)
                    return "Key is in use. Type -l to list the objects in the current scope.";
                Schedule* string = new Schedule (input);
                ArrayPush<Schedule*> (schedules_, string);
                return nullptr;
            }
            case 'P':  // Add new Project
            {
                if (!e)
                    return "You must enter a Project name";
                std::cout << "> Adding Project: " << input << '\n';
                end = TextTokenEnd (input);
                d = *input;
                if (!d || input == end)
                    return Push (stack, this);
                if (ProjectIndex (input) >= 0 || ProjectIndex (input) >= 0)
                    return "Key is in use. Type -l to list the objects in the current scope";
                Project* p = new Project (input);
                ArrayPush<Project*> (projects_, p);
                return nullptr;
            }
            case 'l':    //< List Objects
            {
                ListObjects ();
                return nullptr;
            }
            case 'p':
            {
                //std::cout << "> Printing ";
                Print ();
                return nullptr;
            }
            case 'r':    //< Remove
            {
                std::cout << "> Removing ";
                d = *(++input);
                if (!isspace (d))
                    return "Invalid flag";
                d = *(++input);
                input = SkipSpaces (input);
                d = *input;
                if (d == 0)
                    return "Invalid flag argument";
                if (isdigit (d)) {
                    if (!sscanf (input, "%i", &value)) //< Shouldn't happen.
                        return "Error scanning index to remove";
                    std::cout << value << '\n';
                    if (!RemoveSchedule (value))
                        return "Invalid schedule index";
                } else {
                    // Search for a project with the given key.
                    end = TextTokenEnd (input);
                    if (end == input)
                        return "You must input a valid Project or Schedule "
                        "key";
                    value = ProjectIndex (input);
                    if (value < 0) {
                        value = ScheduleIndex (input);
                        if (value < 0)
                            return "Invalid project index";
                        RemoveSchedule (value);
                    } else {
                        RemoveProject (value);
                    }
                }
                return nullptr;
            }
        }
        return "Invalid Project flag";
    } else if (isdigit (c)) {
        // Because we checked for the -flag before this, we'll never have a 
        // negative number!
        if (!sscanf (input, "%i", &value)) // This shouldn't happen.
            return "Could not read Project member index";
        std::cout << " >" << key_ << " >" << input << '\n';
        input = TextTokenEnd (input);      // Scroll past the number.
        int num_subprojects = projects_->count;
        if (value < num_subprojects)
            return projects_[value]->Command (input, stack);
        else if (value < num_subprojects + schedules_->count - 1)
            return schedules_[value]->Command (input);
        return "Invalid Project member index";
    } else if (c == '?') // Print detailed help
    {
        c = *(input + 1);
        if (!c) {
            std::cout << GetAppHelpString ();
            return nullptr;
        }
        if (!StringCompare (input + 2, "project")) {
            std::cout << Project::GetHelpString ();
            return nullptr;
        }
        if (!StringCompare (input + 2, "schedule")) {
            std::cout << Schedule::GetHelpString ();
            return nullptr;
        }
        if (!StringCompare (input + 2, "task")) {
            std::cout << Task::GetHelpString ();
            return nullptr;
        }
        return nullptr;
    }
    if (*input == 0)
        return Push (stack, this);
    // Search for key to pass the command char onto.
    end = TextTokenEnd (input);
    std::cout << " >" << key_ << " >" << input << '\n';
    if (*input == 0)
        return "Invalid key format.";

    value = ProjectIndex (input);
    if (value >= 0)
        return ArrayGet<Project*> (projects_, value)->Command (end, stack);
    value = ScheduleIndex (input);
    if (value >= 0)
        return ArrayGet<Schedule*> (schedules_, value)->Command (end);
    return "The Project does not contain the given key in this scope.";
}

}  //< namespace pro
}  //< namespace kabuki
