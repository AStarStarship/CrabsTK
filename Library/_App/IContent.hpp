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
/*<  */
{
    public:
    
    int Catagory () = 0;                    
    
    int MinAgeGroup () = 0;
    /*< Returns a number 0 - 21 for the recommended age group. */
    
    string Descriptiong () = 0;
    
    string FullDescriptiong () = 0;
    
    string ContentDescription () = 0;
    /*< Returns a string with the type of content. */
};
}
