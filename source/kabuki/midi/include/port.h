/** Kabuki Starship
    @file    /.../Source/KabukiTheater/_Theater/MIDI/APort.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <KabukiTheater-Config.h>
#include "Message.h"

namespace _Theater { namespace MIDI {

class _KabukiTheater_ APort
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

    APort (int aQueueSize = DefaultQueueSize);
    //< Constructors a port with the given queue size.

    ~APort ();
    //< Destructor.

    void reset ();
    //< Dumps the queue and resets to init state.

    virtual void open () = 0;
    //< Opens this MIDI port.

    virtual void close () = 0;
    //< Closes this MIDI port.

    virtual void pause () = 0;
    //< Pauses this MIDI port.

    int getPortIndex ();
    //< Gets the index of this Port.

    void setPortIndex (int index);
    //< Sets the index of this Port to the new index.

    int getQueueSize ();
    //< returns the queue size.

    void resizeQueue (int newSize);
    //< resizes the queue to the newSize.

    void increaseQueueSize (int amount);
    //< Increases size of the message queue bY the queueSizeIncrease.

    int getNumMessages ();
    //< returns the number of messages in the queue.

    void queue (Message* newMessage);
    //< Adds a newMessage to the tail of the queue.

    Message* dequeue ();
    //< Dequeues the next message from the queue.

    Message* readMessage ();
    //< Dequeues the next message from the queue.

    void deleteQueue ();
    //< Dumps the queue.
    
    private:

    int portNum,                    //< This port index.
        msgQueueSize,               //< The size of the Message queue.
        numMsgs,                    //< The number of messages in the queue.
        readIndex,                  //< The head of the queue.
        queueIndex;                 //< The tail of the queue.
    vector<byte> queue;             //< The MIDI message queue.
    MIDIPortState currentState;     //< The currentState of this port.
};

}   //< MIDI
}   //< _Theater
