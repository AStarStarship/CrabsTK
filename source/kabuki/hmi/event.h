/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/event.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
 
#ifndef HEADER_FOR_KABUKI_HMI_EVENT2
#define HEADER_FOR_KABUKI_HMI_EVENT2

#include "config.h"

namespace kabuki { namespace hmi {

/** A event with a timestamp.
    An example of an Event is typically like a Control or menu function.
 */
class Event {
    public:

    /** Default constructor. */
    Event ();
    
    /** Virtual destructor. */
    virtual ~Event ();

    /** gets the timestamp of the Event. */
    timestamp_t GetTimestamp ();

    /** Triggers the event. */
    virtual void Trigger () = 0;
    
    /** Prints this object to a stdout. */
    virtual void Print () const = 0;

    private:

    timestamp_t timestamp;    //< Event timestamp in microseconds.
};      //< class Event
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_EVENT2


////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef HEADER_FOR_KABUKI_HMI_EVENT
#define HEADER_FOR_KABUKI_HMI_EVENT

#include "config.h"
#include "joystick.h"
#include "keyboard.h"
#include "mouse.h"
#include "sensor.h"

namespace kabuki { namespace hmi {

class Event
/** Defines a system event and its parameters. 
    @ingroup hmi
   
    hmi::Event holds all the informations about a system event
    that just happened. Events are retrieved using the
    hmi::Window::pollEvent and hmi::Window::waitEvent functions.
   
    A hmi::Event instance contains the type of the event
    (mouse moved, key pressed, window closed, ...) as well
    as the details about this particular event. Please note that
    the event parameters are defined in a union, which means that
    only the member matching the type of the event will be properly
    filled; all other members will have undefined values and must not
    be read if the type of the event doesn't match. For example,
    if you received a KeyPressed event, then you must read the
    event.key member, all other members such as event.MouseMove
    or event.text will have undefined values.
   
    Usage example:
    @code
    hmi::Event event;
    while (window.pollEvent(event))
    {
        // Request for closing the window
        if (event.type == hmi::Event::Closed)
            window.close();
   
        // The escape key was pressed
        if ((event.type == hmi::Event::KeyPressed) && (event.key.code == hmi::Keyboard::Escape))
            window.close();
   
        // The window was resized
        if (event.type == hmi::Event::Resized)
            doSomethingWithTheNewSize(event.size.width, event.size.height);
   
        // etc ...
    }
    @endcode */
{
    public:
    
    struct SizeEvent
    /** Size events parameters (Resized). */
    {
        uint width;  //< New width, in pixels
        uint height; //< New height, in pixels
    };

    /** Keyboard event parameters (KeyPressed, KeyReleased). */
    struct KeyEvent
    {
        Keyboard::Key code;    //< Code of the key that has been pressed
        bool alt;     //< Is the Alt key pressed?
        bool control; //< Is the Control key pressed?
        bool shift;   //< Is the Shift key pressed?
        bool system;  //< Is the System key pressed?
    };

    /** Text event parameters (TextEntered). */
    struct TextEvent
    {
        uint32_t unicode; //< UTF-32 Unicode value of the character
    };
    
    /** Mouse move event parameters (MouseMoved). */
    struct MouseMoveEvent
    {
        int x; //< X position of the mouse pointer, relative to the left of the owner window
        int y; //< Y position of the mouse pointer, relative to the top of the owner window
    };
    
    /** Mouse buttons events parameters
        (MouseButtonPressed, MouseButtonReleased). */
    struct MouseButtonEvent
    {
        Mouse::Button button; //< Code of the button that has been pressed
        int x;      //< X position of the mouse pointer, relative to the left of the owner window
        int y;      //< Y position of the mouse pointer, relative to the top of the owner window
    };
    
    /** Mouse wheel events parameters (MouseWheelScrolled). */
    struct MouseWheelScrollEvent
    {
        Mouse::Wheel wheel; //< Which wheel (for mice with multiple ones)
        float delta; //< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
        int x;     //< X position of the mouse pointer, relative to the left of the owner window
        int y;     //< Y position of the mouse pointer, relative to the top of the owner window
    };

    /** Joystick connection events parameters
        (JoystickConnected, JoystickDisconnected). */
    struct JoystickConnectEvent
    {
        uint joystickId; //< Index of the joystick (in range [0 .. Joystick::Count - 1])
    };
    
    /** Joystick axis move event parameters (JoystickMoved). */
    struct JoystickMoveEvent
    {
        uint joystickId;      //< Index of the joystick (in range [0 .. Joystick::Count - 1])
        Joystick::Axis axis;    //< Axis on which the joystick moved
        float position;         //< New position on the axis (in range [-100 .. 100])
    };

    /** Joystick buttons events parameters
        (JoystickButtonPressed, JoystickButtonReleased). */
    struct JoystickButtonEvent
    {
        uint joystickId; //< Index of the joystick (in range [0 .. Joystick::Count - 1])
        uint button;     //< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
    };
    
    /** Touch events parameters (TouchBegan, TouchMoved, TouchEnded). */
    struct TouchEvent
    {
        uint finger;        //< Index of the finger in case of multi-touch events
        int x;              //< X position of the touch, relative to the left of the owner window
        int y;              //< Y position of the touch, relative to the top of the owner window
    };

    /** Sensor event parameters (SensorChanged). */
    struct SensorEvent
    {
        Sensor::Type type; //< Type of the sensor
        float x;           //< Current value of the sensor on X axis
        float y;           //< Current value of the sensor on Y axis
        float z;           //< Current value of the sensor on Z axis
    };

    /** Enumeration of the different types of events. */
    enum EventType
    {
        Closed,                 //< The window requested to be closed (no data)
        Resized,                //< The window was resized (data in event.size)
        LostFocus,              //< The window lost the focus (no data)
        GainedFocus,            //< The window gained the focus (no data)
        TextEntered,            //< A character was entered (data in event.text)
        KeyPressed,             //< A key was pressed (data in event.key)
        KeyReleased,            //< A key was released (data in event.key)
        MouseWheelMoved,        //< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
        MouseWheelScrolled,     //< The mouse wheel was scrolled (data in event.mouseWheelScroll)
        MouseButtonPressed,     //< A mouse button was pressed (data in event.mouseButton)
        MouseButtonReleased,    //< A mouse button was released (data in event.mouseButton)
        MouseMoved,             //< The mouse cursor moved (data in event.mouseMove)
        MouseEntered,           //< The mouse cursor entered the area of the window (no data)
        MouseLeft,              //< The mouse cursor left the area of the window (no data)
        JoystickButtonPressed,  //< A joystick button was pressed (data in event.joystickButton)
        JoystickButtonReleased, //< A joystick button was released (data in event.joystickButton)
        JoystickMoved,          //< The joystick moved along an axis (data in event.joystickMove)
        JoystickConnected,      //< A joystick was connected (data in event.joystickConnect)
        JoystickDisconnected,   //< A joystick was disconnected (data in event.joystickConnect)
        TouchBegan,             //< A touch event began (data in event.touch)
        TouchMoved,             //< A touch moved (data in event.touch)
        TouchEnded,             //< A touch event ended (data in event.touch)
        SensorChanged,          //< A sensor value changed (data in event.sensor)

        Count                   //< Keep last -- the total number of event types
    };

    EventType type; //< Type of the event
    
    union {
        SizeEvent size;                         //< Size event parameters (Event::Resized)
        KeyEvent key;                           //< Key event parameters (Event::KeyPressed, Event::KeyReleased)
        TextEvent text;                         //< Text event parameters (Event::TextEntered)
        MouseMoveEvent mouseMove;               //< Mouse move event parameters (Event::MouseMoved)
        MouseButtonEvent mouseButton;           //< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
        MouseWheelEvent mouseWheel;             //< Mouse wheel event parameters (Event::MouseWheelMoved) (deprecated)
        MouseWheelScrollEvent mouseWheelScroll; //< Mouse wheel event parameters (Event::MouseWheelScrolled)
        JoystickMoveEvent joystickMove;         //< Joystick move event parameters (Event::JoystickMoved)
        JoystickButtonEvent joystickButton;     //< Joystick button event parameters (Event::JoystickButtonPressed, Event::JoystickButtonReleased)
        JoystickConnectEvent joystickConnect;   //< Joystick (dis)connect event parameters (Event::JoystickConnected, Event::JoystickDisconnected)
        TouchEvent touch;                       //< Touch events parameters (Event::TouchBegan, Event::TouchMoved, Event::TouchEnded)
        SensorEvent sensor;                     //< Sensor event parameters (Event::SensorChanged)
    };
};

}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_EVENT
