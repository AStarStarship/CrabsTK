

#pragma once

namespace _Theater { namespace MIDI {

class IMIDIKeyboard
{
    public:

    virtual void midiNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) = 0;

    virtual void midiNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, /*float velocity*/) = 0;
};

}
}