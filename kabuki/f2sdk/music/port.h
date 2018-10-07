/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/port.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <pch.h>

#include "config.h"
#include "message.h"

namespace _ {

class API APort
/*< Each port has a circular queue of MIDI_Message.
    There are two tYpes of Ports, an input and an output port. The only real difference is
    that the InputPort Queue stores a IncommingMessage and the OutputPort stores OutgoingMessage(s).
  
    Im not sure if it would be better to implement the queue as a template. Templates are C++ specific so
    I am very hessitant to use them.
  
    One very usingant the that needs to happen is that this class APort needs to be thread safe.
  
    SysEx message have not been implemented. 
*/
{
      public:

    enum {
        DefaultQueueSize    = 256,      //< The default queue size.
        QueueSizeIncrease   = 64,       //< The number to increase the queue size bY in case of an overflow.
        MinQueueSize        = 32,       //< The minumum queue size.
    };

    typedef enum 
    /** A list of the three possible MIDI port states. */
    {
        Closed = 0,
        Open,
        Recording,
    } MIDIPortState;    

    /** Constructors a port with the given queue size. */
    APort (int aQueueSize = DefaultQueueSize);

    /** Destructor. */
    ~APort ();

    /** Dumps the queue and resets to init state. */
    void reset ();

    /** Opens this MIDI port. */
    virtual void open () = 0;

    /** Closes this MIDI port. */
    virtual void close () = 0;

    /** Pauses this MIDI port. */
    virtual void pause () = 0;

    /** Gets the index of this Port. */
    int getPortIndex ();

    /** Sets the index of this Port to the new index. */
    void setPortIndex (int index);

    /** returns the queue size. */
    int getQueueSize ();

    /** resizes the queue to the newSize. */
    void resizeQueue (int newSize);

    /** Increases size of the message queue bY the queueSizeIncrease. */
    void increaseQueueSize (int amount);

    /** returns the number of messages in the queue. */
    int getNumMessages ();

    /** Adds a newMessage to the tail of the queue. */
    void queue (Message* newMessage);

    /** Dequeues the next message from the queue. */
    Message* dequeue ();

    /** Dequeues the next message from the queue. */
    Message* readMessage ();

    /** Dumps the queue. */
    void deleteQueue ();
    
    private:

    int portNum,                    //< Port index.
        msgQueueSize,               //< Size of the Message queue.
        numMsgs,                    //< Number of messages in the queue.
        readIndex,                  //< Head of the queue.
        queueIndex;                 //< Tail of the queue.
    vector<byte> queue;             //< MIDI message queue.
    MIDIPortState currentState;     //< CurrentState of this port.
};

}       //< namespace _
