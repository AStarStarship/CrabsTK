////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright(C) 2007-2017 Laurent Gomila(laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include "Config.h"

#include <time.h>

namespace _ {
    
class inline Time
/*< Represents a time value.
    @ingroup _

    _::Time encapsulates a time value in a flexible way.
    It allows to define a time value either as a number of
    seconds, milliseconds or microseconds. It also works the
    other way round: you can read a time value as either
    a number of seconds, milliseconds or microseconds.

    By using such a flexible interface, the API doesn't
    impose any fixed type or resolution for time values,
    and let the user choose its own favorite representation.

    Time values support the usual mathematical operations:
    you can add or subtract two times, multiply or divide
    a time by a number, compare two times, etc.

    Since they represent a time span and not an absolute time
    value, times can also be negative.

    Usage example:
    @code
    _::Time t1 = _::seconds(0.1f);
    int32_t milli = t1.asMilliseconds(); // 100

    _::Time t2 = _::milliseconds(30);
    timestamp_t micro = t2.getMicroseconds(); // 30000

    _::Time t3 = _::microseconds(-800000);
    float sec = t3.asSeconds(); // -0.8
    @endcode

    @code
    void update(_::Time elapsed)
    {
        position += speed * elapsed.asSeconds();
    }

    update(_::milliseconds(100));
    @endcode

    @see _::Clock
*/
{
    public:

    Time();
    /*< Default constructor.
    
        Sets the time value to zero. */

    float asSeconds() const;
    /*< Return the time value as a number of seconds.

        @return Time in seconds
        @see asMilliseconds, getMicroseconds. */

    int32_t asMilliseconds() const;
    /*< Return the time value as a number of milliseconds.

        @return Time in milliseconds
        @see asSeconds, getMicroseconds. */

    timestamp_t getMicroseconds() const;
    /*< Return the time value as a number of microseconds.

        @return Time in microseconds
        @see asSeconds, asMilliseconds. */

    static const Time Zero; //< Predefined "zero" time value

    private:

    friend inline Time seconds(float);
    friend inline Time milliseconds(int32_t);
    friend inline Time microseconds(timestamp_t);

    explicit Time(timestamp_t microseconds);
    /*< Construct from a number of microseconds.
    
        This function is internal. To construct time values,
        use _::seconds, _::milliseconds or _::microseconds instead.

        @param microseconds Number of microseconds. */

    private:

    timestamp_t m_microseconds; //< Time value stored as microseconds
};

inline Time seconds(float amount);
/*< Construct a time value from a number of seconds
    @relates Time

    @param amount Number of seconds

    @return Time value constructed from the amount of seconds

    @see milliseconds, microseconds. */

inline Time milliseconds(int32_t amount);
/*< Construct a time value from a number of milliseconds
    @relates Time

    @param amount Number of milliseconds

    @return Time value constructed from the amount of milliseconds

    @see seconds, microseconds. */

inline Time microseconds(timestamp_t amount);
/*< Construct a time value from a number of microseconds
    @relates Time

    @param amount Number of microseconds

    @return Time value constructed from the amount of microseconds

    @see seconds, milliseconds. */

inline bool operator ==(Time left, Time right);
/*< Overload of == operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if both time values are equal. */

inline bool operator !=(Time left, Time right);
/*< Overload of != operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if both time values are different. */

inline bool operator <(Time left, Time right);
/*< Overload of < operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if \a left is lesser than \a right. */

inline bool operator >(Time left, Time right);
/*< Overload of > operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if \a left is greater than \a right. */

inline bool operator <=(Time left, Time right);
/*< Overload of <= operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if \a left is lesser or equal than \a right. */

inline bool operator >=(Time left, Time right);
/*< Overload of >= operator to compare two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return True if \a left is greater or equal than \a right. */

inline Time operator -(Time right);
/*< Overload of unary - operator to negate a time value
    @relates Time

    @param right Right operand(a time)

    @return Opposite of the time value. */

inline Time operator +(Time left, Time right);
/*< Overload of binary + operator to add two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return Sum of the two times values. */

inline Time& operator +=(Time& left, Time right);
/*< Overload of binary += operator to add/assign two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return Sum of the two times values. */

inline Time operator -(Time left, Time right);
/*< Overload of binary - operator to subtract two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return Difference of the two times values. */

inline Time& operator -=(Time& left, Time right);
/*< Overload of binary -= operator to subtract/assign two time values
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return Difference of the two times values. */

inline Time operator *(Time left, float right);
/*< Overload of binary * operator to scale a time value
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left multiplied by \a right. */

inline Time operator *(Time left, timestamp_t right);
/*< Overload of binary * operator to scale a time value
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left multiplied by \a right. */

inline Time operator *(float left, Time right);
/*< Overload of binary * operator to scale a time value
    @relates Time

    @param left  Left operand(a number)
    @param right Right operand(a time)

    @return \a left multiplied by \a right. */

inline Time operator *(timestamp_t left, Time right);
/*< Overload of binary * operator to scale a time value
    @relates Time

    @param left  Left operand(a number)
    @param right Right operand(a time)

    @return \a left multiplied by \a right. */

inline Time& operator *=(Time& left, float right);
/*< Overload of binary *= operator to scale/assign a time value
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left multiplied by \a right. */

inline Time& operator *=(Time& left, timestamp_t right);
/*< Overload of binary *= operator to scale/assign a time value.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left multiplied by \a right. */

inline Time operator /(Time left, float right);
/*< Overload of binary / operator to scale a time value.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left divided by \a right. */

inline Time operator /(Time left, timestamp_t right);
/*< Overload of binary / operator to scale a time value.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left divided by \a right. */

inline Time& operator /=(Time& left, float right);
/*< Overload of binary /= operator to scale/assign a time value
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left divided by \a right. */

inline Time& operator /=(Time& left, timestamp_t right);
/*< Overload of binary /= operator to scale/assign a time value.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a number)

    @return \a left divided by \a right. */

inline float operator /(Time left, Time right);
/*< Overload of binary / operator to compute the ratio of two time values.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return \a left divided by \a right. */

inline Time operator %(Time left, Time right);
/*< Overload of binary % operator to compute remainder of a time value.
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return \a left modulo \a right. */

inline Time& operator %=(Time& left, Time right);
/*< Overload of binary %= operator to compute/assign remainder of a time value
    @relates Time

    @param left  Left operand(a time)
    @param right Right operand(a time)

    @return \a left modulo \a right. */

inline timestamp_t getTime(time_t unixTime, int32_t microseconds);
/*< Generates a timestamp from a unix timestamp and 32-bit mbed microsecond timestamp. */
{
    return(((timestamp_t)UnixTime) << 8) &(timestamp_t)Microseconds;
}

inline time_t getTime_s(timestamp_t t);
/*< Gets the seconds since January 1, 1970. */
{
    return(time_t)t;
}

inline int32_t getTime_us(timestamp_t timestamp);
/*< Gets the microsecond time. */
{
    return(int32_t)((Timestamp & 0xFFFFFFFF00000000) >> 32);
}

inline timestamp_t getTimestamp();
/*< Creates a timestamp of the current time. */
{
    std::chrono::microseconds us (1);

    return us.count;
}

}   //< _
