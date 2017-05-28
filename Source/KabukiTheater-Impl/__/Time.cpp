////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright(C) 2007-2016 Laurent Gomila(laurent@sfml-dev.org)
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

#include <_System/Time.hpp>
#include <chrono>

namespace _System {

const Time Time::Zero;

Time::Time()
:   m_microseconds(0)
{
}

float Time::asSeconds() const
{
    return m_microseconds / 1000000.f;
}

int32_t Time::asMilliseconds() const
{
    return static_cast<int32_t>(m_microseconds / 1000);
}

timestamp_t Time::getMicroseconds() const
{
    return m_microseconds;
}

Time::Time(timestamp_t microseconds)
    :m_microseconds (microseconds)
{
}

Time seconds(float amount)
{
    return Time(static_cast<timestamp_t>(amount * 1000000));
}

Time milliseconds(int32_t amount)
{
    return Time(static_cast<timestamp_t>(amount) * 1000);
}

Time microseconds(timestamp_t amount)
{
    return Time(amount);
}

bool operator ==(Time left, Time right)
{
    return left.getMicroseconds() == right.getMicroseconds();
}

bool operator !=(Time left, Time right)
{
    return left.getMicroseconds() != right.getMicroseconds();
}

bool operator <(Time left, Time right)
{
    return left.getMicroseconds() < right.getMicroseconds();
}

bool operator >(Time left, Time right)
{
    return left.getMicroseconds() > right.getMicroseconds();
}

bool operator <=(Time left, Time right)
{
    return left.getMicroseconds() <= right.getMicroseconds();
}

bool operator >=(Time left, Time right)
{
    return left.getMicroseconds() >= right.getMicroseconds();
}

Time operator -(Time right)
{
    return microseconds(-right.getMicroseconds());
}

Time operator +(Time left, Time right)
{
    return microseconds(left.getMicroseconds() + right.getMicroseconds());
}

Time& operator +=(Time& left, Time right)
{
    return left = left + right;
}

Time operator -(Time left, Time right)
{
    return microseconds(left.getMicroseconds() - right.getMicroseconds());
}

Time& operator -=(Time& left, Time right)
{
    return left = left - right;
}

Time operator *(Time left, float right)
{
    return seconds(left.asSeconds() * right);
}

Time operator *(Time left, timestamp_t right)
{
    return microseconds(left.getMicroseconds() * right);
}

Time operator *(float left, Time right)
{
    return right * left;
}

Time operator *(timestamp_t left, Time right)
{
    return right * left;
}

Time& operator *=(Time& left, float right)
{
    return left = left * right;
}

Time& operator *=(Time& left, timestamp_t right)
{
    return left = left * right;
}

Time operator /(Time left, float right)
{
    return seconds(left.asSeconds() / right);
}

Time operator /(Time left, timestamp_t right)
{
    return microseconds(left.getMicroseconds() / right);
}

Time& operator /=(Time& left, float right)
{
    return left = left / right;
}

Time& operator /=(Time& left, timestamp_t right)
{
    return left = left / right;
}

float operator /(Time left, Time right)
{
    return left.asSeconds() / right.asSeconds();
}

Time operator %(Time left, Time right)
{
    return microseconds(left.getMicroseconds() % right.getMicroseconds());
}

Time& operator %=(Time& left, Time right)
{
    return left = left % right;
}

time_t getTime_s(timestamp_t Timestamp)
{
    return(time_t)Timestamp;
}

int32_t getTime_us(timestamp_t Timestamp)
{
    return(int32_t)((Timestamp & 0xFFFFFFFF00000000) >> 32);
}

timestamp_t getTime(time_t UnixTime, int32_t Microseconds)
{
    return(((timestamp_t)UnixTime) << 8) &(timestamp_t)Microseconds;
}

timestamp_t getTimestamp()
{
    std::chrono::microseconds us (1);
    
    return us.count;
}

}   //< _System
