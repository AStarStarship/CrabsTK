/** Chinese Room
    @file   ~/source/kabuki/chinese_room/include.h
    @author Cale McCollough
    @breif  File contains unit tests for Kabuki Pro.
*/

#include "../pro/include/module.h"

using namespace kabuki::_;
using namespace kabuki::pro;

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
