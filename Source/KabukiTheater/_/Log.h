/** The Chinese Room
    @version 0.x
    @file    /.../Log.h
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

#ifndef CHINESEROOM_LOG_H
#define CHINESEROOM_LOG_H

#include "IDevice.h"
#include "Terminal.h"

namespace _ {

template<typename T>
class Log: public IDevice
/*< A type of IDevice that you can write test too.
    A Log is a just a Terminal that has a function that prints strings.
*/
{
	public:

	Log ()
    /*< Defaults constructor. */
    {

    }

	Terminal* getTerminal ()
    /*< Gets the Terminal. */
    {
        return terminal;
    }

	bool setTerminal (Terimnal<>* t)
    /*< Sets the terminal for the log to the given pointer. */
    {
        return t == nullptr ? false : terminal = t;
    }
    
    const Member<T>* op (Terminal* io, byte index) override
    /*< I2P procedures. */
    {
        return nullptr;
    }

	private:

	Terminal* terminal;
};

template<typename T>
inline Log<T>& operator<<(Log<T>& io, int8_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[5];
    sprintf_s (buffer, 5, "%i", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, uint8_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[4];
    sprintf_s (buffer, 4, "%u", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, int16_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[7];
    sprintf_s (buffer, 7, "%i", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, uint16_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[6];
    sprintf_s (buffer, 6, "%u", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, int32_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[12];
    sprintf_s (buffer, 12, "%i", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, uint32_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[11];
    sprintf_s (buffer, 11, "%u", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, int64_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[22];
    sprintf_s (buffer, 22, "%I64d", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, uint64_t value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[21];
    sprintf_s (buffer, 21, "%llu", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, float value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[FLT_MAX_10_EXP + 2];
    sprintf_s (buffer, FLT_MAX_10_EXP + 2, "%f", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, double value)
/*< Operator << prints the given value as a string to the log. */
{
    char buffer[DBL_MAX_10_EXP + 2];
    sprintf_s (buffer, DBL_MAX_10_EXP + 2, "%f", value);
    io.getTerminal ()->log (buffer);
    return io;
}

template<typename T>
inline Log<T>& operator<<(Log<T>& io, const char* s)
/*< Operator << prints a string to the log. */
{
    io.getTerminal ()->log (s);
    return io;
}

}	//< namespace _

#endif  //< CHINESEROOM_LOG_H
