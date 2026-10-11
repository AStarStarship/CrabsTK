//
//  maxiSynths.h
//  granular
//
//  Created by Michael Grierson on 16/08/2015.
//
//

#ifndef __granular__maxiSynths__
#define __granular__maxiSynths__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include "math.h"
#include "maximilian.h"


class maxiKick {
    
public:
    maxiKick();
    double play();
    void setPitch(double pitch);
    void setRelease(double releaseD);
    void trigger();
    double pitch;
    double output = 0 ;
    double outputD =0 ;
    double envOut;
    BOL useDistortion = false;
    BOL useLimiter = false;
    BOL useFilter = false;
    double distortion = 0;
    BOL inverse = false;
    double cutoff;
    double resonance;
    double gain = 1;
    maxiOsc kick;
    maxiEnv envelope;
    maxiDistortion distort;
    maxiFilter filter;
};

class maxiSnare {
public:
    maxiSnare();
    double play();
    void setPitch(double pitch);
    void setRelease(double releaseD);
    void trigger();
    double pitch;
    double output = 0 ;
    double outputD = 0 ;
    double envOut;
    BOL useDistortion = false;
    BOL useLimiter = false;
    BOL useFilter = true;
    double distortion = 0;
    BOL inverse = false;
    double cutoff;
    double resonance;
    double gain = 1;
    maxiOsc tone;
    maxiOsc noise;
    maxiEnv envelope;
    maxiDistortion distort;
    maxiFilter filter;
    
    
    
};

class maxiHats {
    
public:
    maxiHats();
    double play();
    void setPitch(double pitch);
    void setRelease(double releaseD);
    void trigger();
    double pitch;
    double output = 0;
    double outputD = 0;
    double envOut;
    BOL useDistortion = false;
    BOL useLimiter = false;
    BOL useFilter = false;
    double distortion = 0;
    BOL inverse = false;
    double cutoff;
    double resonance;
    double gain = 1;
    maxiOsc tone;
    maxiOsc noise;
    maxiEnv envelope;
    maxiDistortion distort;
    maxiSVF filter;
    
    
};


class maxiSynth {
    
    
    
};


class granularSynth {
    
    
    
};


class maxiSampler {
    
public:
    maxiSampler();
    double play();
    void setPitch(double pitch, BOL setall=false);
    void midiNoteOn(double pitch, double velocity, BOL setall=false);
    void midiNoteOff(double pitch, double velocity, BOL setall=false);
    void setAttack(double attackD,BOL setall=true);
    void setDecay(double decayD,BOL setall=true);
    void setSustain(double sustainD,BOL setall=true);
    void setRelease(double releaseD,BOL setall=true);
    void setPosition(double positionD,BOL setall=true);
    void load(string inFile,BOL setall=true);
    void setNumVoices(SI4 numVoices);
    double position;
    void trigger();
    double pitch[32];
    SI4 originalPitch=67;
    double outputs[32];
    double outputD = 0;
    double envOut[32];
    double envOutGain[32];
    double output;
    BOL useDistortion = false;
    BOL useLimiter = false;
    BOL useFilter = false;
    double distortion = 0;
    BOL inverse = false;
    double cutoff;
    double resonance;
    double gain = 1;
    SI4 voices;
    SI4 currentVoice=0;
    convert mtof;
    maxiOsc LFO1;
    maxiOsc LFO2;
    maxiOsc LFO3;
    maxiOsc LFO4;
    maxiSample samples[32];
    maxiEnv envelopes[32];
    maxiDistortion distort;
    maxiSVF filters[32];
    BOL sustain = true;
    
    
};

class maxiClock {
public:
    maxiClock();
    void ticker();
    void setTempo(double bpm);
    void setTicksPerBeat(SI4 ticksPerBeat);
    maxiOsc timer;
    SI4 currentCount;
    SI4 lastCount;
    SI4 playHead;
    double bps;
    double bpm;
    SI4 ticks;
    BOL tick;
    
};



#endif