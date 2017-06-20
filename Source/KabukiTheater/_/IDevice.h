/** The Chinese Room
    @version 0.x
    @file    /.../IDevice.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_IDEVICE_H
#define CHINESEROOM_IDEVICE_H

#include "Config.h"

#include "Terminal.h"
#include "Member.h"

namespace _ {

class IDevice
/** Interface for an object that can be manipulated by a Terminal.
    When an object is selected by a Terminal, the user can then call functions of 
    that object. Each object is limited to having .Select(Terminal*, IDevice*). The Terminal has a stack of objects that it then
    pushes the IDevice* on top of. This object is now selected, and agents can 
    now call functions of this object via the Room Control(DC).

    @code
    class DeviceExample : public IDevice
    //< Remote procedure call using IDevice interface example.
    {
        public:

        void functionA() {}                //< Example dummy function A.
        void functionB() {}                //< Example dummy function B.

        const Member<T>* op(Terminal* io, byte index) override
        //< Remote procedure call using switch statement example; note the only RAM used is one pointer per parameter.
        {
            Params<2> param;    //< Contains an array of 2 void* for the max num parameters.

            switch(index)
            {
                case '?': { 
                    static const Member m0 = { "ChineseRoomExample", setNumMembers(2), nullptr, 
                    "Description of ChineseRoomExample." };
                    // 63 is ASCII '?'
                    return &m0;
                }
                case 64: {
                        static const Member m1 = { "functionA",    //< The key of the member.
                        Rx<2, FLT, STR, StringBufferSize>::Header,  //< The rx header with buffer size for string.
                        Tx<2, FLT, STR>::Header,                    //< Note: There is no buffer size for tx values!
                        "Description of functionA." };

                    // 66 is ASCII 'A'
                    if(!io) return &m1;

                    if(io->read(m1.rxHeader, param.args(&ioNumber, ioString))) return readError();

                    functionA();

                    if(io->write(m1.txHeader, param.args(&ioNumber, ioString))) return writeError();
                    return &m1;
                }
                case 65: {
                    static const Member m2 = { "functionB",         //< Duplicate function for example purposes.
                        Rx<2, FLT, STR, StringBufferSize>::Header,  //< Note the C++11 variadic template ensures that 
                        Tx<2, FLT, STR>::Header,                    //< only one copy of the header is in ROM! :-)
                        "Description of functionB." };
                
                    if(!io) return &m2;

                    if(io->read(m2.rxHeader, param.args(&ioNumber, ioString))) return readError();

                    functionB();

                    if(io->write(m2.txHeader, param.args(&ioNumber, ioString))) return writeError();
                    return &m2;
                }
            }
            return nullptr;
        }

        void query(Terminal& io) override
        //< Called when debugging or connecting object does not know the interface for this device.
        {
            io << "Hello world and the number " << 3 << "!\n\n\n";
        }

        private:

        NONCOPYABLE(DeviceExample)

        enum {
            StringBufferSize = 16       //< Example string buffer size.
        };

        float ioNumber;                 //< Example variable.
        char ioString[StringBufferSize];//< Example string.
    };
    @endcode
*/
{
    public:

    virtual const Member<T>* op(Terminal* io, byte index) = 0;
    /*< Inter-process operations. */
};

inline uint_t* setNumMembers (std::uintptr_t value)
/*< converts the value to a pointer. */
{
    return reinterpret_cast<uint_t*>(value);
}

template<typename T>
inline uintptr_t getNumMembers (IDevice* d)
/*< Gets the number of members of the given device. */
{
    if (d == nullptr) return 0;
    const Member<T>* m = d->op (nullptr, '?');
    return m == nullptr ? 0 : reinterpret_cast<uintptr_t>(m->rxHeader);
}

template<typename T, const char* key, byte numMembers, const uint_t* rxHeader, 
    const uint_t* txHeader>
const Member<T>* member ()
{
    static const Member<T> member = { key, rxHeader, txHeader, 0 };
    return &member;
}

template<typename T, const char* key, byte numMembers, const uint_t* rxHeader, 
    const uint_t* txHeader, const char* description>
const Member<T>* member ()
{
    static const Member<T> member = { key, rxHeader, txHeader, description };
    return &member;
}

template<typename T, const char* key, byte numMembers, const char* description>
const Member<T>* memberObject ()
{
    static const Member<T> member = { key, setNumMembers (numMembers), 0, 
        description };
    return &member;
}

template<typename T, const char* key, byte numMembers, const uint_t* txHeader, 
    const char* description>
const Member<T>* memberObject ()
{
    static const Member<T> member = { key, setNumMembers (numMembers), txHeader,
        description };
    return &member;
}

}   //< namespace    

#endif  //< CHINESEROOM_IDEVICE_H
