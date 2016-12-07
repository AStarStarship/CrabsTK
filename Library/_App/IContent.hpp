/*< Kabuki Toolkit
    @file       /.../Source/Kabuki_SDK-Impl/_App/IContent.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#include <string>

namespace _App
{
class IContent
/*< Interface for app content. */
{
    public:
    
    virtual int getAppCatagory () = 0;
    /*< Gets the catagory index of the content. */
    
    int getMinAgeGroup () = 0;
    /*< Returns a number 0 - 21 for the recommended age group. */
    
    const char* getBriefDescriptiong () = 0;
    
    string getDescriptiong () = 0;
    
    string getContentDescription () = 0;
    /*< Returns a string with the type of content. */
};
}
