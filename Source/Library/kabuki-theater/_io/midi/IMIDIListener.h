
#pragma once

namespace _Theater { namespace MIDI {

class IMIDIListener
{
    public:
    
    virtual void midiIn (MidiInput* source, const Message& message) = 0;
    /*< Handels MIDI input from the given source. */
};

}   //< MIDI
}   //< _Theater
