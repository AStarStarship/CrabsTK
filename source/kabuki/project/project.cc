/** CS 162 Program 4-5
    @file   tests.h
    @author Cale McCollough
    @breif  File contains unit tests for Kabuki Pro.
*/

#include "project.h"
#include "utils.h"
#include <iostream>

using namespace _pro;
using namespace std;

class ChineseRoom : public Project
{
    public:

    ChineseRoom () :
        Project ("Chinese-Room")
    {
        PrintBreak ("<\n< Creating-Chinese Project...\n", '-');

        AddSchedule ("Uniprinter_Class");
        Schedule* s = GetSchedule ("Uniprinter_Class");
        if (!s) {
            cout << "<\n< Error: finding Uniprinter_Class schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Finish unit tests for scanner. Test "
                          "memory alignment."));
        
        AddSchedule ("Book_Class");
        s = GetSchedule ("Book_Class");
        if (!s) {
            cout << "<\n< Error: finding Book_Class schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Finish Dictionary data functions. Use same "
                          "memory alignment algorithm as the scanner."));
        Print ();
        PrintBreak ("<\n< Done creating Chinese-Room Project...\n<", '-');
    }
};

class KabukiToolkit: public Project
{
    public:
    
    KabukiToolkit () :
        Project ("kabuki")
    {
        PrintBreak ("<\n< Creating kabuki Project...\n<", '-');

        AddSchedule ("Examples");
        Schedule* s = GetSchedule ("Examples");
        if (!s) {
            cout << "<\n< Error: finding Examples schedule.\n<\n";
            return;
        }
        s->Add (new Task ("Make Serial example for mbed, MinGW, OS X, and "
                          "Linux."));
        
        Print ();
        PrintBreak ("<\n< Done creatingK kabuki Project...\n<", '-');
    }
};

template<uint year, uint month, uint day, uint  hour = 0, uint minute = 0,
    uint second = 0>
    time_t TestTime (char* buffer, int buffer_size) {
    if (buffer == nullptr)
        return 0;
    time_t t;
    time (&t);
    tm* moment = localtime (&t);
    if (!moment) {
        cout << "|\n| Created invalid test moment: " << moment << '\n';
        return 0;
    }
    moment->tm_year = year - TIME_EPOCH;
    moment->tm_mon = month - 1;
    moment->tm_mday = day;
    moment->tm_hour = hour;
    moment->tm_min = minute;
    moment->tm_sec = second;

    if (!PrintDateTimeString (buffer, buffer_size, moment)) {
        cout << "< Error making timestamp \n";

        return 0;
    }
    cout << "< Creating test time: ";
    PrintDateTime (moment);
    t = mktime (moment);
    if (t < 0) {
        cout << "< Invalid " << t << '\n';
        return 0;
    } else {
        cout << '\n';
    }
    return t;
}

/** Date-Time parser tests. */
static void TestDateParser () {
    time_t t,
        t_found;
    tm* lt;
    const char* result;

    PrintBreak ("<", '-');
    cout << "< Testing date-time parser... \n";

    const char* strings[] = { "8/9\0",
        "08/09\0",
        "8/9/17\0",
        "8/09/17\0",
        "8/9/2017\0",
        "8/09/2017\0",
        "8/09/2017\0",
        "08/9/2017\0",
        "8/09/2017@00\0",
        "8.09.2017@00AM\0",
        "8/09/2017@00:00\0",
        "8/09/17@00:0AM\0",
        "8/09/2017@00:00:00\0",
        "8/09/2017@00:00:00AM\0",
        "2017-08-09@00:00:00AM\0",
        "2017-08-09@00:00:00am\0",
        "2017-08-09@00:00:00A\0",
        "2017-08-09@00:00:00a \0",
    };
    for (int i = 0; i < 18; ++i) {
        PrintBreak ("<", '-');
        cout << "| " << i;
        time_t t = 0;
        result = ParseTimeString (strings[i], t);
        CompareTimes (t, 2017, 8, 9, 0, 0, 0);
    }
    enum {
        SIZE = 32
    };

    char timestamp[SIZE];
    PrintBreak ("<", '-');
    PrintBreak ("<\n< Testing more valid input...\n");
    PrintBreak ("<", '-');

    t = TestTime<8, 9, 17, 4, 20> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<2020, 4, 20, 4, 20> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    t = TestTime<1947, 12, 7, 23, 5, 7> (timestamp, SIZE);
    PrintTime (t);
    result = ParseTimeString (timestamp, t_found);
    CompareTimes (t, t_found);

    //system ("PAUSE");

    PrintBreak ("<", '-');
    PrintBreak ("<\n< Testing invalid input...\n<", '-');
    ParseTimeString ("cat", t);
    PrintBreak ("<", '-');
    ParseTimeString ("2017-30-40", t);
    PrintBreak ("<", '-');

    cout << "<\n< Done testing date parsing utils! :-)\n";
    //system ("PAUSE");
}

/** Unit tests for the _pro::Project class. */
static void CreateTestProject (Project& project)
{

}


/** Runs all the unit tests. */
static void RunTests (Project& root, ProjectNode* stack)
{
    char* test1 = "test -s \"Task 1\" -d \"Details\" "
                  "-w 25.0 -a 1.0 -g 0.5 ";
                  "-r \"Wrong! All wrong!\" -R \"I got it all right!\" ";
    char* test2 = "test -s \"Task 1\" -d \"Details\" "
                  "-w 25.0 -a 1.0 -g 0.5 ";
                  "-r \"Wrong! All wrong!\" -R \"I got it all right!\" "
                  "-B @3PM -E @4PM -S @3:30PM -P @6:58PM ";
    const char* result;     //< Pointer to the result of a command.


    PrintBreak ("<", '-', 10);
    cout << "<\n< Testing Kabuki Pro...\n<\n";

    //TestDateParser ();

    //root.AddProject (new ChineseRoom ());
    //root.AddProject (new KabukiToolkit ());

    cout << "<\n< Testing Task.Command (char*):const char*\n<\n";
    root.AddProject (new Project ("test"));

    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command (test1, stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command (test2, stack)) == NULL ? "" : result);
    cout << '\n';
    cout << ((result = root.Command ("test -p", stack)) == NULL ? "" : result);

    cout << "<\n<\n< Done testing Kabuki Pro...\n";
    PrintBreak ("<", '-');
    system ("PAUSE");
}

