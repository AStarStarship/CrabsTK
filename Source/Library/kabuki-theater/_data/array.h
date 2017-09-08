

#include <_/array.h>

namespace _data {
    
/** C++ wrapper class for the Chinese Room c-style Array.
    @code
    @endcode
*/
template<typename T>
class Array {
    public:
    
    Array () {
    }
    
    private:
    
    _::Array<T> array_;     //< The c-style underlaying array.
}
    
}       //< namespace _data